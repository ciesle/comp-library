#!/usr/bin/env python3
import os
import re
import sys

def should_strip_header_line(line):
    """展開したヘッダから不要なトップレベル行を取り除く"""
    return bool(re.match(
        r'^\s*(?:#\s*(?:pragma\s+once\b.*|include\s*<[^>]+>.*)|using\s+namespace\s+std\s*;.*)$',
        line,
    ))

def extract_library_headers_from_iwyu(iwyu_text):
    """IWYU出力から必要な library/ 以下のヘッダを抽出"""
    add_section = False
    full_include_section = False
    add_headers = []
    full_include_headers = []

    for line in iwyu_text.splitlines():
        stripped = line.strip()

        if stripped.startswith("main.cpp should add"):
            add_section = True
            continue
        if stripped.startswith("main.cpp should remove"):
            add_section = False
            continue

        if stripped.startswith("The full include-list for main.cpp:"):
            full_include_section = True
            continue
        if full_include_section and stripped == "---":
            full_include_section = False
            continue

        m = re.search(r'#include\s+"(library/[^"]+)"', line)
        if not m:
            continue

        if add_section:
            add_headers.append(m.group(1))
        if full_include_section:
            full_include_headers.append(m.group(1))

    if full_include_headers:
        return dedupe_preserve_order(full_include_headers)
    return dedupe_preserve_order(add_headers)

def dedupe_preserve_order(headers):
    seen = set()
    result = []

    for header in headers:
        if header not in seen:
            seen.add(header)
            result.append(header)

    return result


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
            m = re.match(r'\s*#\s*include\s*"([^"]+)"', line)
            if m:
                inc_path = m.group(1)
                full_path = os.path.join(os.path.dirname(abs_path), inc_path)
                expanded = expand_includes(full_path, included_files, base_dir)
                result_lines.append(f"// >>> begin include: {inc_path}\n")
                result_lines.append(expanded)
                result_lines.append(f"// <<< end include: {inc_path}\n")
            elif not should_strip_header_line(line):
                result_lines.append(line)

    return "".join(result_lines)

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
    if "library/template.h" in headers:
        headers = ["library/template.h"] + [header for header in headers if header != "library/template.h"]

    with open(os.path.curdir + "/main.cpp", "r", encoding="utf-8") as f:
        main_src = f.read()

    if headers:
        expanded_code = generate_combined_library(headers)
    else:
        expanded_code = ""

    # #include "library/..." をすべて置き換え
    # Use a callable replacement so backslash escapes (e.g. '\\n') in expanded_code
    # are not processed by re.sub and thus are preserved literally.
    replaced_library_include = False

    def replace_library_include(_):
        nonlocal replaced_library_include
        if replaced_library_include:
            return ""
        replaced_library_include = True
        return expanded_code.strip()

    replaced = re.sub(
        r'^[^\S\n]*#[^\S\n]*include[^\S\n]*"library/[^"]+"[^\S\n]*$',
        replace_library_include,
        main_src,
        flags=re.MULTILINE,
    )
    print(replaced)


if __name__ == "__main__":
    main()
