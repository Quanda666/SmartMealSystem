// MealSystem.h
#pragma once

#ifndef MEAL_SYSTEM_H
#define MEAL_SYSTEM_H

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <ctime>
#include <random>
#include <memory>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

// ==================== 营养信息结构体 ====================
struct NutritionInfo {
    double calories;      // 卡路里
    double protein;       // 蛋白质 (g)
    double carbs;         // 碳水化合物 (g)
    double fat;           // 脂肪 (g)
    double fiber;         // 膳食纤维 (g)

    NutritionInfo() : calories(0), protein(0), carbs(0), fat(0), fiber(0) {}
    NutritionInfo(double cal, double pro, double car, double f, double fib)
        : calories(cal), protein(pro), carbs(car), fat(f), fiber(fib) {}

    // 显示营养信息
    void display() const {
        cout << fixed << setprecision(2);
        cout << " 热量: " << calories << " kcal" << endl;
        cout << " 蛋白质: " << protein << " g" << endl;
        cout << " 碳水化合物: " << carbs << " g" << endl;
        cout << " 脂肪: " << fat << " g" << endl;
        cout << " 膳食纤维: " << fiber << " g" << endl;
    }

    // 运算符重载
    NutritionInfo operator+(const NutritionInfo& other) const {
        return NutritionInfo(
            calories + other.calories,
            protein + other.protein,
            carbs + other.carbs,
            fat + other.fat,
            fiber + other.fiber
        );
    }

    void operator+=(const NutritionInfo& other) {
        calories += other.calories;
        protein += other.protein;
        carbs += other.carbs;
        fat += other.fat;
        fiber += other.fiber;
    }
};

// ==================== 食物类 ====================
class FoodItem {
private:
    int id;
    string name;
    string category;          // 类别：主食、蔬菜、肉类等
    NutritionInfo nutrition;
    vector<string> tasteTags; // 口味标签：甜、咸、辣、酸等
    double servingSize;       // 每份大小(g)

public:
    FoodItem(int id = 0, const string& name = "", const string& category = "",
        const NutritionInfo& nutr = NutritionInfo(), double serving = 100.0)
        : id(id), name(name), category(category), nutrition(nutr), servingSize(serving) {}

    // Getter方法
    int getId() const { return id; }
    string getName() const { return name; }
    string getCategory() const { return category; }
    NutritionInfo getNutrition() const { return nutrition; }
    double getServingSize() const { return servingSize; }
    vector<string> getTasteTags() const { return tasteTags; }

    // Setter方法
    void setId(int newId) { id = newId; }
    void setName(const string& newName) { name = newName; }
    void setCategory(const string& newCategory) { category = newCategory; }
    void setNutrition(const NutritionInfo& newNutrition) { nutrition = newNutrition; }
    void setServingSize(double size) { servingSize = size; }

    // 添加口味标签
    void addTasteTag(const string& tag) {
        if (find(tasteTags.begin(), tasteTags.end(), tag) == tasteTags.end()) {
            tasteTags.push_back(tag);
        }
    }

    // 移除口味标签
    void removeTasteTag(const string& tag) {
        tasteTags.erase(remove(tasteTags.begin(), tasteTags.end(), tag), tasteTags.end());
    }

    // 显示食物信息
    void display() const {
        cout << "=================================" << endl;
        cout << " 食物ID: " << id << endl;
        cout << " 名称: " << name << endl;
        cout << " 类别: " << category << endl;
        cout << " 份量: " << servingSize << "g" << endl;
        cout << " 营养成分:" << endl;
        nutrition.display();
        if (!tasteTags.empty()) {
            cout << " 口味标签: ";
            for (const auto& tag : tasteTags) {
                cout << tag << " ";
            }
            cout << endl;
        }
        cout << "=================================" << endl;
    }

    // 保存为字符串
    string toString() const {
        ostringstream oss;
        oss << id << "," << name << "," << category << ","
            << nutrition.calories << "," << nutrition.protein << ","
            << nutrition.carbs << "," << nutrition.fat << ","
            << nutrition.fiber << "," << servingSize;
        for (const auto& tag : tasteTags) {
            oss << "," << tag;
        }
        return oss.str();
    }

