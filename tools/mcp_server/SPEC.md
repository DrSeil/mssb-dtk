# MCP Server Implementation Spec: `mssb-dtk-context`

## Overview

A Python MCP server that indexes the entire mssb-dtk project at startup and exposes fast,
structured query tools to Claude. Instead of spawning subprocesses and grepping files on
every question, Claude calls tools directly in-conversation and gets structured JSON
responses in milliseconds.

## Repository Context

- **Project:** Mario Superstar Baseball (GYQE01) matching decompilation
- **Compiler:** Metrowerks CodeWarrior GC/1.3.2 (via WiBo wrapper on Linux)
- **Working directory:** `/home/michael/Desktop/mssb-dtk`
- **Build system:** Ninja (`ninja` / `ninja report`)
- **Python version:** 3.x (already used throughout the project)

## Benefit Over Current `kappa_context.py`

| Capability                        | kappa_context.py          | MCP Server                          |
|-----------------------------------|---------------------------|--------------------------------------|
| Startup cost                      | ~2–5s per call (greps)    | One-time index at server start       |
| Struct size                       | Not computed              | Fully computed (recursive)           |
| Rodata values                     | Not shown                 | Float/int values resolved            |
| Caller search scope               | Game asm only             | All modules (game, challenge, menus, DOL) |
| Incremental queries               | Whole tool reruns         | Individual tool calls mid-conversation |
| Struct field lookup by offset     | No                        | Yes — "what field is at 0x64?"       |
| Find structs by stride/size       | No                        | Yes — "which struct is 0x90 bytes?"  |
| Ghidra name resolution            | No                        | Yes — address → Ghidra name lookup   |
| Output format                     | Text blob                 | Structured JSON                      |
| Stale data handling               | Always fresh (slow)       | Re-index on demand + file watcher    |

---

## Files to Create

```
tools/mcp_server/
├── server.py        # MCP stdio entry point, tool registration
├── sidecar.py       # Tiny FastAPI app on :8001 for hook-triggered reindex
├── indexer.py       # Builds and manages the in-memory index
├── struct_db.py     # Struct parsing, size computation, field lookup
├── asm_db.py        # Assembly file parsing: callers, callees, access patterns
├── symbol_db.py     # symbols.txt + exported_symbols.csv parsing
├── rodata_db.py     # .rodata section value extraction from asm files
└── watcher.py       # watchdog-based file watcher for automatic re-index
```

---

## Dependencies

Add to project (install via pip):
- `mcp` — official Anthropic MCP Python SDK (`pip install mcp`)
- `watchdog` — filesystem event watching (`pip install watchdog`)
- `fastapi` + `uvicorn` — already available (used by `tools/aidecomp/server.py`)

---

## Index Built at Startup

`indexer.py` reads all source files once on startup and builds five independent sub-indexes.
Each sub-index can be rebuilt independently without touching the others.

### 1. Struct Index (`struct_db.py`)

Parse `in_game.h` (28,849 lines) and all `include/**/*.h` (~230 files) into:

```python
struct_index: dict[str, StructDef]  # Keyed by "module:name" or "name" if unique

@dataclass
class FieldDef:
    name: str
    type: str           # raw type string, e.g. "struct ACTBone *"
    base_type: str      # stripped type, e.g. "ACTBone"
    is_pointer: bool
    is_array: bool
    array_count: int    # 1 if not array
    offset: int         # byte offset within struct (computed)
    size: int           # byte size of this field (computed)
    bitfield_info: dict # Optional: { "bits": 3, "offset": 0 }

@dataclass
class StructDef:
    name: str
    module: str         # "game", "challenge", "menus", "dol", or "global"
    fields: list[FieldDef]
    computed_size: int  # total size including tail padding
    alignment: int      # largest member alignment
    source_file: str
    source_line: int
```

Struct sizes are computed recursively. Primitive sizes:

```python
PRIMITIVE_SIZES = {
    "u8": 1, "s8": 1, "byte": 1, "char": 1, "bool": 1, "undefined": 1, "undefined1": 1,
    "u16": 2, "s16": 2, "short": 2, "ushort": 2, "undefined2": 2,
    "u32": 4, "s32": 4, "int": 4, "uint": 4, "float": 4, "f32": 4, "undefined4": 4,
    "u64": 8, "s64": 8, "double": 8, "f64": 8, "undefined8": 8,
    "pointer": 4,   # GameCube is 32-bit
    "void": 0,
}
```

