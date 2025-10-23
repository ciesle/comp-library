#!/usr/bin/env python3
import os
import re
import sys

# 再帰的に展開する関数
def expand_includes(path, included_files, base_dir="library"):
    if not os.path.exists(path):
        print(f"Warning: file not found: {path}", file=sys.stderr)
        return ""

    abs_path = os.path.abspath(path)
    if abs_path in included_files:
        return ""  # 重複展開防止

    included_files.add(abs_path)

    result_lines = [f"// ===== {path} =====\n"]
    with open(path, "r", encoding="utf-8") as f:
        for line in f:
            m = re.match(r'\s*#\s*include\s+"([^"]+)"', line)
            m2 = re.match(r'\s*(#\s*(?:pragma|include\s*<.*>.*))|using namespace std;.*', line)
            if m:
                inc_path = m.group(1)
                full_path = os.path.join(os.path.dirname(abs_path), inc_path)
                expanded = expand_includes(full_path, included_files, base_dir)
                result_lines.append(f"// >>> begin include: {inc_path}\n")
                result_lines.append(expanded)
                result_lines.append(f"// <<< end include: {inc_path}\n")
            elif not m2:
                result_lines.append(line)

    return "".join(result_lines)

def extract_library_headers_from_iwyu(iwyu_text):
    """IWYU出力から library/ 以下の add対象ヘッダを抽出"""
    add_section = False
    headers = []

    for line in iwyu_text.splitlines():
        if line.strip().startswith("main.cpp should add"):
            add_section = True
            continue
        if line.strip().startswith("main.cpp should remove"):
            add_section = False
            continue
        if add_section:
            m = re.search(r'#include\s+"(library/[^"]+)"', line)
            if m:
                headers.append(m.group(1))

    return sorted(set(headers))

def generate_combined_library(headers):
    """展開済みヘッダ群を1つにまとめる"""
    included_files = set()
    combined_output = []

    for header in headers:
        combined_output.append(expand_includes(header, included_files))
        combined_output.append("\n")

    return "".join(combined_output)

def main():
    iwyu_output = sys.stdin.read()
    print(iwyu_output, file=sys.stderr)  # デバッグ用にIWYU出力を標準エラーに表示
    headers = extract_library_headers_from_iwyu(iwyu_output)

    headers = ["library/template.h"] + extract_library_headers_from_iwyu(iwyu_output)

    with open(os.path.curdir + "/main.cpp", "r", encoding="utf-8") as f:
        main_src = f.read()

    if headers:
        expanded_code = generate_combined_library(headers)
    else:
        expanded_code = ""

    # #include "library/include.h" を置き換え
    # Use a callable replacement so backslash escapes (e.g. '\\n') in expanded_code
    # are not processed by re.sub and thus are preserved literally.
    replaced = re.sub(
        r'#\s*include\s*"library/include\.h"',
        lambda m, s=expanded_code.strip(): s,
        main_src,
        count=1,
    )
    print(replaced)


if __name__ == "__main__":
    main()