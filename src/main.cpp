#include "../include/User.h"
#include "../include/Food.h"
#include "../include/Meal.h"
#include "../include/Database.h"
#include "../include/RecommendationEngine.h"
#include "../include/Utils.h"
#include <iostream>
#include <string>
#include <limits>
#include <iomanip>
#include <map>

class MealRecommendationSystem {
private:
    Database db;
    RecommendationEngine engine;
    User* currentUser;

public:
    MealRecommendationSystem() : currentUser(nullptr) {
        db = Database("data/users.txt", "data/foods.txt", "data/meals.txt");
    }

    void run() {
        if (!db.loadFoods()) {
            std::cout << "首次运行，初始化数据..." << std::endl;
            db.initializeSampleData();
        }
        db.loadUsers();
        db.loadMeals();
        
        engine.setFoodDatabase(db.getAllFoods());
        
        showWelcome();
        
        while (true) {
            if (currentUser == nullptr) {
                if (!loginMenu()) {
                    break;
                }
            } else {
                mainMenu();
            }
        }
        
        std::cout << "\n感谢使用智能配餐推荐系统！再见！" << std::endl;
    }

private:
    void showWelcome() {
        Utils::clearScreen();
        std::cout << "╔══════════════════════════════════════════════════════╗" << std::endl;
        std::cout << "║                                                        ║" << std::endl;
        std::cout << "║          智能配餐推荐系统 v1.0                         ║" << std::endl;
        std::cout << "║                                                        ║" << std::endl;
        std::cout << "║     Intelligent Meal Recommendation System             ║" << std::endl;
        std::cout << "║                                                        ║" << std::endl;
        std::cout << "╚══════════════════════════════════════════════════════╝" << std::endl;
        std::cout << std::endl;
    }

    bool loginMenu() {
        std::cout << "\n╔══════════════════════════════════════╗" << std::endl;
        std::cout << "║         欢迎使用                       ║" << std::endl;
        std::cout << "╠══════════════════════════════════════╣" << std::endl;
        std::cout << "║  1. 用户登录                          ║" << std::endl;
        std::cout << "║  2. 用户注册                          ║" << std::endl;
        std::cout << "║  0. 退出系统                          ║" << std::endl;
        std::cout << "╚══════════════════════════════════════╝" << std::endl;
        
        int choice = Utils::getIntInput("\n请选择 (0-2): ");
        
        switch (choice) {
            case 1:
                login();
                break;
            case 2:
                registerUser();
                break;
            case 0:
                return false;
            default:
                std::cout << "无效选择！" << std::endl;
                Utils::pause();
        }
        
        return true;
    }

    void login() {
        Utils::clearScreen();
        std::cout << "\n═══════ 用户登录 ═══════" << std::endl;
        
        std::string username = Utils::getInput("用户名: ");
        std::string password = Utils::getInput("密码: ");
        
        User* user = db.findUserByUsername(username);
        if (user != nullptr && user->getPassword() == password) {
            currentUser = user;
            std::cout << "\n登录成功！欢迎, " << username << "!" << std::endl;
            Utils::pause();
        } else {
            std::cout << "\n用户名或密码错误！" << std::endl;
            Utils::pause();
        }
    }

    void registerUser() {
        Utils::clearScreen();
        std::cout << "\n═══════ 用户注册 ═══════" << std::endl;
        
        std::string username = Utils::getInput("请输入用户名: ");
        
        if (db.findUserByUsername(username) != nullptr) {
            std::cout << "\n用户名已存在！" << std::endl;
            Utils::pause();
            return;
        }
        
        std::string password = Utils::getInput("请输入密码: ");
        
        User newUser(db.getNextUserId(), username, password);
        
        newUser.setAge(Utils::getIntInput("请输入年龄: "));
        newUser.setWeight(Utils::getDoubleInput("请输入体重 (kg): "));
        newUser.setHeight(Utils::getDoubleInput("请输入身高 (cm): "));
        
        std::cout << "\n性别 (1.男 2.女): ";
        int genderChoice = Utils::getIntInput("");
        newUser.setGender(genderChoice == 1 ? "male" : "female");
        
        std::cout << "\n活动水平:" << std::endl;
        std::cout << "1. 久坐 (sedentary)" << std::endl;
        std::cout << "2. 轻度活动 (light)" << std::endl;
        std::cout << "3. 中度活动 (moderate)" << std::endl;
        std::cout << "4. 重度活动 (active)" << std::endl;
        std::cout << "5. 非常活跃 (very_active)" << std::endl;
        int activityChoice = Utils::getIntInput("请选择 (1-5): ");
        
        std::string activityLevel;
        switch (activityChoice) {
            case 1: activityLevel = "sedentary"; break;
            case 2: activityLevel = "light"; break;
            case 3: activityLevel = "moderate"; break;
            case 4: activityLevel = "active"; break;
            case 5: activityLevel = "very_active"; break;
            default: activityLevel = "moderate";
        }
        newUser.setActivityLevel(activityLevel);
        
        newUser.calculateNutritionGoals();
        
        if (db.addUser(newUser)) {
            std::cout << "\n注册成功！" << std::endl;
            currentUser = db.findUserByUsername(username);
        } else {
            std::cout << "\n注册失败！" << std::endl;
        }
        
        Utils::pause();
    }