**Alignment rules (Metrowerks/GC 1.3.2):**
- **Default:** Field alignment = min(field_size, 4) for primitives.
- **Pragmas:** The parser MUST track `#pragma options align=...` and `#pragma pack(...)`.
- Struct alignment = max field alignment among all members (constrained by current pragma).
- Each field is placed at the next offset that is a multiple of its alignment.
- Tail padding added to make total size a multiple of struct alignment.

**Parsing notes for `in_game.h` (Ghidra export quirks):**
- **Macros:** Handle the `E(base_type, name)` macro pattern by treating it as the `base_type`.
- **Bitfields:** If encountered, track bit offset within the current word. Support is "best effort."
- `pointer` is a Ghidra typedef for `void *` — treat as 4 bytes.
- `undefined` / `undefined4` etc. are Ghidra placeholders — treat as byte/word/etc.
- `struct struct { ... }` naming errors exist — skip or handle gracefully.
- Enum fields: find enum size from definition (usually `int` = 4 bytes).
- `union` fields: size = max of all union member sizes, alignment = max of member alignments.
- **Name Collisions:** Use `module:name` as the internal key (e.g., `game:minigameStruct`) to prevent collisions across RELs.

### 2. Symbol Index (`symbol_db.py`)

Parse all four `symbols.txt` files:
- `config/GYQE01/symbols.txt` — DOL symbols (absolute addresses `fn_80XXXXXX`)
- `config/GYQE01/game/symbols.txt` — Game REL symbols (`fn_3_XXXX`, `lbl_3_XXXX`)
- `config/GYQE01/challenge/symbols.txt` — Challenge REL (`fn_1_XXXX`, `lbl_1_XXXX`)
- `config/GYQE01/menus/symbols.txt` — Menus REL (`fn_2_XXXX`, `lbl_2_XXXX`)

Format of each line:
```
fn_3_6A2A4 = .text:0x0006A2A4; // type:function size:0x5C
lbl_3_bss_BA00 = .bss:0x0000BA00; // type:object size:0x8 scope:local data:4byte
```

```python
@dataclass
class SymbolDef:
    name: str
    section: str        # ".text", ".bss", ".data", ".rodata"
    address: int        # section-relative (REL) or absolute (DOL)
    module: str         # "game", "challenge", "menus", "dol"
    sym_type: str       # "function", "object"
    size: int
    scope: str          # "global", "local"
```

Also parse `exported_symbols.csv` (Ghidra export, 11,409 rows):
```csv
"Name","Location","Type","Namespace","Source","Reference Count","Offcut Ref Count"
"hugeAnimStruct","80366548","Data","Global","Default","122","0"
```

Build a secondary index: `address_hex → ghidra_name`.

### 3. Extern Declaration Index (part of `struct_db.py`)

Parse all `include/**/*.h` for `extern` declarations:

```python
@dataclass
class ExternDecl:
    symbol_name: str
    declared_type: str      # e.g. "hugeAnimStruct"
    is_pointer: bool        # e.g. "Camera *g_pCamera" → True
    declaration_text: str   # full line, e.g. "extern hugeAnimStruct g_hugeAnimStruct;"
    source_file: str
```

### 4. Assembly Index (`asm_db.py`)

Single-pass scan of all `.s` files in `build/GYQE01/` (47 files, ~22 MB total):

```python
@dataclass
class FuncAsmInfo:
    name: str
    module: str
    asm_file: str
    start_line: int
    lines: list[str]        # raw asm lines (stripped)
    callers: list[str]      # function names that call this via `bl` or `b` (tail call)
    callees: list[str]      # functions this calls via `bl` or `b` (tail call)
```

**Call Graph Tracking:**
- Include `bl` (standard call).
- Include `b` (unconditional branch) if the target is a known function entry point (tail call optimization).
- Ignore local branches (e.g., `b .L_00...`).

Build:
- `func_index: dict[str, FuncAsmInfo]` — keyed by function name
- `callers_index: dict[str, list[str]]` — func_name → list of callers
- `callees_index: dict[str, list[str]]` — func_name → list of callees