    // 从字符串加载
    static FoodItem fromString(const string& str) {
        istringstream iss(str);
        string token;
        vector<string> tokens;

        while (getline(iss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 9) {
            return FoodItem(); // 返回默认对象
        }

        FoodItem food;
        food.id = stoi(tokens[0]);
        food.name = tokens[1];
        food.category = tokens[2];
        food.nutrition = NutritionInfo(
            stod(tokens[3]),  // calories
            stod(tokens[4]),  // protein
            stod(tokens[5]),  // carbs
            stod(tokens[6]),  // fat
            stod(tokens[7])   // fiber
        );
        food.servingSize = stod(tokens[8]);

        // 加载口味标签（如果存在）
        for (size_t i = 9; i < tokens.size(); i++) {
            food.tasteTags.push_back(tokens[i]);
        }

        return food;
    }
};

// ==================== 用户偏好设置 ====================
class UserPreferences {
private:
    map<string, double> tasteWeights;  // 口味权重
    vector<string> dislikedFoods;      // 不喜欢的食物
    vector<string> allergies;          // 过敏食物
    bool isVegetarian;                 // 是否素食
    bool isHalal;                      // 是否清真
    double spicyTolerance;             // 辣度耐受度 0-1

public:
    UserPreferences() : isVegetarian(false), isHalal(false), spicyTolerance(0.5) {
        // 默认口味权重
        tasteWeights["sweet"] = 0.5;   // 甜
        tasteWeights["salty"] = 0.7;   // 咸
        tasteWeights["spicy"] = 0.5;   // 辣
        tasteWeights["sour"] = 0.4;    // 酸
        tasteWeights["umami"] = 0.6;   // 鲜
    }

    // Getter方法
    double getTasteWeight(const string& taste) const {
        auto it = tasteWeights.find(taste);
        return it != tasteWeights.end() ? it->second : 0.5;
    }

    bool getIsVegetarian() const { return isVegetarian; }
    bool getIsHalal() const { return isHalal; }
    double getSpicyTolerance() const { return spicyTolerance; }
    const vector<string>& getDislikedFoods() const { return dislikedFoods; }
    const vector<string>& getAllergies() const { return allergies; }

    // Setter方法
    void setTasteWeight(const string& taste, double weight) {
        if (weight >= 0 && weight <= 1) {
            tasteWeights[taste] = weight;
        }
    }

    void setIsVegetarian(bool vegetarian) { isVegetarian = vegetarian; }
    void setIsHalal(bool halal) { isHalal = halal; }
    void setSpicyTolerance(double tolerance) {
        if (tolerance >= 0 && tolerance <= 1) {
            spicyTolerance = tolerance;
        }
    }

    void addDislikedFood(const string& food) {
        if (find(dislikedFoods.begin(), dislikedFoods.end(), food) == dislikedFoods.end()) {
            dislikedFoods.push_back(food);
        }
    }

    void addAllergy(const string& allergy) {
        if (find(allergies.begin(), allergies.end(), allergy) == allergies.end()) {
            allergies.push_back(allergy);
        }
    }

    // 检查是否喜欢某食物
    bool isFoodLiked(const FoodItem& food) const {
        // 检查是否是不喜欢的食物
        for (const auto& disliked : dislikedFoods) {
            if (food.getName().find(disliked) != string::npos) {
                return false;
            }
        }

        // 检查过敏
        for (const auto& allergy : allergies) {
            if (food.getName().find(allergy) != string::npos) {
                return false;
            }
        }

        // 检查饮食限制
        if (isVegetarian) {
            vector<string> nonVegCategories = { "meat", "seafood", "poultry" };
            for (const auto& category : nonVegCategories) {
                if (food.getCategory() == category) {
                    return false;
                }
            }
        }

        return true;
    }

    // 计算食物偏好得分
    double calculatePreferenceScore(const FoodItem& food) const {
        if (!isFoodLiked(food)) {
            return 0.0;
        }

        double score = 1.0;
        const auto& tags = food.getTasteTags();

        for (const auto& tag : tags) {
            auto it = tasteWeights.find(tag);
            if (it != tasteWeights.end()) {
                score += it->second;
            }
        }

        return score;
    }

    // 显示偏好设置
    void display() const {
        cout << "=== 用户偏好设置 ===" << endl;
        cout << " 素食: " << (isVegetarian ? "是" : "否") << endl;
        cout << " 清真: " << (isHalal ? "是" : "否") << endl;
        cout << " 辣度耐受: " << fixed << setprecision(2) << spicyTolerance << endl;

        cout << " 口味权重:" << endl;
        for (const auto& pair : tasteWeights) {
            cout << "  " << pair.first << ": " << pair.second << endl;
        }

        if (!dislikedFoods.empty()) {
            cout << " 不喜欢的食物:" << endl;
            for (const auto& food : dislikedFoods) {
                cout << "  " << food << endl;
            }
        }

        if (!allergies.empty()) {
            cout << " 过敏食物:" << endl;
            for (const auto& allergy : allergies) {
                cout << "  " << allergy << endl;
            }
        }
    }
};

// ==================== 用户类 ====================
class User {
private:
    int userId;
    string username;
    int age;
    string gender;              // "male" 或 "female"
    double weight;              // 体重(kg)
    double height;              // 身高(cm)
    string activityLevel;       // 活动水平
    double targetCalories;      // 目标卡路里
    UserPreferences preferences;
    vector<vector<FoodItem>> mealHistory;  // 餐单历史

public:
    User(int id = 0, const string& name = "", int age = 0, const string& gender = "male",
        double weight = 0, double height = 0, const string& activity = "moderate")
        : userId(id), username(name), age(age), gender(gender),
        weight(weight), height(height), activityLevel(activity) {
        calculateNutritionalNeeds();
    }