    void mainMenu() {
        Utils::clearScreen();
        
        std::cout << "\n╔══════════════════════════════════════════════════╗" << std::endl;
        std::cout << "║          智能配餐推荐系统 - 主菜单                 ║" << std::endl;
        std::cout << "╠══════════════════════════════════════════════════╣" << std::endl;
        std::cout << "║  当前用户: " << std::left << std::setw(37) 
                  << currentUser->getUsername() << "║" << std::endl;
        std::cout << "╠══════════════════════════════════════════════════╣" << std::endl;
        std::cout << "║  1. 获取每日配餐推荐                              ║" << std::endl;
        std::cout << "║  2. 查看历史记录                                  ║" << std::endl;
        std::cout << "║  3. 手动调整餐单                                  ║" << std::endl;
        std::cout << "║  4. 食物数据库查询                                ║" << std::endl;
        std::cout << "║  5. 营养成分可视化                                ║" << std::endl;
        std::cout << "║  6. 个人资料管理                                  ║" << std::endl;
        std::cout << "║  7. 口味偏好设置                                  ║" << std::endl;
        std::cout << "║  0. 退出登录                                      ║" << std::endl;
        std::cout << "╚══════════════════════════════════════════════════╝" << std::endl;
        
        int choice = Utils::getIntInput("\n请选择 (0-7): ");
        
        switch (choice) {
            case 1:
                getDailyRecommendation();
                break;
            case 2:
                viewHistory();
                break;
            case 3:
                adjustMeal();
                break;
            case 4:
                browseFoodDatabase();
                break;
            case 5:
                visualizeNutrition();
                break;
            case 6:
                manageProfile();
                break;
            case 7:
                managePreferences();
                break;
            case 0:
                currentUser = nullptr;
                std::cout << "\n已退出登录。" << std::endl;
                Utils::pause();
                break;
            default:
                std::cout << "无效选择！" << std::endl;
                Utils::pause();
        }
    }

    void getDailyRecommendation() {
        Utils::clearScreen();
        std::cout << "\n═══════ 每日配餐推荐 ═══════" << std::endl;
        
        std::string date = Utils::getCurrentDate();
        std::cout << "\n推荐日期: " << date << std::endl;
        
        std::cout << "\n正在生成推荐餐单..." << std::endl;
        
        std::vector<Meal> recommendedMeals = engine.recommendDailyMeals(*currentUser, date);
        
        std::cout << "\n╔══════════════════════════════════════════════════╗" << std::endl;
        std::cout << "║             今日推荐餐单                           ║" << std::endl;
        std::cout << "╚══════════════════════════════════════════════════╝" << std::endl;
        
        double totalCalories = 0, totalProtein = 0, totalCarbs = 0, totalFat = 0;
        
        for (auto& meal : recommendedMeals) {
            meal.setId(db.getNextMealId());
            meal.setUserId(currentUser->getId());
            meal.displayMeal();
            
            totalCalories += meal.getTotalCalories();
            totalProtein += meal.getTotalProtein();
            totalCarbs += meal.getTotalCarbs();
            totalFat += meal.getTotalFat();
        }
        
        std::cout << "\n╔══════════════════════════════════════════════════╗" << std::endl;
        std::cout << "║              全天营养总计                          ║" << std::endl;
        std::cout << "╠══════════════════════════════════════════════════╣" << std::endl;
        std::cout << "║  总热量:     " << (int)totalCalories << " / " 
                  << (int)currentUser->getDailyCalorieGoal() << " kcal" << std::endl;
        std::cout << "║  总蛋白质:   " << (int)totalProtein << " / " 
                  << (int)currentUser->getDailyProteinGoal() << " g" << std::endl;
        std::cout << "║  总碳水化合物: " << (int)totalCarbs << " / " 
                  << (int)currentUser->getDailyCarbGoal() << " g" << std::endl;
        std::cout << "║  总脂肪:     " << (int)totalFat << " / " 
                  << (int)currentUser->getDailyFatGoal() << " g" << std::endl;
        std::cout << "╚══════════════════════════════════════════════════╝" << std::endl;
        
        std::cout << "\n是否保存此推荐餐单？(y/n): ";
        std::string save = Utils::getInput("");
        
        if (save == "y" || save == "Y") {
            for (const auto& meal : recommendedMeals) {
                db.addMeal(meal);
                engine.addToHistory(currentUser->getId(), meal);
            }
            std::cout << "已保存餐单！" << std::endl;
        }
        
        Utils::pause();
    }