Parse `.fn FuncName, global` / `.endfn FuncName` delimiters. Strip trailing comma from
function names (`.fn fn_3_6C4D0, global` → `fn_3_6C4D0`).

### 5. Rodata Index (`rodata_db.py`)

Scan `.s` files for rodata labels and their values. Rodata appears in two forms:

```asm
lbl_3_rodata_FD0:
    .float 1.0

lbl_3_rodata_FD4:
    .4byte 0x3F800000
```

```python
@dataclass
class RotadaEntry:
    label: str
    raw_hex: str        # e.g. "3F800000"
    as_float: float     # IEEE 754 interpretation
    as_int: int         # unsigned int interpretation
    source_file: str
    source_line: int
```

---

## Tools Exposed to Claude

### `lookup_struct(type_name, max_depth=3)`

Returns the struct definition with fields, offsets, sizes, and recursively resolved
pointer-to types up to `max_depth` levels.

**Input:** `{ "type_name": "ACTBone", "max_depth": 3 }`

**Output:**
```json
{
  "name": "ACTBone",
  "computed_size": 144,
  "alignment": 4,
  "source": "in_game.h:889",
  "fields": [
    {"name": "boneID",    "type": "short",      "offset": 0,  "size": 2},
    {"name": "branch",    "type": "DSBranch",   "offset": 4,  "size": 16},
    {"name": "dispObj",   "type": "DODisplayObj *", "offset": 20, "size": 4},
    {"name": "orientationCtrl", "type": "CTRLControl", "offset": 24, "size": 32},
    ...
  ],
  "pointer_types": {
    "DSBranch": { "computed_size": 16, "fields": [...] },
    "CTRLControl": { "computed_size": 32, "fields": [...] }
  }
}
```

### `lookup_symbol(name)`

Returns address, module, type, size, Ghidra name, and extern declaration.

**Input:** `{ "name": "g_hugeAnimStruct" }`

**Output:**
```json
{
  "name": "g_hugeAnimStruct",
  "address": "0x8036E548",
  "module": "dol",
  "section": ".data",
  "sym_type": "object",
  "size": "0x3154",
  "ghidra_name": "hugeAnimStruct",
  "declaration": "extern hugeAnimStruct g_hugeAnimStruct;",
  "declared_type": "hugeAnimStruct",
  "declared_type_is_pointer": false
}
```

### `find_callers(func_name, include_snippets=true, snippet_lines_before=12, snippet_lines_after=3)`

Returns all callers across all modules with optional assembly snippets showing argument
setup before the `bl` instruction.

**Input:** `{ "func_name": "fn_3_6A2A4", "include_snippets": true, "snippet_lines_before": 12 }`

**Output:**
```json
{
  "func_name": "fn_3_6A2A4",
  "caller_count": 2,
  "callers": [
    {
      "function": "fn_3_6C4D0",
      "module": "game",
      "file": "build/GYQE01/game/asm/game/rep_F80.s",
      "snippet": [
        "/* 0006C730 */ lbz r3, 0x252(r24)",
        ">>> /* 0006C734 */ bl fn_3_6A2A4",
        "/* 0006C738 */ b .L_0006C828"
      ]
    },
    ...
  ]
}
```

### `lookup_rodata(label_name)`

Finds a rodata label and returns its raw bytes and float/int interpretations.

**Input:** `{ "label_name": "lbl_3_rodata_FD0" }`

**Output:**
```json
{
  "label": "lbl_3_rodata_FD0",
  "raw_hex": "3F800000",
  "as_float": 1.0,
  "as_int": 1065353216,
  "source": "build/GYQE01/game/asm/game/rep_F80.s:42"
}
```

### `find_struct_at_offset(offset, size_hint=null)`

Returns structs that have a field starting at the given byte offset. Optionally filter by
struct size (useful when you know the mulli stride).

**Input:** `{ "offset": 100, "size_hint": 144 }`  (0x64 = 100, 0x90 = 144)

**Output:**
```json
{
  "matches": [
    {
      "struct": "hugeAnimStruct",
      "field_name": "actRelatedPtr",
      "field_type": "ACTBone *",
      "offset": 100,
      "struct_size": 12628
    }
  ]
}
```

### `find_structs_by_size(size)`

Returns all structs whose computed size equals `size`.