    // Getter方法
    int getUserId() const { return userId; }
    string getUsername() const { return username; }
    int getAge() const { return age; }
    string getGender() const { return gender; }
    double getWeight() const { return weight; }
    double getHeight() const { return height; }
    string getActivityLevel() const { return activityLevel; }
    double getTargetCalories() const { return targetCalories; }
    UserPreferences& getPreferences() { return preferences; }
    const UserPreferences& getPreferences() const { return preferences; }
    vector<vector<FoodItem>>& getMealHistory() { return mealHistory; }

    // Setter方法
    void setUserId(int id) { userId = id; }
    void setUsername(const string& name) { username = name; }
    void setAge(int newAge) { age = newAge; calculateNutritionalNeeds(); }
    void setGender(const string& newGender) { gender = newGender; calculateNutritionalNeeds(); }
    void setWeight(double newWeight) { weight = newWeight; calculateNutritionalNeeds(); }
    void setHeight(double newHeight) { height = newHeight; calculateNutritionalNeeds(); }
    void setActivityLevel(const string& activity) { activityLevel = activity; calculateNutritionalNeeds(); }

    // 计算营养需求
    void calculateNutritionalNeeds() {
        // 计算基础代谢率(BMR)
        double bmr;
        if (gender == "male") {
            bmr = 88.362 + (13.397 * weight) + (4.799 * height) - (5.677 * age);
        }
        else {
            bmr = 447.593 + (9.247 * weight) + (3.098 * height) - (4.330 * age);
        }

        // 活动系数
        double activityFactor = 1.2;  // 默认轻度活动
        if (activityLevel == "sedentary") activityFactor = 1.2;
        else if (activityLevel == "light") activityFactor = 1.375;
        else if (activityLevel == "moderate") activityFactor = 1.55;
        else if (activityLevel == "active") activityFactor = 1.725;
        else if (activityLevel == "very_active") activityFactor = 1.9;

        targetCalories = bmr * activityFactor;
    }

    // 添加餐单到历史
    void addMealToHistory(const vector<FoodItem>& meal) {
        mealHistory.push_back(meal);
    }

    // 显示用户信息
    void display() const {
        cout << "=== 用户信息 ===" << endl;
        cout << " 用户ID: " << userId << endl;
        cout << " 用户名: " << username << endl;
        cout << " 年龄: " << age << endl;
        cout << " 性别: " << gender << endl;
        cout << " 体重: " << weight << " kg" << endl;
        cout << " 身高: " << height << " cm" << endl;
        cout << " 活动水平: " << activityLevel << endl;
        cout << " 目标卡路里: " << fixed << setprecision(0) << targetCalories << " kcal" << endl;
        preferences.display();
    }

    // 保存用户数据
    string saveToString() const {
        ostringstream oss;
        oss << userId << "," << username << "," << age << "," << gender << ","
            << weight << "," << height << "," << activityLevel << ","
            << targetCalories;
        return oss.str();
    }

    // 加载用户数据
    static User loadFromString(const string& str) {
        istringstream iss(str);
        vector<string> tokens;
        string token;

        while (getline(iss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 8) return User();

        User user(
            stoi(tokens[0]),  // userId
            tokens[1],        // username
            stoi(tokens[2]),  // age
            tokens[3],        // gender
            stod(tokens[4]),  // weight
            stod(tokens[5]),  // height
            tokens[6]         // activityLevel
        );
        user.targetCalories = stod(tokens[7]);
        return user;
    }
};

// ==================== 食物数据库管理 ====================
class FoodDatabase {
private:
    vector<FoodItem> foods;
    string databaseFile;

public:
    FoodDatabase(const string& filename = "foods.txt") : databaseFile(filename) {
        loadFromFile();
        if (foods.empty()) {
            initializeDefaultFoods();
            saveToFile();
        }
    }

    // 添加食物
    void addFood(const FoodItem& food) {
        foods.push_back(food);
    }

    // 根据ID查找食物
    FoodItem* findFoodById(int id) {
        for (auto& food : foods) {
            if (food.getId() == id) {
                return &food;
            }
        }
        return nullptr;
    }

