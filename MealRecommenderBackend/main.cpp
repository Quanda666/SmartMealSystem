// main.cpp
#include "MealSystem.h"
#include <iostream>
using namespace std;

// 显示欢迎信息
void showWelcome() {
    system("cls");
    cout << "==============================================" << endl;
    cout << "      欢迎使用智能配餐推荐系统" << endl;
    cout << "  Intelligent Meal Recommendation System" << endl;
    cout << "==============================================" << endl;
    cout << endl;
    cout << "系统功能：" << endl;
    cout << "* 用户注册与管理" << endl;
    cout << "* 个性化偏好设置" << endl;
    cout << "* 智能餐单推荐" << endl;
    cout << "* 营养计算与分析" << endl;
    cout << "* 历史记录追踪" << endl;
    cout << "* 食物数据库管理" << endl;
    cout << endl;
    cout << "按Enter键开始..." << endl;
    cin.get();
}

// 保存系统数据
void saveSystemData(FoodDatabase& foodDB, UserManager& userManager) {
    foodDB.saveToFile();
    userManager.saveUsers();
    cout << "系统数据已保存！" << endl;
}

// 显示系统信息
void showSystemInfo() {
    cout << endl;
    cout << "==============================================" << endl;
    cout << "系统版本: 1.0.0" << endl;
    cout << "开发者: 智能配餐系统团队" << endl;
    cout << "技术栈: C++ 17, STL, 随机算法" << endl;
    cout << "功能: 个性化餐单推荐系统" << endl;
    cout << "==============================================" << endl;
}

// 主函数
int main() {
    // 设置控制台编码（Windows）
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    try {
        showWelcome();

        // 创建系统对象
        MealSystemUI systemUI;

        // 运行主界面
        systemUI.showMainMenu();

        cout << "\n感谢使用智能配餐推荐系统！" << endl;
        cout << "再见！" << endl;

    }
    catch (const exception& e) {
        cerr << "系统错误: " << e.what() << endl;
        cout << "按Enter键退出..." << endl;
        cin.get();
        return 1;
    }

    return 0;
}