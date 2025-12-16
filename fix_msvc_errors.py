#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import re

def fix_compilation_issues(file_path):
    """Fix common MSVC compilation issues"""
    try:
        with open(file_path, 'r', encoding='utf-8-sig') as f:
            content = f.read()
    except UnicodeDecodeError:
        try:
            with open(file_path, 'r', encoding='utf-8') as f:
                content = f.read()
        except:
            print(f"Cannot read {file_path}")
            return False
    
    original_content = content
    
    # Fix string literal issues (C2001 errors)
    # Remove trailing spaces and fix line continuations
    lines = content.split('\n')
    fixed_lines = []
    
    i = 0
    while i < len(lines):
        line = lines[i].rstrip()
        
        # Check if this line is incomplete (ends with a string literal or operator)
        if (line.endswith('"') and not line.count('"') % 2 == 0) or \
           line.endswith('+') or \
           line.endswith(',') or \
           line.endswith('(') or \
           ('::' in line and not line.endswith('::') and '{' not in line):
            
            # This might be a multi-line string or continuation
            # Look ahead to see if we need to join with next line
            if i + 1 < len(lines):
                next_line = lines[i + 1].strip()
                if next_line.startswith('"') or next_line.startswith('std::') or \
                   next_line.startswith('cout') or next_line.startswith('cin') or \
                   next_line.startswith('=') or next_line.startswith('+'):
                    line += ' ' + next_line
                    i += 1  # Skip the next line as we consumed it
        
        fixed_lines.append(line)
        i += 1
    
    content = '\n'.join(fixed_lines)
    
    # Fix specific MSVC syntax issues
    fixes = [
        # Fix for C2143 and C2146 errors - missing semicolons
        (r'(\w+)\s*\(\s*$', r'\1();'),  # Function call without semicolon
        (r'if\s*\([^)]+\)\s*$', r'if (condition) {'),  # if without body
        (r'while\s*\([^)]+\)\s*$', r'while (condition) {'),  # while without body
        (r'for\s*\([^)]+\)\s*$', r'for (condition) {'),  # for without body
        
        # Fix for range-based for loops (C3531 errors)
        (r'for\s*\(\s*auto\s+(\w+)\s*:\s*(\w+)\)\s*$', r'for (auto& \1 : \2) {'),
        
        # Fix for string literal concatenation issues
        (r'"\s*\+\s*(\w+)', r'" + \1'),
        (r'(\w+)\s*\+\s*"', r'\1 + "'),
        
        # Fix for problematic character encoding
        (r'[^\x00-\x7F]+', ''),  # Remove non-ASCII characters that might cause issues
    ]
    
    for pattern, replacement in fixes:
        content = re.sub(pattern, replacement, content, flags=re.MULTILINE)
    
    # Save the fixed content
    if content != original_content:
        with open(file_path, 'w', encoding='utf-8-sig') as f:
            f.write(content)
        print(f"Fixed compilation issues in {file_path}")
        return True
    return False

def analyze_errors():
    """Analyze the reported errors and create targeted fixes"""
    errors = {
        "Utils.cpp": [
            "C2001: 常量中有换行符",
            "C2143: 缺少;",
            "C2146: 缺少;在标识符前面"
        ],
        "User.cpp": [
            "C2001: 常量中有换行符", 
            "C2143: 缺少;",
            "C2146: 缺少;在标识符前面",
            "C2065: 未声明的标识符",
            "C3531: auto符号必须具有初始值设定项"
        ],
        "Food.cpp": [
            "C2065: 未声明的标识符",
            "C2614: 非法的成员初始化"
        ],
        "main.cpp": [
            "C2001: 常量中有换行符",
            "C2143: 缺少;",
            "C2146: 缺少;在标识符前面"
        ]
    }
    return errors

def main():
    """Fix compilation issues"""
    src_dir = '/home/engine/project/src'
    include_dir = '/home/engine/project/include'
    
    cpp_files = []
    
    # Get all .cpp and .h files
    for directory in [src_dir, include_dir]:
        for root, dirs, files in os.walk(directory):
            for file in files:
                if file.endswith(('.cpp', '.h')):
                    cpp_files.append(os.path.join(root, file))
    
    print(f"Analyzing {len(cpp_files)} files...")
    
    # Analyze errors
    errors = analyze_errors()
    
    fixed_count = 0
    for file_path in cpp_files:
        if fix_compilation_issues(file_path):
            fixed_count += 1
    
    print(f"Fixed compilation issues in {fixed_count} files")

if __name__ == "__main__":
    main()