    // 根据名称搜索食物
    vector<FoodItem> searchFoods(const string& keyword) const {
        vector<FoodItem> results;
        string lowerKeyword = keyword;
        transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(), ::tolower);

        for (const auto& food : foods) {
            string foodName = food.getName();
            transform(foodName.begin(), foodName.end(), foodName.begin(), ::tolower);

            if (foodName.find(lowerKeyword) != string::npos) {
                results.push_back(food);
            }
        }
        return results;
    }

    // 根据类别获取食物
    vector<FoodItem> getFoodsByCategory(const string& category) const {
        vector<FoodItem> results;
        for (const auto& food : foods) {
            if (food.getCategory() == category) {
                results.push_back(food);
            }
        }
        return results;
    }

    // 获取所有食物
    const vector<FoodItem>& getAllFoods() const {
        return foods;
    }

    // 显示所有食物
    void displayAllFoods() const {
        cout << "=== 食物数据库 (" << foods.size() << " 种食物) ===" << endl;
        for (const auto& food : foods) {
            cout << " [" << food.getId() << "] " << food.getName()
                << " (" << food.getCategory() << ")" << endl;
        }
    }

    // 显示食物详情
    void displayFoodDetails(int id) const {
        for (const auto& food : foods) {
            if (food.getId() == id) {
                food.display();
                return;
            }
        }
        cout << "食物ID " << id << " 不存在！" << endl;
    }

    // 初始化默认食物数据
    void initializeDefaultFoods() {
        // 清空现有数据
        foods.clear();

        // 主食类
        foods.push_back(FoodItem(1, "米饭", "staple", NutritionInfo(130, 2.7, 28, 0.3, 0.4), 100));
        foods.back().addTasteTag("neutral");

        foods.push_back(FoodItem(2, "全麦面包", "staple", NutritionInfo(247, 13, 41, 3.4, 7), 100));
        foods.back().addTasteTag("neutral");

        foods.push_back(FoodItem(3, "燕麦片", "staple", NutritionInfo(389, 16.9, 66, 6.9, 10.6), 100));
        foods.back().addTasteTag("sweet");

        // 蛋白质类
        foods.push_back(FoodItem(4, "鸡胸肉", "meat", NutritionInfo(165, 31, 0, 3.6, 0), 100));
        foods.back().addTasteTag("salty");

        foods.push_back(FoodItem(5, "鸡蛋", "egg", NutritionInfo(155, 13, 1.1, 11, 0), 100));
        foods.back().addTasteTag("neutral");

        foods.push_back(FoodItem(6, "三文鱼", "seafood", NutritionInfo(208, 20, 0, 13, 0), 100));
        foods.back().addTasteTag("umami");

        foods.push_back(FoodItem(7, "豆腐", "bean", NutritionInfo(76, 8, 2, 4.8, 0.3), 100));
        foods.back().addTasteTag("neutral");

        // 蔬菜类
        foods.push_back(FoodItem(8, "西兰花", "vegetable", NutritionInfo(34, 2.8, 7, 0.4, 2.6), 100));
        foods.back().addTasteTag("umami");

        foods.push_back(FoodItem(9, "菠菜", "vegetable", NutritionInfo(23, 2.9, 3.6, 0.4, 2.2), 100));
        foods.back().addTasteTag("neutral");

        foods.push_back(FoodItem(10, "胡萝卜", "vegetable", NutritionInfo(41, 0.9, 10, 0.2, 2.8), 100));
        foods.back().addTasteTag("sweet");

        // 水果类
        foods.push_back(FoodItem(11, "苹果", "fruit", NutritionInfo(52, 0.3, 14, 0.2, 2.4), 100));
        foods.back().addTasteTag("sweet");
        foods.back().addTasteTag("sour");

        foods.push_back(FoodItem(12, "香蕉", "fruit", NutritionInfo(89, 1.1, 23, 0.3, 2.6), 100));
        foods.back().addTasteTag("sweet");

        // 奶制品
        foods.push_back(FoodItem(13, "牛奶", "dairy", NutritionInfo(61, 3.2, 4.8, 3.3, 0), 100));
        foods.back().addTasteTag("neutral");

        // 坚果类
        foods.push_back(FoodItem(14, "杏仁", "nut", NutritionInfo(579, 21, 22, 49, 12.5), 100));
        foods.back().addTasteTag("salty");
    }

    // 保存到文件
    bool saveToFile() const {
        ofstream file(databaseFile);
        if (!file.is_open()) {
            return false;
        }

        for (const auto& food : foods) {
            file << food.toString() << endl;
        }

        file.close();
        return true;
    }

