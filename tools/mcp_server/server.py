"""
server.py — MCP stdio entry point for mssb-dtk-context server.

Tools exposed:
  lookup_struct, lookup_symbol, find_callers, lookup_rodata,
  find_struct_at_offset, find_structs_by_size, get_function_info,
  resolve_address, reindex, index_status

Startup sequence:
  1. Build full index (symbols, structs, asm, rodata)
  2. Start file watcher daemon thread (auto-reindex on file changes)
  3. Start HTTP sidecar daemon thread on :8001 (hook-triggered reindex)
  4. Enter MCP stdio loop
"""

import asyncio
import json
import sys
import threading
from pathlib import Path

import uvicorn
from mcp.server import Server
from mcp.server.stdio import stdio_server
from mcp import types

# Resolve project root (two levels up from tools/mcp_server/)
PROJECT_ROOT = Path(__file__).parent.parent.parent.resolve()

# Ensure the tools/mcp_server/ directory is importable
sys.path.insert(0, str(Path(__file__).parent))

from indexer import ProjectIndex, set_shared_index, _log
import sidecar
from watcher import start_watcher


# ---------------------------------------------------------------------------
# Startup
# ---------------------------------------------------------------------------

_log(f"Project root: {PROJECT_ROOT}")

index = ProjectIndex(PROJECT_ROOT)
set_shared_index(index)
index.build_all()

# File watcher — auto re-index on file changes
start_watcher(index, PROJECT_ROOT)

# HTTP sidecar — hook-triggered re-index
threading.Thread(
    target=lambda: uvicorn.run(
        sidecar.app,
        host="127.0.0.1",
        port=8001,
        log_level="error",
    ),
    daemon=True,
    name="sidecar",
).start()

# ---------------------------------------------------------------------------
# MCP Server
# ---------------------------------------------------------------------------

app = Server("mssb-dtk-context")

_TOOL_SCHEMAS = [
    types.Tool(
        name="lookup_struct",
        description=(
            "Return a struct/union definition with computed field offsets and sizes. "
            "Recursively resolves pointer-to types up to max_depth levels."
        ),
        inputSchema={
            "type": "object",
            "properties": {
                "type_name": {"type": "string", "description": "Struct or union name, e.g. 'ACTBone'"},
                "max_depth": {"type": "integer", "default": 2, "description": "Max pointer-type recursion depth"},
            },
            "required": ["type_name"],
        },
    ),
    types.Tool(
        name="lookup_symbol",
        description=(
            "Look up a symbol by name. Returns address, module, type, size, "
            "Ghidra cross-reference name, and extern declaration."
        ),
        inputSchema={
            "type": "object",
            "properties": {
                "name": {"type": "string", "description": "Symbol name, e.g. 'g_hugeAnimStruct' or 'fn_3_6A2A4'"},
            },
            "required": ["name"],
        },
    ),
    types.Tool(
        name="find_callers",
        description=(
            "Find all functions that call the given function via bl. "
            "Optionally include assembly snippets showing argument setup."
        ),
        inputSchema={
            "type": "object",
            "properties": {
                "func_name": {"type": "string", "description": "Function name to find callers of"},
                "include_snippets": {"type": "boolean", "default": True},
                "snippet_lines_before": {"type": "integer", "default": 12},
                "snippet_lines_after": {"type": "integer", "default": 3},
                "max_callers": {"type": "integer", "default": 5, "description": "Max callers to return snippets for"},
            },
            "required": ["func_name"],
        },
    ),
    types.Tool(
        name="lookup_rodata",
        description="Look up a rodata or data label by name. Returns float/double/int interpretations.",
        inputSchema={
            "type": "object",
            "properties": {
                "label_name": {"type": "string", "description": "Label name, e.g. 'lbl_3_rodata_28F8'"},
            },
            "required": ["label_name"],
        },
    ),
    types.Tool(
        name="find_struct_at_offset",
        description=(
            "Find structs that have a field starting at the given byte offset. "
            "Optionally filter by struct size (useful when you know a mulli stride)."
        ),
        inputSchema={
            "type": "object",
            "properties": {
                "offset": {"type": "integer", "description": "Byte offset to search for (e.g. 100 for 0x64)"},
                "size_hint": {"type": "integer", "description": "Filter to structs of exactly this size (optional)"},
            },
            "required": ["offset"],
        },
    ),
    types.Tool(
        name="find_structs_by_size",
        description="Find all structs whose computed size equals the given value.",
        inputSchema={
            "type": "object",
            "properties": {
                "size": {"type": "integer", "description": "Exact struct size in bytes (e.g. 144 for 0x90)"},
            },
            "required": ["size"],
        },
    ),
    types.Tool(
        name="get_function_info",
        description=(
            "One-stop overview of a function: address, module, size, source file, "
            "callers, and callees."
        ),
        inputSchema={
            "type": "object",
            "properties": {
                "func_name": {"type": "string", "description": "Function name, e.g. 'fn_3_6A2A4'"},
            },
            "required": ["func_name"],
        },
    ),
    types.Tool(
        name="resolve_address",
        description="Resolve a hex address to a symbol name and Ghidra name.",
        inputSchema={
            "type": "object",
            "properties": {
                "address_hex": {"type": "string", "description": "Hex address, e.g. '806A9338' or '0x806A9338'"},
            },
            "required": ["address_hex"],
        },
    ),
    types.Tool(
        name="reindex",
        description=(
            "Trigger a partial or full re-index of the project. "
            "Use after editing headers or running ninja. "
            "Valid targets: 'structs', 'asm', 'symbols', 'rodata', 'all'."
        ),
        inputSchema={
            "type": "object",
            "properties": {
                "targets": {
                    "type": "array",
                    "items": {"type": "string"},
                    "default": ["all"],
                    "description": "Sub-indexes to rebuild",
                },
            },
        },
    ),
    types.Tool(
        name="index_status",
        description="Return the current state of each sub-index: last build time, entry count, and staleness.",
        inputSchema={
            "type": "object",
            "properties": {},
        },
    ),
]


