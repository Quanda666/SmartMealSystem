@echo off
chcp 65001 > nul

REM 检查是否已编译
if not exist "build\bin\Release\MealRecommendationSystem.exe" (
    if not exist "build\bin\Debug\MealRecommendationSystem.exe" (
        echo 程序尚未编译，请先运行 build.bat
        pause
        exit /b 1
    )
    set EXEC_PATH=build\bin\Debug\MealRecommendationSystem.exe
) else (
    set EXEC_PATH=build\bin\Release\MealRecommendationSystem.exe
)

REM 运行程序
echo 启动智能配餐推荐系统...
echo.
%EXEC_PATH%

pause