**Input:** `{ "size": 144 }`  (0x90 = 144)

**Output:**
```json
{
  "size": 144,
  "matches": [
    {"name": "ACTBone", "source": "in_game.h:889"},
    {"name": "HugeAnimEntry68", "source": "include/UnknownHeaders.h:129"}
  ]
}
```

### `get_function_info(func_name)`

One-stop overview: address, module, size, source file (from splits.txt), callers, callees.

**Input:** `{ "func_name": "fn_3_6A2A4" }`

**Output:**
```json
{
  "name": "fn_3_6A2A4",
  "address": "0x0006A2A4",
  "module": "game",
  "section": ".text",
  "size": "0x5C",
  "source_file": "game/rep_F80.c",
  "callers": ["fn_3_6C4D0", "fn_3_E1478"],
  "callees": []
}
```

### `resolve_address(address_hex)`

Takes a hex address and returns symbol name, module, and Ghidra name.

**Input:** `{ "address_hex": "806A9338" }`

**Output:**
```json
{
  "address": "0x806A9338",
  "symbol_name": "fn_3_6A2A4",
  "module": "game",
  "ghidra_name": null
}
```

### `reindex(targets=["all"])`

Triggers a partial or full re-index. Valid target values: `"structs"`, `"asm"`,
`"symbols"`, `"rodata"`, `"all"`.

**Input:** `{ "targets": ["structs"] }`

**Output:**
```json
{
  "reindexed": ["structs", "externs"],
  "duration_ms": 2340,
  "struct_count": 847,
  "extern_count": 312
}
```

### `index_status()`

Returns when each sub-index was last built, how many entries it has, and whether any
source files have been modified since the index was built.

**Output:**
```json
{
  "sub_indexes": [
    {
      "name": "structs",
      "last_built": "2026-03-20T14:32:11",
      "entry_count": 847,
      "stale": false
    },
    {
      "name": "asm",
      "last_built": "2026-03-20T14:30:05",
      "entry_count": 2126,
      "stale": true,
      "stale_reason": "3 .s files modified since last index"
    }
  ]
}
```

---

## Re-indexing Strategy

### Why Re-indexing Is Critical

The index goes stale on every decompilation iteration:
- **Struct/header edits** (`UnknownHomes_Game.h`, `UnknownHeaders.h`) invalidate
  `lookup_struct` and `find_struct_at_offset`
- **New `ninja` builds** regenerate `.s` asm files, invalidating caller/callee data
  and rodata values
- **New `extern` declarations** in headers invalidate `lookup_symbol`

Without re-indexing, Claude would make decisions based on stale struct layouts — exactly
the kind of subtle bug that causes wrong decompilation.

### Sub-index Partitioning

Re-index only the affected partition, not everything:

| Sub-index       | Source files                              | Typical re-index time |
|-----------------|-------------------------------------------|-----------------------|
| `structs`       | `in_game.h`, `include/**/*.h`             | ~2–3s                 |
| `externs`       | `include/**/*.h`                          | ~1s (runs with structs) |
| `asm`           | `build/GYQE01/**/asm/**/*.s`             | ~5–10s                |
| `rodata`        | `build/GYQE01/**/asm/**/*.s`             | ~3–5s (runs with asm) |
| `symbols`       | `config/**/symbols.txt`                   | <1s                   |

`structs` and `externs` always rebuild together. `asm` and `rodata` always rebuild together.

### Trigger 1: Explicit `reindex()` Tool Call

Claude calls `reindex(["structs"])` after editing a header, or `reindex(["asm"])` after
a build. Always available as the explicit escape hatch.

### Trigger 2: File Watcher (Automatic)

`watcher.py` uses the `watchdog` library to watch the filesystem in a daemon thread
inside `server.py`. On file change, it debounces 500ms (to avoid thrashing during a
ninja build writing many `.s` files simultaneously), then re-indexes the affected
sub-index.