@app.list_tools()
async def list_tools() -> list[types.Tool]:
    return _TOOL_SCHEMAS


@app.call_tool()
async def call_tool(name: str, arguments: dict) -> list[types.TextContent]:
    # Wait if a rebuild is in progress
    index.wait_for_rebuild()

    try:
        result = _dispatch(name, arguments)
    except Exception as e:
        result = {"error": str(e), "tool": name}

    return [types.TextContent(type="text", text=json.dumps(result, indent=2, default=str))]


def _dispatch(name: str, args: dict) -> dict:
    if name == "lookup_struct":
        return _tool_lookup_struct(args)
    elif name == "lookup_symbol":
        return _tool_lookup_symbol(args)
    elif name == "find_callers":
        return _tool_find_callers(args)
    elif name == "lookup_rodata":
        return _tool_lookup_rodata(args)
    elif name == "find_struct_at_offset":
        return _tool_find_struct_at_offset(args)
    elif name == "find_structs_by_size":
        return _tool_find_structs_by_size(args)
    elif name == "get_function_info":
        return _tool_get_function_info(args)
    elif name == "resolve_address":
        return _tool_resolve_address(args)
    elif name == "reindex":
        return _tool_reindex(args)
    elif name == "index_status":
        return index.get_status()
    else:
        return {"error": f"Unknown tool: {name}"}


# ---------------------------------------------------------------------------
# Tool implementations
# ---------------------------------------------------------------------------

def _tool_lookup_struct(args: dict) -> dict:
    type_name = args["type_name"]
    max_depth = args.get("max_depth", 2)

    result = index.struct_db.lookup_recursive(type_name, max_depth=max_depth)
    if result is None:
        return {"error": f"Struct '{type_name}' not found"}
    return result


def _tool_lookup_symbol(args: dict) -> dict:
    name = args["name"]

    sym = index.symbol_db.lookup(name)
    ext = index.struct_db.lookup_extern(name)
    ghidra = None

    if sym:
        ghidra = index.symbol_db.ghidra_name(sym.address)

    if sym is None and ext is None:
        return {"error": f"Symbol '{name}' not found"}

    result: dict = {"name": name}

    if sym:
        result.update({
            "address": f"0x{sym.address:08X}",
            "module": sym.module,
            "section": sym.section,
            "sym_type": sym.sym_type,
            "size": f"0x{sym.size:X}",
            "scope": sym.scope,
        })

    if ghidra:
        result["ghidra_name"] = ghidra

    if ext:
        result["declaration"] = ext.declaration_text
        result["declared_type"] = ext.declared_type
        result["declared_type_is_pointer"] = ext.is_pointer

    return result