    void viewHistory() {
        Utils::clearScreen();
        std::cout << "\n═══════ 历史记录 ═══════" << std::endl;
        
        auto meals = db.getUserMeals(currentUser->getId());
        
        if (meals.empty()) {
            std::cout << "\n暂无历史记录。" << std::endl;
        } else {
            std::cout << "\n共有 " << meals.size() << " 条记录。" << std::endl;
            
            std::map<std::string, std::vector<Meal>> mealsByDate;
            for (const auto& meal : meals) {
                mealsByDate[meal.getDate()].push_back(meal);
            }
            
            for (const auto& pair : mealsByDate) {
                std::cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;
                std::cout << "日期: " << pair.first << std::endl;
                std::cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << std::endl;
                
                for (const auto& meal : pair.second) {
                    meal.displayMeal();
                }
            }
            
            engine.loadHistory({{currentUser->getId(), meals}});
            engine.displayRecommendationStats(*currentUser);
        }
        
        Utils::pause();
    }

    void adjustMeal() {
        Utils::clearScreen();
        std::cout << "\n═══════ 手动调整餐单 ═══════" << std::endl;
        
        std::cout << "\n1. 创建新餐单" << std::endl;
        std::cout << "2. 修改已有餐单" << std::endl;
        int choice = Utils::getIntInput("\n请选择: ");
        
        if (choice == 1) {
            createCustomMeal();
        } else if (choice == 2) {
            std::cout << "\n功能开发中..." << std::endl;
            Utils::pause();
        }
    }

    void createCustomMeal() {
        Utils::clearScreen();
        std::cout << "\n═══════ 创建自定义餐单 ═══════" << std::endl;
        
        std::string date = Utils::getInput("\n日期 (YYYY-MM-DD, 回车使用今天): ");
        if (date.empty()) {
            date = Utils::getCurrentDate();
        }
        
        std::cout << "\n餐次类型:" << std::endl;
        std::cout << "1. 早餐 (breakfast)" << std::endl;
        std::cout << "2. 午餐 (lunch)" << std::endl;
        std::cout << "3. 晚餐 (dinner)" << std::endl;
        std::cout << "4. 加餐 (snack)" << std::endl;
        int mealTypeChoice = Utils::getIntInput("请选择: ");
        
        std::string mealType;
        switch (mealTypeChoice) {
            case 1: mealType = "breakfast"; break;
            case 2: mealType = "lunch"; break;
            case 3: mealType = "dinner"; break;
            case 4: mealType = "snack"; break;
            default: mealType = "lunch";
        }
        
        Meal customMeal(db.getNextMealId(), currentUser->getId(), date, mealType);
        
        while (true) {
            std::cout << "\n当前餐单:" << std::endl;
            customMeal.displayMeal();
            
            std::cout << "\n1. 添加食物" << std::endl;
            std::cout << "2. 删除食物" << std::endl;
            std::cout << "3. 保存并退出" << std::endl;
            std::cout << "0. 放弃并返回" << std::endl;
            
            int action = Utils::getIntInput("\n请选择: ");
            
            if (action == 1) {
                std::string keyword = Utils::getInput("搜索食物名称: ");
                auto results = db.searchFoodsByName(keyword);
                
                if (results.empty()) {
                    std::cout << "未找到相关食物。" << std::endl;
                } else {
                    std::cout << "\n搜索结果:" << std::endl;
                    for (size_t i = 0; i < results.size(); ++i) {
                        std::cout << (i+1) << ". " << results[i].getName() 
                                  << " - " << results[i].getCalories() << " kcal" << std::endl;
                    }
                    
                    int foodChoice = Utils::getIntInput("\n选择食物编号 (0取消): ");
                    if (foodChoice > 0 && foodChoice <= (int)results.size()) {
                        customMeal.addFood(results[foodChoice - 1]);
                        std::cout << "已添加！" << std::endl;
                    }
                }
            } else if (action == 2) {
                int foodId = Utils::getIntInput("输入要删除的食物ID: ");
                customMeal.removeFood(foodId);
                std::cout << "已删除！" << std::endl;
            } else if (action == 3) {
                db.addMeal(customMeal);
                engine.addToHistory(currentUser->getId(), customMeal);
                std::cout << "\n餐单已保存！" << std::endl;
                Utils::pause();
                break;
            } else if (action == 0) {
                break;
            }
        }
    }

