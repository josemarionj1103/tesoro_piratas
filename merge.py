import os
import re

files_order = [
    "grafo.h",
    "hash.h",
    "arbol.h",
    "explorador.h",
    "archivos.h",
    "grafo.cpp",
    "hash.cpp",
    "arbol.cpp",
    "explorador.cpp",
    "archivos.cpp",
    "main.cpp"
]

base_dir = r"c:\Users\Hp\Desktop\Tesor_Piratas"
out_file = os.path.join(base_dir, "main_combined.cpp")

standard_includes = set()
code_blocks = []

include_pattern = re.compile(r'^\s*#include\s*<([^>]+)>')
local_include_pattern = re.compile(r'^\s*#include\s*"([^"]+)"')
guard_pattern = re.compile(r'^\s*#(ifndef|define)\s+[A-Za-z0-9_]+_H\b')
endif_pattern = re.compile(r'^\s*#endif')
pragma_once_pattern = re.compile(r'^\s*#pragma\s+once')

for fname in files_order:
    fpath = os.path.join(base_dir, fname)
    if not os.path.exists(fpath):
        continue
    
    with open(fpath, "r", encoding="utf-8") as f:
        lines = f.readlines()
        
    filtered_lines = []
    for line in lines:
        m_std = include_pattern.match(line)
        if m_std:
            standard_includes.add(m_std.group(1))
            continue
            
        m_loc = local_include_pattern.match(line)
        if m_loc:
            continue
            
        if guard_pattern.match(line) or endif_pattern.match(line):
            continue
            
        if pragma_once_pattern.match(line):
            continue
            
        # also ignore main.cpp local includes, just in case
        
        filtered_lines.append(line)
        
    code_blocks.append(f"// --- BEGIN {fname} ---\n" + "".join(filtered_lines) + f"\n// --- END {fname} ---\n")

with open(out_file, "w", encoding="utf-8") as f:
    for inc in sorted(standard_includes):
        f.write(f"#include <{inc}>\n")
    f.write("\n")
    for block in code_blocks:
        f.write(block)
        f.write("\n")

print(f"Combined into {out_file}")
