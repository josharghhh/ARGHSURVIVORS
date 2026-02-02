#!/usr/bin/env python3
"""
Quick, offline inspector for Arma Reforger persistence .blob files.

This does not fully decode the binary format, but it gives you:
- file size and basic metadata
- offsets for top-level persistence sections
- nearby header integers (best-effort)
- extracted printable strings with offsets

Usage:
  python3 tools/blob_inspect.py /path/to/file.blob
  python3 tools/blob_inspect.py /path/to/file.blob --json out.json
  python3 tools/blob_inspect.py /path/to/file.blob --strings out.txt
"""

from __future__ import annotations

import argparse
import json
import os
from pathlib import Path
from typing import Iterable


SECTION_NAMES = (
    "System",
    "Player",
    "Character",
    "Vehicle",
    "Storage",
    "Item",
    "Logic",
    "AIWaypoint",
    "MilitaryBase",
    "Turret",
)


def read_u32(data: bytes, offset: int) -> int | None:
    if offset < 0 or offset + 4 > len(data):
        return None
    return int.from_bytes(data[offset : offset + 4], "little", signed=False)


def iter_strings(data: bytes, min_len: int = 4) -> Iterable[tuple[int, str]]:
    start = None
    for idx, b in enumerate(data):
        is_print = 32 <= b <= 126
        if is_print:
            if start is None:
                start = idx
        else:
            if start is not None:
                if idx - start >= min_len:
                    yield start, data[start:idx].decode("ascii", errors="replace")
                start = None
    if start is not None and len(data) - start >= min_len:
        yield start, data[start:].decode("ascii", errors="replace")


def find_sections(data: bytes) -> list[dict[str, object]]:
    results: list[dict[str, object]] = []
    for name in SECTION_NAMES:
        needle = name.encode("ascii")
        start = 0
        while True:
            idx = data.find(needle, start)
            if idx == -1:
                break
            after = idx + len(needle)
            entry = {
                "name": name,
                "offset": idx,
                "u32_before": [read_u32(data, idx - 4 * n) for n in (1, 2, 3)],
                "u32_after": [read_u32(data, after + 4 * n) for n in range(3)],
            }
            results.append(entry)
            start = idx + 1
    return results


def main() -> int:
    parser = argparse.ArgumentParser(description="Inspect Arma Reforger .blob save files")
    parser.add_argument("blob", type=Path, help="path to a .blob file")
    parser.add_argument("--json", type=Path, help="write summary to JSON")
    parser.add_argument("--strings", type=Path, help="write extracted strings to text file")
    parser.add_argument("--min-string-len", type=int, default=4, help="minimum string length")
    args = parser.parse_args()

    path = args.blob
    if not path.exists():
        raise SystemExit(f"blob not found: {path}")

    data = path.read_bytes()
    summary: dict[str, object] = {
        "path": str(path),
        "size_bytes": len(data),
        "sections": find_sections(data),
    }

    strings = list(iter_strings(data, min_len=args.min_string_len))
    summary["string_count"] = len(strings)

    if args.strings:
        args.strings.parent.mkdir(parents=True, exist_ok=True)
        with args.strings.open("w", encoding="utf-8") as f:
            for offset, text in strings:
                f.write(f"{offset:08x}  {text}\n")

    if args.json:
        args.json.parent.mkdir(parents=True, exist_ok=True)
        with args.json.open("w", encoding="utf-8") as f:
            json.dump(summary, f, indent=2)

    print(f"file: {path}")
    print(f"size: {len(data)} bytes")
    print(f"sections: {len(summary['sections'])}")
    print(f"strings: {len(strings)} (min len {args.min_string_len})")
    if args.json:
        print(f"wrote: {args.json}")
    if args.strings:
        print(f"wrote: {args.strings}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
