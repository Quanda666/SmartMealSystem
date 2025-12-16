#include "../include/Database.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

Database::Database() : usersFile("data/users.txt"), 
                       foodsFile("data/foods.txt"),
                       mealsFile("data/meals.txt") {}

Database::Database(const std::string& usersFile, const std::string& foodsFile, const std::string& mealsFile)
    : usersFile(usersFile), foodsFile(foodsFile), mealsFile(mealsFile) {}

std::vector<std::string> Database::split(const std::string& str, char delimiter) const {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

std::set<std::string> Database::parseTagString(const std::string& tagStr) const {
    std::set<std::string> tags;
    if (tagStr.empty()) return tags;
    
    auto tokens = split(tagStr, ',');
    for (const auto& token : tokens) {
        if (!token.empty()) {
            tags.insert(token);
        }
    }
    return tags;
}

std::string Database::tagsToString(const std::set<std::string>& tags) const {
    std::stringstream ss;
    bool first = true;
    for (const auto& tag : tags) {
        if (!first) ss << ",";
        ss << tag;
        first = false;
    }
    return ss.str();
}

bool Database::loadUsers() {
    std::ifstream file(usersFile);
    if (!file.is_open()) {
        return false;
    }
    
    users.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        auto parts = split(line, '|');
        if (parts.size() >= 15) {
            User user;
            user.setId(std::stoi(parts[0]));
            user.setUsername(parts[1]);
            user.setPassword(parts[2]);
            user.setAge(std::stoi(parts[3]));
            user.setWeight(std::stod(parts[4]));
            user.setHeight(std::stod(parts[5]));
            user.setGender(parts[6]);
            user.setActivityLevel(parts[7]);
            user.setDailyCalorieGoal(std::stod(parts[8]));
            user.setDailyProteinGoal(std::stod(parts[9]));
            user.setDailyCarbGoal(std::stod(parts[10]));
            user.setDailyFatGoal(std::stod(parts[11]));
            
            auto preferredTags = parseTagString(parts[12]);
            for (const auto& tag : preferredTags) {
                user.addPreferredTag(tag);
            }
            
            auto avoidedTags = parseTagString(parts[13]);
            for (const auto& tag : avoidedTags) {
                user.addAvoidedTag(tag);
            }
            
            auto allergens = parseTagString(parts[14]);
            for (const auto& allergen : allergens) {
                user.addAllergen(allergen);
            }
            
            users.push_back(user);
        }
    }
    
    file.close();
    return true;
}

bool Database::loadFoods() {
    std::ifstream file(foodsFile);
    if (!file.is_open()) {
        return false;
    }
    
    foods.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        auto parts = split(line, '|');
        if (parts.size() >= 9) {
            Food food;
            food.setId(std::stoi(parts[0]));
            food.setName(parts[1]);
            food.setCalories(std::stod(parts[2]));
            food.setProtein(std::stod(parts[3]));
            food.setCarbohydrates(std::stod(parts[4]));
            food.setFat(std::stod(parts[5]));
            food.setFiber(std::stod(parts[6]));
            food.setTags(parseTagString(parts[7]));
            food.setCategory(parts[8]);
            
            foods.push_back(food);
        }
    }
    
    file.close();
    return true;
}

bool Database::loadMeals() {
    std::ifstream file(mealsFile);
    if (!file.is_open()) {
        return false;
    }
    
    mealHistory.clear();
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        auto parts = split(line, '|');
        if (parts.size() >= 10) {
            Meal meal;
            meal.setId(std::stoi(parts[0]));
            int userId = std::stoi(parts[1]);
            meal.setUserId(userId);
            meal.setDate(parts[2]);
            meal.setMealType(parts[3]);
            meal.setIsRecommended(parts[8] == "1");
            
            auto foodIds = split(parts[9], ',');
            for (const auto& foodIdStr : foodIds) {
                if (!foodIdStr.empty()) {
                    int foodId = std::stoi(foodIdStr);
                    Food* food = findFoodById(foodId);
                    if (food) {
                        meal.addFood(*food);
                    }
                }
            }
            
            mealHistory[userId].push_back(meal);
        }
    }
    
    file.close();
    return true;
}