    // 从文件加载
    bool loadFromFile() {
        ifstream file(databaseFile);
        if (!file.is_open()) {
            return false;
        }

        foods.clear();
        string line;

        while (getline(file, line)) {
            if (!line.empty()) {
                FoodItem food = FoodItem::fromString(line);
                if (food.getId() != 0) {
                    foods.push_back(food);
                }
            }
        }

        file.close();
        return true;
    }
};

// ==================== 智能推荐系统 ====================
class MealRecommender {
private:
    FoodDatabase& foodDB;
    mt19937 rng;

public:
    MealRecommender(FoodDatabase& db) : foodDB(db) {
        rng.seed(static_cast<unsigned int>(time(nullptr)));
    }

    // 推荐单餐
    vector<FoodItem> recommendMeal(User& user, const string& mealType, int numItems = 3) {
        vector<FoodItem> recommendedMeal;
        vector<FoodItem> allFoods = foodDB.getAllFoods();

        // 根据餐型确定食物类别组合
        vector<string> categories = getCategoriesForMealType(mealType);

        // 为每个类别选择食物
        for (const auto& category : categories) {
            if (recommendedMeal.size() >= numItems) break;

            vector<FoodItem> categoryFoods = foodDB.getFoodsByCategory(category);
            if (categoryFoods.empty()) continue;

            // 根据用户偏好排序
            vector<pair<FoodItem, double>> scoredFoods;
            for (const auto& food : categoryFoods) {
                double score = user.getPreferences().calculatePreferenceScore(food);
                if (score > 0) {
                    scoredFoods.push_back({ food, score });
                }
            }

            if (scoredFoods.empty()) continue;

            // 按得分排序
            sort(scoredFoods.begin(), scoredFoods.end(),
                [](const pair<FoodItem, double>& a, const pair<FoodItem, double>& b) {
                    return a.second > b.second;
                });

            // 选择得分最高的食物
            int selected = 0;
            if (scoredFoods.size() > 1) {
                uniform_int_distribution<int> dist(0, min(2, (int)scoredFoods.size() - 1));
                selected = dist(rng);
            }

            recommendedMeal.push_back(scoredFoods[selected].first);
        }

        return recommendedMeal;
    }

    // 推荐一日三餐
    map<string, vector<FoodItem>> recommendDailyMeals(User& user) {
        map<string, vector<FoodItem>> dailyMeals;

        vector<string> mealTypes = { "breakfast", "lunch", "dinner", "snack" };

        for (const auto& mealType : mealTypes) {
            dailyMeals[mealType] = recommendMeal(user, mealType);
        }

        return dailyMeals;
    }

    // 根据餐型确定食物类别
    vector<string> getCategoriesForMealType(const string& mealType) {
        if (mealType == "breakfast") {
            return { "staple", "egg", "dairy", "fruit" };
        }
        else if (mealType == "lunch") {
            return { "staple", "meat", "vegetable", "vegetable" };
        }
        else if (mealType == "dinner") {
            return { "staple", "seafood", "vegetable", "fruit" };
        }
        else if (mealType == "snack") {
            return { "fruit", "nut", "dairy" };
        }

        return { "staple", "vegetable", "meat" };
    }

    // 检查餐单营养均衡性
    bool checkMealBalance(const vector<FoodItem>& meal, double targetCalories, double tolerance = 0.3) {
        if (meal.empty()) return false;

        NutritionInfo total;
        for (const auto& food : meal) {
            total += food.getNutrition();
        }

        // 检查热量是否在目标范围内
        double calorieRatio = total.calories / targetCalories;
        return (calorieRatio >= (1 - tolerance) && calorieRatio <= (1 + tolerance));
    }
};

// ==================== 用户管理系统 ====================
class UserManager {
private:
    vector<User> users;
    string userFile;
    int nextUserId;

public:
    UserManager(const string& filename = "users.txt") : userFile(filename), nextUserId(1) {
        loadUsers();
    }

    // 注册新用户 - 修改返回类型为引用
    User& registerUser(const string& username, int age, const string& gender,
        double weight, double height, const string& activityLevel) {

        User newUser(nextUserId++, username, age, gender, weight, height, activityLevel);
        users.push_back(newUser);
        saveUsers();

        return users.back();
    }

    // 登录用户
    User* loginUser(int userId) {
        for (auto& user : users) {
            if (user.getUserId() == userId) {
                return &user;
            }
        }
        return nullptr;
    }

    // 获取所有用户
    const vector<User>& getAllUsers() const {
        return users;
    }

    // 保存用户数据 - 移除const修饰符
    bool saveUsers() {  // 移除const
        ofstream file(userFile);
        if (!file.is_open()) {
            return false;
        }

        for (const auto& user : users) {
            file << user.saveToString() << endl;
        }

        file.close();
        return true;
    }

