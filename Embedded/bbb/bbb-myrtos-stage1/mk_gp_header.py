#!/usr/bin/env python3
import pathlib
import struct
import sys

LOAD_ADDR = 0x402F0400

if len(sys.argv) != 3:
    raise SystemExit(f"usage: {sys.argv[0]} stage1.bin MLO")

payload = pathlib.Path(sys.argv[1]).read_bytes()
header = struct.pack("<II", len(payload), LOAD_ADDR)
pathlib.Path(sys.argv[2]).write_bytes(header + payload)