```python
# watcher.py
import threading
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

class IndexWatcher(FileSystemEventHandler):
    def __init__(self, index):
        self.index = index
        self._pending = set()
        self._timer = None
        self._lock = threading.Lock()

    def on_modified(self, event):
        if event.is_directory:
            return
        path = event.src_path
        with self._lock:
            if path.endswith('.h') or path == 'in_game.h':
                self._pending.add('structs')
            elif path.endswith('.s'):
                self._pending.add('asm')
            elif 'symbols.txt' in path:
                self._pending.add('symbols')
            self._schedule()

    def _schedule(self):
        if self._timer:
            self._timer.cancel()
        self._timer = threading.Timer(0.5, self._do_reindex)
        self._timer.start()

    def _do_reindex(self):
        with self._lock:
            targets = list(self._pending)
            self._pending.clear()
        for target in targets:
            self.index.reindex([target])

def start_watcher(index, project_root):
    observer = Observer()
    handler = IndexWatcher(index)
    observer.schedule(handler, path=str(project_root / 'include'), recursive=True)
    observer.schedule(handler, path=str(project_root / 'build'), recursive=True)
    observer.schedule(handler, path=str(project_root / 'config'), recursive=True)
    observer.schedule(handler, str(project_root / 'in_game.h'))
    observer.daemon = True
    observer.start()
    return observer
```

### Trigger 3: Post-Build Hook via Sidecar

Claude Code's `PostToolUse` hook fires after every `ninja` bash command. The hook calls
a tiny HTTP sidecar (`sidecar.py`) running on port 8001, which signals the MCP server's
shared index to re-index the asm partition.

The MCP server itself must stay pure stdio (MCP protocol requirement), so the sidecar
imports the same `ProjectIndex` singleton and triggers re-index directly.

**`sidecar.py`:**
```python
from fastapi import FastAPI
import uvicorn
from indexer import get_shared_index   # singleton accessor

app = FastAPI()

@app.post("/reindex/{target}")
async def reindex(target: str):
    index = get_shared_index()
    result = index.reindex([target])
    return result

if __name__ == "__main__":
    uvicorn.run(app, host="127.0.0.1", port=8001)
```

Start the sidecar as a background thread inside `server.py` on startup:
```python
threading.Thread(
    target=lambda: uvicorn.run(sidecar.app, host="127.0.0.1", port=8001, log_level="error"),
    daemon=True
).start()
```

**`~/.claude/settings.json` hook addition:**
```json
{
  "hooks": {
    "PostToolUse": [
      {
        "matcher": "Bash(ninja*)",
        "hooks": [
          {
            "type": "command",
            "command": "curl -s -X POST http://localhost:8001/reindex/asm || true"
          }
        ]
      }
    ]
  }
}
```

### Thread Safety and Coordination

The index is a shared resource accessed by:
1. The MCP stdio server's async event loop.
2. The file watcher thread (`watcher.py`).
3. The HTTP sidecar (`sidecar.py`).

**Locking Strategy:**
- Use a `threading.RLock` (Recursive Lock) for all synchronous read/write operations within the `ProjectIndex` and sub-DBs.
- The MCP server's `call_tool` implementation should be aware of pending re-indexes. If a re-index is in progress, the tool call should wait until it completes to ensure data consistency.
- Implement a `status` flag on the indexer to track when a rebuild is active.

---

## MCP Server Entry Point (`server.py`)

```python
import asyncio
import threading
import uvicorn
from pathlib import Path
from mcp.server import Server
from mcp.server.stdio import stdio_server
from mcp import types

from indexer import ProjectIndex
import sidecar
from watcher import start_watcher

PROJECT_ROOT = Path(__file__).parent.parent.parent  # /home/michael/Desktop/mssb-dtk

# Build index at startup
index = ProjectIndex(PROJECT_ROOT)
index.build_all()

# Start file watcher daemon thread
start_watcher(index, PROJECT_ROOT)

# Start HTTP sidecar daemon thread (for hook-triggered reindex)
threading.Thread(
    target=lambda: uvicorn.run(sidecar.app, host="127.0.0.1", port=8001, log_level="error"),
    daemon=True
).start()

app = Server("mssb-dtk")

@app.list_tools()
async def list_tools() -> list[types.Tool]:
    return [
        types.Tool(name="lookup_struct",        inputSchema=...),
        types.Tool(name="lookup_symbol",        inputSchema=...),
        types.Tool(name="find_callers",         inputSchema=...),
        types.Tool(name="lookup_rodata",        inputSchema=...),
        types.Tool(name="find_struct_at_offset",inputSchema=...),
        types.Tool(name="find_structs_by_size", inputSchema=...),
        types.Tool(name="get_function_info",    inputSchema=...),
        types.Tool(name="resolve_address",      inputSchema=...),
        types.Tool(name="reindex",              inputSchema=...),
        types.Tool(name="index_status",         inputSchema=...),
    ]

@app.call_tool()
async def call_tool(name: str, arguments: dict) -> list[types.TextContent]:
    result = dispatch(name, arguments)  # route to appropriate index method
    return [types.TextContent(type="text", text=json.dumps(result, indent=2))]

async def main():
    async with stdio_server() as streams:
        await app.run(*streams, app.create_initialization_options())

asyncio.run(main())
```

