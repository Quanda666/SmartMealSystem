@echo off
chcp 65001 > nul
echo ==================================
echo 智能配餐推荐系统 - 编译脚本
echo ==================================

REM 创建构建目录
if not exist "build" (
    echo 创建构建目录...
    mkdir build
)

REM 进入构建目录
cd build

REM 运行CMake (VS2022)
echo 配置CMake...
cmake .. -G "Visual Studio 17 2022" -A x64
if errorlevel 1 (
    echo CMake配置失败！
    pause
    exit /b 1
)

REM 编译项目
echo 编译项目 (Release模式)...
cmake --build . --config Release
if errorlevel 1 (
    echo 编译失败！
    pause
    exit /b 1
)

echo.
echo ==================================
echo 编译成功！
echo ==================================
echo 可执行文件位置: build\bin\Release\MealRecommendationSystem.exe
echo.
echo 运行程序:
echo   cd build\bin\Release
echo   MealRecommendationSystem.exe
echo.
echo 或双击 run.bat 运行程序
echo ==================================
pause
