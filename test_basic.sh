#!/bin/bash

echo "==================================="
echo "基础功能测试"
echo "==================================="

# Check if executable exists
if [ ! -f "build/bin/MealRecommendationSystem" ]; then
    echo "❌ 可执行文件不存在，请先运行 ./build.sh"
    exit 1
fi

echo "✅ 可执行文件存在"

# Check if data directory exists
if [ ! -d "data" ]; then
    echo "❌ data 目录不存在"
    exit 1
fi

echo "✅ data 目录存在"

# Run the program with exit command
echo "0" | ./build/bin/MealRecommendationSystem > /tmp/test_output.txt 2>&1
EXIT_CODE=$?

if [ $EXIT_CODE -eq 0 ]; then
    echo "✅ 程序正常启动和退出"
else
    echo "❌ 程序异常退出，退出码: $EXIT_CODE"
    cat /tmp/test_output.txt
    exit 1
fi

# Check if sample data was initialized
if [ -f "data/foods.txt" ]; then
    FOOD_COUNT=$(wc -l < data/foods.txt)
    echo "✅ 食物数据初始化成功 ($FOOD_COUNT 种食物)"
else
    echo "❌ 食物数据未初始化"
    exit 1
fi

echo ""
echo "==================================="
echo "所有基础测试通过！"
echo "==================================="
echo ""
echo "系统已准备就绪，可以使用："
echo "  ./build/bin/MealRecommendationSystem"
echo ""
echo "或查看用户指南："
echo "  cat USER_GUIDE.md"
