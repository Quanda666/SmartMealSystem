#!/bin/bash

echo "=================================="
echo "智能配餐推荐系统 - 编译脚本"
echo "=================================="

# 创建构建目录
if [ ! -d "build" ]; then
    echo "创建构建目录..."
    mkdir -p build
fi

# 进入构建目录
cd build

# 运行CMake
echo "配置CMake..."
cmake .. || { echo "CMake配置失败！"; exit 1; }

# 编译项目
echo "编译项目..."
make -j4 || { echo "编译失败！"; exit 1; }

echo ""
echo "=================================="
echo "编译成功！"
echo "=================================="
echo "可执行文件位置: build/bin/MealRecommendationSystem"
echo ""
echo "运行程序:"
echo "  cd build/bin"
echo "  ./MealRecommendationSystem"
echo ""
echo "或者直接运行:"
echo "  ./build/bin/MealRecommendationSystem"
echo "=================================="
