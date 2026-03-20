"""
watcher.py — Filesystem watcher for automatic re-indexing.

Watches:
  - include/**/*.h → triggers 'structs' re-index
  - in_game.h → triggers 'structs' re-index
  - build/GYQE01/**/*.s → triggers 'asm' re-index
  - config/**/symbols.txt → triggers 'symbols' re-index

Debounces 500ms to avoid thrashing during `ninja` builds that write many files.
"""

import threading
from pathlib import Path

from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler


class _IndexWatcher(FileSystemEventHandler):
    def __init__(self, index, project_root: Path):
        self._index = index
        self._project_root = project_root
        self._pending: set[str] = set()
        self._timer: threading.Timer | None = None
        self._lock = threading.Lock()

    def on_modified(self, event):
        if event.is_directory:
            return
        self._handle(event.src_path)

    def on_created(self, event):
        if event.is_directory:
            return
        self._handle(event.src_path)

    def _handle(self, path_str: str):
        target = self._classify(path_str)
        if target is None:
            return
        with self._lock:
            self._pending.add(target)
            if self._timer:
                self._timer.cancel()
            self._timer = threading.Timer(0.5, self._fire)
            self._timer.daemon = True
            self._timer.start()

    def _classify(self, path_str: str) -> str | None:
        """Return the sub-index name that should be rebuilt for this path."""
        p = path_str.lower()
        if p.endswith(".h") or path_str.endswith("in_game.h"):
            return "structs"
        if p.endswith(".s"):
            return "asm"
        if "symbols.txt" in path_str:
            return "symbols"
        return None

    def _fire(self):
        with self._lock:
            targets = list(self._pending)
            self._pending.clear()
            self._timer = None

        if targets:
            from indexer import _log
            _log(f"Auto-reindex triggered: {targets}")
            self._index.reindex(targets)


def start_watcher(index, project_root: Path):
    """Start a background file watcher. Returns the Observer (already started)."""
    handler = _IndexWatcher(index, project_root)
    observer = Observer()

    # Watch include/ directory recursively for .h file changes
    include_dir = project_root / "include"
    if include_dir.exists():
        observer.schedule(handler, str(include_dir), recursive=True)

    # Watch build/GYQE01/ recursively for .s file changes
    build_dir = project_root / "build" / "GYQE01"
    if build_dir.exists():
        observer.schedule(handler, str(build_dir), recursive=True)

    # Watch config/ for symbols.txt changes
    config_dir = project_root / "config"
    if config_dir.exists():
        observer.schedule(handler, str(config_dir), recursive=True)

    # Watch project root (non-recursive) for in_game.h
    observer.schedule(handler, str(project_root), recursive=False)

    observer.daemon = True
    observer.start()
    return observer