---

## Claude Code Configuration

Add `mcpServers` to `~/.claude/settings.json`:

```json
{
  "mcpServers": {
    "mssb-dtk": {
      "type": "stdio",
      "command": "python3",
      "args": ["/home/michael/Desktop/mssb-dtk/tools/mcp_server/server.py"],
      "env": {
        "PYTHONPATH": "/home/michael/Desktop/mssb-dtk"
      }
    }
  },
  "hooks": {
    "PostToolUse": [
      {
        "matcher": "Bash(ninja*)",
        "hooks": [
          {
            "type": "command",
            "command": "curl -s -X POST http://localhost:8001/reindex/asm || true"
          }
        ]
      }
    ]
  }
}
```

---

## Implementation Order

Build in this order — each step is independently testable:

1. **`symbol_db.py`** — Simplest: parse fixed-format text files. Test: `lookup_symbol("g_hugeAnimStruct")` returns correct address.
2. **`struct_db.py`** — Core value. Parse headers, compute sizes. Test: `lookup_struct("ACTBone")` returns `computed_size=144` (verify against known sizes in the codebase).
3. **`rodata_db.py`** — Parse `.s` files for float labels. Test: `lookup_rodata("lbl_3_rodata_FD0")` returns `as_float=1.0`.
4. **`asm_db.py`** — Heaviest. Single-pass asm scan. Test: `find_callers("fn_3_6A2A4")` returns `fn_3_6C4D0` and `fn_3_E1478` with correct snippets.
5. **`indexer.py`** — Wire the four DBs into a `ProjectIndex` class with `build_all()` and `reindex(targets)`.
6. **`server.py`** — MCP stdio server. Wire tool calls to index methods.
7. **`watcher.py`** — File watcher. Verify debounce works during `ninja` builds.
8. **`sidecar.py`** — HTTP trigger. Test: `curl -X POST localhost:8001/reindex/asm` triggers re-index.
9. **Settings** — Update `~/.claude/settings.json` with `mcpServers` and `hooks`.
10. **End-to-end test** — Start server, edit a header, verify `index_status()` shows stale, verify auto-reindex fires within 1 second.

---

## Notes and Gotchas

- **`in_game.h` is 28,849 lines** — parse with a single `f.read()` + regex, not line-by-line, for speed. Target: full struct parse in under 3 seconds.
- **Ghidra export quirks** — `pointer` = 4 bytes, `undefined` = 1 byte, `undefined4` = 4 bytes, `undefined2` = 2 bytes. The file has some struct naming errors (`struct struct`); skip gracefully.
- **Function names in asm** — `.fn fn_3_6C4D0, global` has a trailing comma before `global`. Strip it when capturing the name.
- **Enum sizes** — Ghidra exports enums with `int` underlying type. Treat all enums as 4 bytes unless explicitly declared as `byte` or `short` variants.
- **Union sizes** — size = max of all member sizes, alignment = max of all member alignments.
- **The sidecar port 8001** — Check it doesn't conflict with `tools/aidecomp/server.py`. The ADS Dashboard uses port 8000 by default, so 8001 is free.
- **Thread safety** — The index is read by the MCP stdio server's async event loop and written by the file watcher thread and sidecar. Protect with `threading.RLock` around all index reads and writes.
- **Startup time** — Full index build targets under 15 seconds. Log progress to stderr so the user can see it starting up: `"[mssb-dtk] Indexing structs... done (847 structs, 2.1s)"`.
- **MCP SDK version** — Use `mcp>=1.0.0`. The API for `stdio_server` and `Server` may vary; refer to the official Anthropic MCP Python SDK docs.