    // 加载用户数据
    bool loadUsers() {
        ifstream file(userFile);
        if (!file.is_open()) {
            return false;
        }

        users.clear();
        string line;
        nextUserId = 1;

        while (getline(file, line)) {
            if (!line.empty()) {
                User user = User::loadFromString(line);
                if (user.getUserId() != 0) {
                    users.push_back(user);
                    nextUserId = max(nextUserId, user.getUserId() + 1);
                }
            }
        }

        file.close();
        return true;
    }
};

// ==================== 系统主界面类 ====================
class MealSystemUI {
private:
    FoodDatabase foodDB;
    UserManager userManager;
    MealRecommender recommender;
    User* currentUser;

public:
    MealSystemUI() : recommender(foodDB), currentUser(nullptr) {}

    // 显示主菜单
    void showMainMenu() {
        while (true) {
            system("cls");
            cout << "=================================" << endl;
            cout << "   智能配餐推荐系统" << endl;
            cout << "=================================" << endl;
            cout << "1. 用户注册" << endl;
            cout << "2. 用户登录" << endl;
            cout << "3. 设置偏好" << endl;
            cout << "4. 获取推荐" << endl;
            cout << "5. 查看历史" << endl;
            cout << "6. 食物数据库" << endl;
            cout << "7. 用户信息" << endl;
            cout << "8. 退出系统" << endl;
            cout << "=================================" << endl;
            cout << "当前用户: " << (currentUser ? currentUser->getUsername() : "未登录") << endl;
            cout << "请选择 (1-8): ";

            int choice;
            cin >> choice;
            cin.ignore();

            switch (choice) {
            case 1: registerUser(); break;
            case 2: loginUser(); break;
            case 3: setPreferences(); break;
            case 4: getRecommendation(); break;
            case 5: viewHistory(); break;
            case 6: browseFoodDatabase(); break;
            case 7: showUserInfo(); break;
            case 8: return;
            default: cout << "无效选择，请重新输入！" << endl;
            }

            cout << "\n按Enter键继续...";
            cin.get();
        }
    }

    // 用户注册
    void registerUser() {
        system("cls");
        cout << "=== 用户注册 ===" << endl;

        string username, gender, activityLevel;
        int age;
        double weight, height;

        cout << "请输入用户名: ";
        getline(cin, username);

        cout << "请输入年龄: ";
        cin >> age;

        cout << "请输入性别 (male/female): ";
        cin >> gender;

        cout << "请输入体重 (kg): ";
        cin >> weight;

        cout << "请输入身高 (cm): ";
        cin >> height;
        cin.ignore();

        cout << "请选择活动水平:" << endl;
        cout << "1. 久坐 (sedentary)" << endl;
        cout << "2. 轻度活动 (light)" << endl;
        cout << "3. 中度活动 (moderate)" << endl;
        cout << "4. 活跃 (active)" << endl;
        cout << "5. 非常活跃 (very_active)" << endl;
        cout << "选择 (1-5): ";

        int activityChoice;
        cin >> activityChoice;
        cin.ignore();

        switch (activityChoice) {
        case 1: activityLevel = "sedentary"; break;
        case 2: activityLevel = "light"; break;
        case 3: activityLevel = "moderate"; break;
        case 4: activityLevel = "active"; break;
        case 5: activityLevel = "very_active"; break;
        default: activityLevel = "moderate";
        }

        currentUser = &userManager.registerUser(username, age, gender, weight, height, activityLevel);
        if (currentUser) {
            cout << "\n注册成功！" << endl;
            cout << "您的用户ID是: " << currentUser->getUserId() << endl;
            cout << "请记住此ID用于登录。" << endl;
        }
        else {
            cout << "注册失败！" << endl;
        }
    }

    // 用户登录
    void loginUser() {
        system("cls");
        cout << "=== 用户登录 ===" << endl;

        cout << "请输入用户ID: ";
        int userId;
        cin >> userId;
        cin.ignore();

        currentUser = userManager.loginUser(userId);
        if (currentUser) {
            cout << "\n登录成功！欢迎 " << currentUser->getUsername() << endl;
        }
        else {
            cout << "登录失败！用户ID不存在。" << endl;
        }
    }