def _tool_find_callers(args: dict) -> dict:
    func_name = args["func_name"]
    include_snippets = args.get("include_snippets", True)
    lines_before = args.get("snippet_lines_before", 12)
    lines_after = args.get("snippet_lines_after", 3)
    max_callers = args.get("max_callers", 5)

    caller_names = index.asm_db.get_callers(func_name)

    result: dict = {
        "func_name": func_name,
        "caller_count": len(caller_names),
        "callers": [],
    }

    shown = caller_names[:max_callers]
    for caller in shown:
        caller_info = index.asm_db.get_func(caller)
        entry: dict = {
            "function": caller,
            "module": caller_info.module if caller_info else "unknown",
            "file": caller_info.asm_file if caller_info else "unknown",
        }
        if include_snippets:
            snippets = index.asm_db.get_caller_snippet(
                func_name, caller, lines_before, lines_after
            )
            if snippets:
                entry["snippets"] = [s["snippet"] for s in snippets]
        result["callers"].append(entry)

    if len(caller_names) > max_callers:
        result["additional_callers"] = caller_names[max_callers:]

    return result


def _tool_lookup_rodata(args: dict) -> dict:
    label = args["label_name"]
    entry = index.rodata_db.lookup(label)
    if entry is None:
        return {"error": f"Rodata label '{label}' not found"}
    return entry.to_dict()


def _tool_find_struct_at_offset(args: dict) -> dict:
    offset = args["offset"]
    size_hint = args.get("size_hint")
    matches = index.struct_db.find_field_at_offset(offset, size_hint)
    return {
        "offset": offset,
        "offset_hex": f"0x{offset:X}",
        "size_hint": size_hint,
        "match_count": len(matches),
        "matches": matches,
    }


def _tool_find_structs_by_size(args: dict) -> dict:
    size = args["size"]
    structs = index.struct_db.find_by_size(size)
    return {
        "size": size,
        "size_hex": f"0x{size:X}",
        "match_count": len(structs),
        "matches": [
            {"name": s.name, "source": f"{s.source_file}:{s.source_line}"}
            for s in structs
        ],
    }


def _tool_get_function_info(args: dict) -> dict:
    func_name = args["func_name"]

    sym = index.symbol_db.lookup(func_name)
    asm_info = index.asm_db.get_func(func_name)
    callers = index.asm_db.get_callers(func_name)
    callees = index.asm_db.get_callees(func_name)

    if sym is None and asm_info is None:
        return {"error": f"Function '{func_name}' not found"}

    result: dict = {"name": func_name}

    if sym:
        result.update({
            "address": f"0x{sym.address:08X}",
            "module": sym.module,
            "section": sym.section,
            "size": f"0x{sym.size:X}",
        })

    if asm_info:
        result.setdefault("module", asm_info.module)
        result["asm_file"] = asm_info.asm_file

    result["callers"] = callers
    result["caller_count"] = len(callers)
    result["callees"] = callees
    result["callee_count"] = len(callees)

    return result


def _tool_resolve_address(args: dict) -> dict:
    addr_str = args["address_hex"].strip()
    if addr_str.startswith("0x") or addr_str.startswith("0X"):
        addr_str = addr_str[2:]
    try:
        address = int(addr_str, 16)
    except ValueError:
        return {"error": f"Invalid hex address: {args['address_hex']}"}

    sym = index.symbol_db.resolve_address(address)
    ghidra = index.symbol_db.ghidra_name(address)

    result: dict = {"address": f"0x{address:08X}"}

    if sym:
        result["symbol_name"] = sym.name
        result["module"] = sym.module
        result["sym_type"] = sym.sym_type
    else:
        result["symbol_name"] = None

    result["ghidra_name"] = ghidra

    return result


def _tool_reindex(args: dict) -> dict:
    targets = args.get("targets", ["all"])
    if not isinstance(targets, list):
        targets = [targets]
    return index.reindex(targets)


# ---------------------------------------------------------------------------
# Entry point
# ---------------------------------------------------------------------------

async def main():
    async with stdio_server() as (read_stream, write_stream):
        await app.run(
            read_stream,
            write_stream,
            app.create_initialization_options(),
        )


if __name__ == "__main__":
    asyncio.run(main())
