# 修复说明

## 问题1：std::length_error 运行时异常

### 问题描述
在用户注册流程中，输入活动水平选择时发生 `std::length_error` 异常，导致程序崩溃。

### 根本原因
当用户输入的活动水平选择值不在1-5范围内时（例如输入0、负数或大于5的数字），代码会尝试访问数组越界，导致未定义行为。

### 修复措施
1. **添加输入验证循环**：在 `main.cpp` 的 `registerUser()` 和 `showUserProfile()` 函数中，为性别选择和活动水平选择添加了输入验证循环，确保输入值在有效范围内。

2. **改进输入函数**：
   - 在 `Utils.cpp` 的 `getIntInput()` 函数中添加了范围检查（-1000000 到 1000000）
   - 在 `Utils.cpp` 的 `getDoubleInput()` 函数中添加了范围检查（0 到 1000000）
   - 优化了 `getStringInput()` 函数的逻辑

## 问题2：Database.cpp 编码错误

### 问题描述
Database.cpp 文件在 Windows MSVC 编译器下出现大量编码相关错误：
- C4819: 文件包含不能在当前代码页(936)中表示的字符
- C3872: 字符不允许在标识符中使用
- C2001: 常量中有换行符
- C3688: 无效的文本后缀

### 根本原因
346-381行的 `initializeSampleData()` 函数中包含大量中文字符串，由于缺少UTF-8编码前缀，导致MSVC编译器无法正确解析。

### 修复措施
1. **添加 u8 前缀**：为所有中文字符串字面量添加 `u8` 前缀，确保它们被识别为UTF-8编码字符串。

2. **更新 CMakeLists.txt**：
   - 为 MSVC 添加 `/utf-8` 编译选项
   - 添加 `/wd4819` 禁用C4819警告
   - 为 GCC/Clang 添加 `-finput-charset=UTF-8` 和 `-fexec-charset=UTF-8` 选项
   - 添加 `_CRT_SECURE_NO_WARNINGS` 定义

## 编译建议

### Windows (MSVC)
```cmd
# 清理构建目录
rmdir /s /q build
mkdir build
cd build

# 生成项目
cmake .. -G "Visual Studio 17 2022" -A x64

# 编译
cmake --build . --config Release

# 运行
.\bin\Release\MealRecommendationSystem.exe
```

### Linux/macOS
```bash
# 清理构建目录
rm -rf build
mkdir build
cd build

# 生成项目和编译
cmake ..
make

# 运行
./bin/MealRecommendationSystem
```

## 测试建议

1. 测试注册流程时，尝试输入无效的选择值（如0、-1、6等），确认程序能够正确处理并要求重新输入。

2. 测试输入异常大的数值（如体重输入100000），确认程序会拒绝并要求重新输入。

3. 确认所有中文字符能够正确显示。

## 后续改进建议

1. 考虑为每个输入字段添加更具体的范围验证（例如年龄1-150，体重20-500，身高50-250等）。

2. 考虑添加异常捕获机制，在发生异常时提供更友好的错误信息。

3. 考虑使用更现代的输入验证库或框架。