    // 设置偏好
    void setPreferences() {
        if (!currentUser) {
            cout << "请先登录！" << endl;
            return;
        }

        system("cls");
        cout << "=== 设置用户偏好 ===" << endl;

        auto& pref = currentUser->getPreferences();

        while (true) {
            system("cls");
            cout << "=== 偏好设置菜单 ===" << endl;
            cout << "1. 设置口味偏好" << endl;
            cout << "2. 设置饮食限制" << endl;
            cout << "3. 添加不喜欢的食物" << endl;
            cout << "4. 查看当前设置" << endl;
            cout << "5. 返回主菜单" << endl;
            cout << "请选择 (1-5): ";

            int choice;
            cin >> choice;
            cin.ignore();

            switch (choice) {
            case 1: setTastePreferences(pref); break;
            case 2: setDietaryRestrictions(pref); break;
            case 3: addDislikedFoods(pref); break;
            case 4: pref.display(); break;
            case 5: return;
            default: cout << "无效选择！" << endl;
            }

            cout << "\n按Enter键继续...";
            cin.get();
        }
    }

    // 设置口味偏好
    void setTastePreferences(UserPreferences& pref) {
        cout << "\n=== 设置口味偏好 (0-1) ===" << endl;
        cout << "当前设置:" << endl;
        cout << " 甜度: " << pref.getTasteWeight("sweet") << endl;
        cout << " 咸度: " << pref.getTasteWeight("salty") << endl;
        cout << " 辣度: " << pref.getTasteWeight("spicy") << endl;
        cout << " 酸度: " << pref.getTasteWeight("sour") << endl;
        cout << " 鲜度: " << pref.getTasteWeight("umami") << endl;

        cout << "\n输入新的权重值:" << endl;

        cout << "甜度 (0-1): ";
        double value;
        cin >> value;
        pref.setTasteWeight("sweet", value);

        cout << "咸度 (0-1): ";
        cin >> value;
        pref.setTasteWeight("salty", value);

        cout << "辣度 (0-1): ";
        cin >> value;
        pref.setTasteWeight("spicy", value);

        cout << "酸度 (0-1): ";
        cin >> value;
        pref.setTasteWeight("sour", value);

        cout << "鲜度 (0-1): ";
        cin >> value;
        pref.setTasteWeight("umami", value);

        cout << "口味偏好已更新！" << endl;
    }

    // 设置饮食限制
    void setDietaryRestrictions(UserPreferences& pref) {
        cout << "\n=== 设置饮食限制 ===" << endl;

        cout << "是否为素食者？ (1=是, 0=否): ";
        bool isVegetarian;
        cin >> isVegetarian;
        pref.setIsVegetarian(isVegetarian);

        cout << "是否需要清真食品？ (1=是, 0=否): ";
        bool isHalal;
        cin >> isHalal;
        pref.setIsHalal(isHalal);

        cout << "辣度耐受度 (0-1, 0=不能吃辣, 1=非常能吃辣): ";
        double tolerance;
        cin >> tolerance;
        pref.setSpicyTolerance(tolerance);

        cout << "饮食限制已更新！" << endl;
    }

    // 添加不喜欢的食物
    void addDislikedFoods(UserPreferences& pref) {
        cout << "\n=== 添加不喜欢的食物 ===" << endl;
        cout << "输入不喜欢的食物名称 (输入'quit'结束):" << endl;

        string foodName;
        while (true) {
            cout << "食物名称: ";
            getline(cin, foodName);

            if (foodName == "quit") {
                break;
            }

            if (!foodName.empty()) {
                pref.addDislikedFood(foodName);
                cout << "已添加: " << foodName << endl;
            }
        }
    }

    // 获取推荐
    void getRecommendation() {
        if (!currentUser) {
            cout << "请先登录！" << endl;
            return;
        }

        system("cls");
        cout << "=== 获取餐单推荐 ===" << endl;

        cout << "请选择餐单类型:" << endl;
        cout << "1. 早餐 (breakfast)" << endl;
        cout << "2. 午餐 (lunch)" << endl;
        cout << "3. 晚餐 (dinner)" << endl;
        cout << "4. 加餐 (snack)" << endl;
        cout << "5. 一日三餐 (daily)" << endl;
        cout << "请选择 (1-5): ";

        int choice;
        cin >> choice;
        cin.ignore();

        string mealType;
        switch (choice) {
        case 1: mealType = "breakfast"; break;
        case 2: mealType = "lunch"; break;
        case 3: mealType = "dinner"; break;
        case 4: mealType = "snack"; break;
        case 5: mealType = "daily"; break;
        default: mealType = "lunch";
        }

        if (mealType == "daily") {
            // 推荐一日三餐
            auto dailyMeals = recommender.recommendDailyMeals(*currentUser);

            for (const auto& mealPair : dailyMeals) {
                cout << "\n=== " << mealPair.first << " ===" << endl;
                displayMeal(mealPair.second, *currentUser);

                // 添加到历史
                currentUser->addMealToHistory(mealPair.second);
            }
        }
        else {
            // 推荐单餐
            vector<FoodItem> meal = recommender.recommendMeal(*currentUser, mealType);
            displayMeal(meal, *currentUser);

            // 添加到历史
            currentUser->addMealToHistory(meal);
        }

        cout << "\n推荐完成！已保存到历史记录。" << endl;
    }