bool Database::saveUsers() {
    std::ofstream file(usersFile);
    if (!file.is_open()) {
        return false;
    }
    
    for (const auto& user : users) {
        file << user.toString() << std::endl;
    }
    
    file.close();
    return true;
}

bool Database::saveFoods() {
    std::ofstream file(foodsFile);
    if (!file.is_open()) {
        return false;
    }
    
    for (const auto& food : foods) {
        file << food.toString() << std::endl;
    }
    
    file.close();
    return true;
}

bool Database::saveMeals() {
    std::ofstream file(mealsFile);
    if (!file.is_open()) {
        return false;
    }
    
    for (const auto& pair : mealHistory) {
        for (const auto& meal : pair.second) {
            file << meal.toString() << std::endl;
        }
    }
    
    file.close();
    return true;
}

bool Database::addUser(const User& user) {
    for (const auto& u : users) {
        if (u.getUsername() == user.getUsername()) {
            return false;
        }
    }
    users.push_back(user);
    return saveUsers();
}

bool Database::updateUser(const User& user) {
    for (auto& u : users) {
        if (u.getId() == user.getId()) {
            u = user;
            return saveUsers();
        }
    }
    return false;
}

User* Database::findUserByUsername(const std::string& username) {
    for (auto& user : users) {
        if (user.getUsername() == username) {
            return &user;
        }
    }
    return nullptr;
}

User* Database::findUserById(int id) {
    for (auto& user : users) {
        if (user.getId() == id) {
            return &user;
        }
    }
    return nullptr;
}

bool Database::addFood(const Food& food) {
    foods.push_back(food);
    return saveFoods();
}

bool Database::updateFood(const Food& food) {
    for (auto& f : foods) {
        if (f.getId() == food.getId()) {
            f = food;
            return saveFoods();
        }
    }
    return false;
}

Food* Database::findFoodById(int id) {
    for (auto& food : foods) {
        if (food.getId() == id) {
            return &food;
        }
    }
    return nullptr;
}

std::vector<Food> Database::getAllFoods() const {
    return foods;
}

std::vector<Food> Database::searchFoodsByName(const std::string& keyword) const {
    std::vector<Food> results;
    for (const auto& food : foods) {
        if (food.getName().find(keyword) != std::string::npos) {
            results.push_back(food);
        }
    }
    return results;
}

bool Database::addMeal(const Meal& meal) {
    mealHistory[meal.getUserId()].push_back(meal);
    return saveMeals();
}

std::vector<Meal> Database::getUserMeals(int userId) const {
    auto it = mealHistory.find(userId);
    if (it != mealHistory.end()) {
        return it->second;
    }
    return std::vector<Meal>();
}

std::vector<Meal> Database::getUserMealsByDate(int userId, const std::string& date) const {
    std::vector<Meal> result;
    auto it = mealHistory.find(userId);
    if (it != mealHistory.end()) {
        for (const auto& meal : it->second) {
            if (meal.getDate() == date) {
                result.push_back(meal);
            }
        }
    }
    return result;
}

int Database::getNextUserId() const {
    int maxId = 0;
    for (const auto& user : users) {
        if (user.getId() > maxId) {
            maxId = user.getId();
        }
    }
    return maxId + 1;
}

int Database::getNextFoodId() const {
    int maxId = 0;
    for (const auto& food : foods) {
        if (food.getId() > maxId) {
            maxId = food.getId();
        }
    }
    return maxId + 1;
}

int Database::getNextMealId() const {
    int maxId = 0;
    for (const auto& pair : mealHistory) {
        for (const auto& meal : pair.second) {
            if (meal.getId() > maxId) {
                maxId = meal.getId();
            }
        }
    }
    return maxId + 1;
}