    void browseFoodDatabase() {
        Utils::clearScreen();
        std::cout << "\n═══════ 食物数据库 ═══════" << std::endl;
        
        auto foods = db.getAllFoods();
        
        std::cout << "\n1. 浏览所有食物" << std::endl;
        std::cout << "2. 搜索食物" << std::endl;
        std::cout << "3. 按类别浏览" << std::endl;
        int choice = Utils::getIntInput("\n请选择: ");
        
        if (choice == 1) {
            std::cout << "\n共有 " << foods.size() << " 种食物:" << std::endl;
            for (const auto& food : foods) {
                std::cout << "\nID: " << food.getId() << " - " << food.getName() 
                          << " (" << food.getCategory() << ")" << std::endl;
                std::cout << "  热量: " << food.getCalories() << " kcal, "
                          << "蛋白质: " << food.getProtein() << "g, "
                          << "碳水: " << food.getCarbohydrates() << "g, "
                          << "脂肪: " << food.getFat() << "g" << std::endl;
            }
        } else if (choice == 2) {
            std::string keyword = Utils::getInput("\n搜索关键词: ");
            auto results = db.searchFoodsByName(keyword);
            
            if (results.empty()) {
                std::cout << "未找到相关食物。" << std::endl;
            } else {
                for (const auto& food : results) {
                    food.displayInfo();
                }
            }
        } else if (choice == 3) {
            std::map<std::string, std::vector<Food>> foodsByCategory;
            for (const auto& food : foods) {
                foodsByCategory[food.getCategory()].push_back(food);
            }
            
            std::cout << "\n食物类别:" << std::endl;
            int i = 1;
            std::vector<std::string> categories;
            for (const auto& pair : foodsByCategory) {
                std::cout << i++ << ". " << pair.first 
                          << " (" << pair.second.size() << " 种)" << std::endl;
                categories.push_back(pair.first);
            }
            
            int catChoice = Utils::getIntInput("\n选择类别: ");
            if (catChoice > 0 && catChoice <= (int)categories.size()) {
                std::string selectedCategory = categories[catChoice - 1];
                std::cout << "\n" << selectedCategory << ":" << std::endl;
                for (const auto& food : foodsByCategory[selectedCategory]) {
                    food.displayInfo();
                }
            }
        }
        
        Utils::pause();
    }

    void visualizeNutrition() {
        Utils::clearScreen();
        std::cout << "\n═══════ 营养成分可视化 ═══════" << std::endl;
        
        std::cout << "\n1. 查看今日营养摄入" << std::endl;
        std::cout << "2. 查看指定日期营养摄入" << std::endl;
        std::cout << "3. 查看营养目标达成情况" << std::endl;
        int choice = Utils::getIntInput("\n请选择: ");
        
        std::string date;
        if (choice == 1) {
            date = Utils::getCurrentDate();
        } else if (choice == 2) {
            date = Utils::getInput("请输入日期 (YYYY-MM-DD): ");
        } else if (choice == 3) {
            displayNutritionGoals();
            Utils::pause();
            return;
        }
        
        auto meals = db.getUserMealsByDate(currentUser->getId(), date);
        
        if (meals.empty()) {
            std::cout << "\n该日期暂无记录。" << std::endl;
        } else {
            double totalCalories = 0, totalProtein = 0, totalCarbs = 0, totalFat = 0;
            
            for (const auto& meal : meals) {
                totalCalories += meal.getTotalCalories();
                totalProtein += meal.getTotalProtein();
                totalCarbs += meal.getTotalCarbs();
                totalFat += meal.getTotalFat();
            }
            
            std::cout << "\n日期: " << date << std::endl;
            Utils::displayNutritionChart(totalCalories, totalProtein, totalCarbs, totalFat);
            
            std::cout << "\n与目标对比:" << std::endl;
            Utils::displayProgressBar(totalCalories, currentUser->getDailyCalorieGoal(), "热量");
            Utils::displayProgressBar(totalProtein, currentUser->getDailyProteinGoal(), "蛋白质");
            Utils::displayProgressBar(totalCarbs, currentUser->getDailyCarbGoal(), "碳水化合物");
            Utils::displayProgressBar(totalFat, currentUser->getDailyFatGoal(), "脂肪");
        }
        
        Utils::pause();
    }

