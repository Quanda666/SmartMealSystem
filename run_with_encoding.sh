#!/bin/bash
# 一键运行智能配餐推荐系统（自动设置编码环境）

# 获取脚本所在目录
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# 设置编码环境
if [ -f "$SCRIPT_DIR/.encoding_profile" ]; then
    source "$SCRIPT_DIR/.encoding_profile"
else
    # Fallback设置
    export LANG=C.UTF-8
    export LC_ALL=C.UTF-8
fi

echo "================================================"
echo "智能配餐推荐系统"
echo "================================================"

# 检查程序是否存在
if [ ! -f "$SCRIPT_DIR/build/bin/MealRecommendationSystem" ]; then
    echo "错误: 程序不存在，请先编译项目"
    echo "运行: cd $SCRIPT_DIR && ./build.sh"
    exit 1
fi

# 运行程序
# 切换到二进制文件目录以确保能找到 data/ 目录
cd "$SCRIPT_DIR/build/bin"
./MealRecommendationSystem
