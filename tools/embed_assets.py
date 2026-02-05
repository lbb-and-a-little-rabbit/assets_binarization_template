#!/usr/bin/env python3
# embed_assets.py
# Usage: python embed_assets.py <assets_dir> <out_dir>
# Produces: assets_data.cpp and assets_symbols.h in out_dir

import sys
import os
import re

def sanitize(name):
    # keep letters, digits and underscore; replace others with underscore
    return re.sub(r'[^A-Za-z0-9_]', '_', name)

def relpath_from_assets(assets_dir, fullpath):
    # compute relative path from assets_dir and drop leading separators
    rp = os.path.relpath(fullpath, assets_dir)
    rp = rp.replace('\\', '/')
    return rp

def make_symbol_from_relpath(relpath):
    # remove extension, make pieces joined by underscore
    base = os.path.splitext(relpath)[0]  # remove .png
    base = base.replace('/', '_')
    return sanitize(base)

def emit_header(hpath, entries):
    with open(hpath, 'w', newline='\n') as f:
        f.write('// Auto-generated asset symbols header\n#pragma once\n\n#include <cstddef>\n\n')
        for sym, size in entries:
            f.write(f'extern const unsigned char {sym}[];\n')
            f.write(f'extern const std::size_t {sym}_size;\n')
            macro = sym
            f.write(f'#define ASSET_START_{macro} ({sym})\n')
            f.write(f'#define ASSET_END_{macro}   ({sym} + {sym}_size)\n\n')

def emit_cpp(cpppath, entries, assets_dir):
    with open(cpppath, 'w', newline='\n') as f:
        f.write('// Auto-generated asset data\n#include <cstddef>\n\n')
        for sym, size in entries:
            # find original file path by reversing name? We have passed full list, so pass mapping.
            pass
        # We'll generate arrays by reading files again from mapping
        # entries is list of (sym, filepath)
        for sym, filepath in entries:
            data = open(filepath, 'rb').read()
            f.write(f'// {os.path.basename(filepath)}\n')
            f.write(f'extern const unsigned char {sym}[] = {{\n')
            # write hex bytes 12 per line
            for i in range(0, len(data), 12):
                chunk = data[i:i+12]
                line = ', '.join(f'0x{b:02x}' for b in chunk)
                f.write('    ' + line)
                if i + 12 < len(data):
                    f.write(',\n')
                else:
                    f.write(',\n')
            f.write('};\n')
            f.write(f'extern const std::size_t {sym}_size = {len(data)};\n\n')

def main():
    if len(sys.argv) < 3:
        print("Usage: embed_assets.py <assets_dir> <out_dir>", file=sys.stderr)
        return 1
    assets_dir = sys.argv[1]
    out_dir = sys.argv[2]
    if not os.path.isdir(assets_dir):
        print("assets_dir not found: " + assets_dir, file=sys.stderr)
        return 2
    os.makedirs(out_dir, exist_ok=True)

    entries = []
    # walk assets dir, collect png files in deterministic order
    files = []
    for root, dirs, filenames in os.walk(assets_dir):
        filenames.sort()
        dirs.sort()
        for fn in filenames:
            if fn.lower().endswith('.png'):
                files.append(os.path.join(root, fn))
    files.sort()
    for fp in files:
        rel = relpath_from_assets(assets_dir, fp)  # e.g. "attack/attack0001.png"
        sym = make_symbol_from_relpath(rel)        # e.g. "attack_attack0001"
        entries.append((sym, fp))

    # emit header and cpp
    header_path = os.path.join(out_dir, 'assets_symbols.h')
    cpp_path = os.path.join(out_dir, 'assets_data.cpp')

    # header entries: list of (sym, size) - but we'll write sizes as separate const in cpp
    header_entries = [(sym, None) for sym,fp in entries]
    emit_header(header_path, header_entries)
    emit_cpp(cpp_path, entries, assets_dir)

    print("Wrote", header_path, cpp_path)
    return 0

if __name__ == '__main__':
    sys.exit(main())