void Database::initializeSampleData() {
    foods.clear();
    
    foods.push_back(Food(1, "白米饭", 116, 2.6, 25.6, 0.3, 0.3, {"清淡"}, "主食"));
    foods.push_back(Food(2, "全麦面包", 246, 8.5, 45.3, 3.5, 6.8, {"清淡"}, "主食"));
    foods.push_back(Food(3, "燕麦粥", 68, 2.4, 12.0, 1.4, 1.7, {"清淡"}, "主食"));
    foods.push_back(Food(4, "红薯", 86, 1.6, 20.1, 0.2, 3.0, {"甜"}, "主食"));
    
    foods.push_back(Food(5, "鸡胸肉", 133, 24.6, 2.5, 5.0, 0.0, {"清淡"}, "肉类"));
    foods.push_back(Food(6, "三文鱼", 206, 22.5, 0.0, 13.4, 0.0, {"鲜"}, "肉类"));
    foods.push_back(Food(7, "牛肉", 250, 26.3, 0.0, 15.8, 0.0, {"鲜"}, "肉类"));
    foods.push_back(Food(8, "猪里脊", 143, 20.3, 1.3, 7.9, 0.0, {"鲜"}, "肉类"));
    
    foods.push_back(Food(9, "西兰花", 34, 4.1, 6.6, 0.4, 3.7, {"清淡"}, "蔬菜"));
    foods.push_back(Food(10, "菠菜", 23, 2.9, 3.6, 0.3, 2.2, {"清淡"}, "蔬菜"));
    foods.push_back(Food(11, "胡萝卜", 41, 0.9, 9.6, 0.2, 2.8, {"甜"}, "蔬菜"));
    foods.push_back(Food(12, "番茄", 18, 0.9, 3.9, 0.2, 1.2, {"酸"}, "蔬菜"));
    
    foods.push_back(Food(13, "豆腐", 76, 8.1, 4.2, 3.7, 0.4, {"清淡"}, "豆制品"));
    foods.push_back(Food(14, "豆浆", 31, 3.0, 1.1, 1.6, 1.1, {"清淡"}, "豆制品"));
    
    foods.push_back(Food(15, "鸡蛋", 147, 12.6, 1.3, 10.6, 0.0, {"清淡"}, "蛋类"));
    foods.push_back(Food(16, "鸭蛋", 180, 12.6, 3.1, 13.0, 0.0, {"咸"}, "蛋类"));
    
    foods.push_back(Food(17, "苹果", 52, 0.3, 13.8, 0.2, 2.4, {"甜"}, "水果"));
    foods.push_back(Food(18, "香蕉", 89, 1.1, 22.8, 0.3, 2.6, {"甜"}, "水果"));
    foods.push_back(Food(19, "橙子", 47, 0.9, 11.8, 0.1, 2.4, {"酸","甜"}, "水果"));
    
    foods.push_back(Food(20, "核桃", 654, 15.2, 13.7, 65.2, 6.7, {"香"}, "坚果"));
    foods.push_back(Food(21, "杏仁", 578, 21.3, 21.7, 50.6, 12.2, {"香"}, "坚果"));
    
    foods.push_back(Food(22, "牛奶", 54, 3.0, 3.4, 3.2, 0.0, {"清淡"}, "饮品"));
    foods.push_back(Food(23, "酸奶", 72, 2.5, 9.3, 2.7, 0.0, {"酸","甜"}, "饮品"));
    
    foods.push_back(Food(24, "紫菜蛋花汤", 35, 3.2, 2.1, 1.8, 0.5, {"鲜"}, "汤"));
    foods.push_back(Food(25, "番茄蛋汤", 45, 3.5, 3.2, 2.3, 0.8, {"酸","鲜"}, "汤"));
    
    saveFoods();
    std::cout << "已初始化 " << foods.size() << " 种食物数据。" << std::endl;
}
