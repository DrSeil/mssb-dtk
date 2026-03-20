"""
sidecar.py — Tiny HTTP sidecar for hook-triggered reindex.

Runs on port 8001 as a daemon thread inside server.py.
Accepts POST /reindex/<target> and triggers the shared ProjectIndex.

Claude Code's PostToolUse hook (after ninja) calls:
  curl -s -X POST http://localhost:8001/reindex/asm || true
"""

from fastapi import FastAPI, HTTPException
from fastapi.responses import JSONResponse

app = FastAPI(title="mssb-dtk reindex sidecar")

VALID_TARGETS = {"symbols", "structs", "asm", "rodata", "all"}


@app.post("/reindex/{target}")
async def reindex(target: str):
    if target not in VALID_TARGETS:
        raise HTTPException(
            status_code=400,
            detail=f"Unknown target '{target}'. Valid: {sorted(VALID_TARGETS)}"
        )

    from indexer import get_shared_index
    index = get_shared_index()
    if index is None:
        raise HTTPException(status_code=503, detail="Index not initialized yet")

    targets = ["asm", "rodata"] if target in ("asm", "rodata") else [target]
    if target == "all":
        targets = ["all"]

    result = index.reindex(targets)
    return JSONResponse(content=result)


@app.get("/status")
async def status():
    from indexer import get_shared_index
    index = get_shared_index()
    if index is None:
        return JSONResponse(content={"status": "not initialized"})
    return JSONResponse(content=index.get_status())