    // 显示餐单
    void displayMeal(const vector<FoodItem>& meal, const User& user) {
        if (meal.empty()) {
            cout << "无法生成推荐餐单。" << endl;
            return;
        }

        NutritionInfo total;

        cout << "推荐食物:" << endl;
        for (size_t i = 0; i < meal.size(); i++) {
            cout << " " << (i + 1) << ". " << meal[i].getName()
                << " (" << meal[i].getCategory() << ")" << endl;
            total += meal[i].getNutrition();
        }

        cout << "\n营养总结:" << endl;
        total.display();

        cout << "\n占目标热量比例: "
            << fixed << setprecision(1)
            << (total.calories / user.getTargetCalories() * 100) << "%" << endl;
    }

    // 查看历史
    void viewHistory() {
        if (!currentUser) {
            cout << "请先登录！" << endl;
            return;
        }

        system("cls");
        cout << "=== 餐单历史记录 ===" << endl;

        const auto& history = currentUser->getMealHistory();

        if (history.empty()) {
            cout << "暂无历史记录。" << endl;
            return;
        }

        for (size_t i = 0; i < history.size(); i++) {
            cout << "\n第 " << (i + 1) << " 餐:" << endl;
            cout << "----------------" << endl;

            NutritionInfo total;
            for (const auto& food : history[i]) {
                cout << "  " << food.getName() << endl;
                total += food.getNutrition();
            }

            cout << "\n  总热量: " << total.calories << " kcal" << endl;
        }

        cout << "\n共 " << history.size() << " 条历史记录。" << endl;
    }

    // 浏览食物数据库
    void browseFoodDatabase() {
        system("cls");
        cout << "=== 食物数据库 ===" << endl;

        while (true) {
            cout << "\n请选择操作:" << endl;
            cout << "1. 查看所有食物" << endl;
            cout << "2. 搜索食物" << endl;
            cout << "3. 查看食物详情" << endl;
            cout << "4. 按类别查看" << endl;
            cout << "5. 返回主菜单" << endl;
            cout << "请选择 (1-5): ";

            int choice;
            cin >> choice;
            cin.ignore();

            switch (choice) {
            case 1: foodDB.displayAllFoods(); break;
            case 2: searchFoods(); break;
            case 3: viewFoodDetails(); break;
            case 4: viewFoodsByCategory(); break;
            case 5: return;
            default: cout << "无效选择！" << endl;
            }

            cout << "\n按Enter键继续...";
            cin.get();
            system("cls");
        }
    }

    // 搜索食物
    void searchFoods() {
        cout << "\n=== 搜索食物 ===" << endl;
        cout << "请输入关键词: ";
        string keyword;
        getline(cin, keyword);

        vector<FoodItem> results = foodDB.searchFoods(keyword);

        if (results.empty()) {
            cout << "未找到相关食物。" << endl;
        }
        else {
            cout << "\n找到 " << results.size() << " 种食物:" << endl;
            for (const auto& food : results) {
                cout << " [" << food.getId() << "] " << food.getName()
                    << " (" << food.getCategory() << ")" << endl;
            }
        }
    }

    // 查看食物详情
    void viewFoodDetails() {
        cout << "\n=== 查看食物详情 ===" << endl;
        cout << "请输入食物ID: ";

        int foodId;
        cin >> foodId;
        cin.ignore();

        foodDB.displayFoodDetails(foodId);
    }

    // 按类别查看食物
    void viewFoodsByCategory() {
        cout << "\n=== 按类别查看食物 ===" << endl;
        cout << "可用类别: staple, meat, seafood, vegetable, fruit, dairy, nut, egg, bean" << endl;
        cout << "请输入类别名称: ";

        string category;
        getline(cin, category);

        vector<FoodItem> foods = foodDB.getFoodsByCategory(category);

        if (foods.empty()) {
            cout << "该类别下没有食物。" << endl;
        }
        else {
            cout << "\n" << category << " 类别下的食物 (" << foods.size() << " 种):" << endl;
            for (const auto& food : foods) {
                cout << " [" << food.getId() << "] " << food.getName() << endl;
            }
        }
    }

    // 显示用户信息
    void showUserInfo() {
        if (!currentUser) {
            cout << "请先登录！" << endl;
            return;
        }

        system("cls");
        currentUser->display();
    }
};

#endif // MEAL_SYSTEM_H