    void displayNutritionGoals() {
        std::cout << "\n═══════ 每日营养目标 ═══════" << std::endl;
        
        Utils::displayProgressBar(0, currentUser->getDailyCalorieGoal(), "热量目标");
        Utils::displayProgressBar(0, currentUser->getDailyProteinGoal(), "蛋白质目标");
        Utils::displayProgressBar(0, currentUser->getDailyCarbGoal(), "碳水目标");
        Utils::displayProgressBar(0, currentUser->getDailyFatGoal(), "脂肪目标");
    }

    void manageProfile() {
        Utils::clearScreen();
        std::cout << "\n═══════ 个人资料管理 ═══════" << std::endl;
        
        currentUser->displayProfile();
        
        std::cout << "\n1. 修改基本信息" << std::endl;
        std::cout << "2. 重新计算营养目标" << std::endl;
        std::cout << "0. 返回" << std::endl;
        
        int choice = Utils::getIntInput("\n请选择: ");
        
        if (choice == 1) {
            std::cout << "\n修改基本信息:" << std::endl;
            currentUser->setAge(Utils::getIntInput("年龄: "));
            currentUser->setWeight(Utils::getDoubleInput("体重 (kg): "));
            currentUser->setHeight(Utils::getDoubleInput("身高 (cm): "));
            
            currentUser->calculateNutritionGoals();
            db.updateUser(*currentUser);
            
            std::cout << "\n信息已更新！" << std::endl;
            Utils::pause();
        } else if (choice == 2) {
            currentUser->calculateNutritionGoals();
            db.updateUser(*currentUser);
            std::cout << "\n营养目标已重新计算！" << std::endl;
            currentUser->displayProfile();
            Utils::pause();
        }
    }

    void managePreferences() {
        Utils::clearScreen();
        std::cout << "\n═══════ 口味偏好设置 ═══════" << std::endl;
        
        std::cout << "\n当前喜欢的口味: ";
        for (const auto& tag : currentUser->getPreferredTags()) {
            std::cout << tag << " ";
        }
        std::cout << std::endl;
        
        std::cout << "当前避免的口味: ";
        for (const auto& tag : currentUser->getAvoidedTags()) {
            std::cout << tag << " ";
        }
        std::cout << std::endl;
        
        std::cout << "\n1. 添加喜欢的口味" << std::endl;
        std::cout << "2. 添加避免的口味" << std::endl;
        std::cout << "3. 设置过敏源" << std::endl;
        std::cout << "0. 返回" << std::endl;
        
        int choice = Utils::getIntInput("\n请选择: ");
        
        if (choice == 1) {
            std::cout << "\n常见口味标签: 清淡, 甜, 咸, 酸, 辣, 鲜, 香" << std::endl;
            std::string tag = Utils::getInput("输入口味标签: ");
            currentUser->addPreferredTag(tag);
            db.updateUser(*currentUser);
            std::cout << "已添加！" << std::endl;
            Utils::pause();
        } else if (choice == 2) {
            std::cout << "\n常见口味标签: 清淡, 甜, 咸, 酸, 辣, 鲜, 香" << std::endl;
            std::string tag = Utils::getInput("输入口味标签: ");
            currentUser->addAvoidedTag(tag);
            db.updateUser(*currentUser);
            std::cout << "已添加！" << std::endl;
            Utils::pause();
        } else if (choice == 3) {
            std::string allergen = Utils::getInput("输入过敏源: ");
            currentUser->addAllergen(allergen);
            db.updateUser(*currentUser);
            std::cout << "已添加！" << std::endl;
            Utils::pause();
        }
    }
};

int main() {
    MealRecommendationSystem system;
    system.run();
    return 0;
}
