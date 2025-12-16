#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import re

def fix_file_encoding(file_path):
    """Fix encoding issues in C++ files"""
    try:
        # Read file with UTF-8 encoding
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
    except UnicodeDecodeError:
        print(f"Cannot read {file_path} with UTF-8")
        return False
    
    # Replace problematic Chinese comments with English
    replacements = {
        # User.h replacements
        "// 喜欢的口味标签": "// preferred taste tags",
        "// 避免的口味标签": "// avoided taste tags", 
        "// 过敏源": "// allergens",
        "// 体重(kg)": "// weight (kg)",
        "// 身高(cm)": "// height (cm)",
        "// 性别": "// gender",
        "// 活动水平：sedentary, light, moderate, active, very_active": "// activity level: sedentary, light, moderate, active, very_active",
        "// 每日卡路里目标": "// daily calorie goal",
        "// 每日蛋白质目标(g)": "// daily protein goal (g)",
        "// 每日碳水目标(g)": "// daily carb goal (g)",
        "// 每日脂肪目标(g)": "// daily fat goal (g)",
        
        # Food.h replacements
        "// 口味标签：辣、甜、咸、酸等": "// taste tags: spicy, sweet, salty, sour, etc.",
        "// 类别：主食、蔬菜、肉类、水果等": "// category: staple food, vegetables, meat, fruit, etc.",
        "// 卡路里(kcal)": "// calories (kcal)",
        "// 蛋白质(g)": "// protein (g)",
        "// 碳水化合物(g)": "// carbohydrates (g)",
        "// 脂肪(g)": "// fat (g)",
        "// 纤维素(g)": "// fiber (g)",
        
        # Utils.cpp replacements
        '"无效输入，请输入一个整数。"': '"Invalid input, please enter an integer."',
        '"无效输入，请输入一个数字。"': '"Invalid input, please enter a number."',
        '"输入不能为空，请重试。"': '"Input cannot be empty, please try again."',
        '"按回车键继续..."': '"Press Enter to continue..."',
        
        # Add more replacements as needed
    }
    
    # Apply replacements
    original_content = content
    for chinese, english in replacements.items():
        content = content.replace(chinese, english)
    
    # Save with UTF-8 BOM for Windows compatibility
    if content != original_content:
        with open(file_path, 'w', encoding='utf-8-sig') as f:
            f.write(content)
        print(f"Fixed encoding in {file_path}")
        return True
    return False

def main():
    """Fix encoding issues in all C++ source files"""
    src_dir = '/home/engine/project/src'
    include_dir = '/home/engine/project/include'
    
    cpp_files = []
    
    # Get all .cpp and .h files
    for directory in [src_dir, include_dir]:
        for root, dirs, files in os.walk(directory):
            for file in files:
                if file.endswith(('.cpp', '.h')):
                    cpp_files.append(os.path.join(root, file))
    
    print(f"Found {len(cpp_files)} files to check")
    
    fixed_count = 0
    for file_path in cpp_files:
        if fix_file_encoding(file_path):
            fixed_count += 1
    
    print(f"Fixed encoding in {fixed_count} files")

if __name__ == "__main__":
    main()