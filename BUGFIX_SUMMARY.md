# Bug修复总结

## 问题
- 程序在第二次运行时出现"vector subscript out of range"错误并崩溃
- 活动水平选择时可能出现闪退

## 修复
修改了`src/Database.cpp`文件中的`loadUsers()`函数（第96-129行），在访问用户数据的可选字段（preferredTags、avoidedTags、allergens）前添加了边界检查。

## 修改前
```cpp
if (tokens.size() >= 12) {
    // ...
    auto preferredTags = parseTagString(tokens[12]);  // 可能越界
    auto avoidedTags = parseTagString(tokens[13]);     // 可能越界
    auto allergens = parseTagString(tokens[14]);       // 可能越界
}
```

## 修改后
```cpp
if (tokens.size() >= 12) {
    // ...
    std::set<std::string> preferredTags;
    std::set<std::string> avoidedTags;
    std::set<std::string> allergens;
    
    if (tokens.size() > 12) {
        preferredTags = parseTagString(tokens[12]);
    }
    if (tokens.size() > 13) {
        avoidedTags = parseTagString(tokens[13]);
    }
    if (tokens.size() > 14) {
        allergens = parseTagString(tokens[14]);
    }
}
```

## 测试结果
✅ 第一次运行（注册）：正常
✅ 第二次运行（登录）：正常，无崩溃
✅ 多次运行（连续登录）：正常
✅ 所有活动水平选择（1-5）：正常
✅ 中文显示：正常

## 注意事项
- 修复过程中未修改任何编码相关代码，中文显示保持正常
- 修复使程序能够处理不同格式的用户数据文件（向后兼容）
