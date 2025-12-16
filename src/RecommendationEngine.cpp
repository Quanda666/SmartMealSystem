#include "../include/RecommendationEngine.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>

RecommendationEngine::RecommendationEngine() {}

void RecommendationEngine::setFoodDatabase(const std::vector<Food>& foods) {
    foodDatabase = foods;
}

void RecommendationEngine::addToHistory(int userId, const Meal& meal) {
    userHistory[userId].push_back(meal);
}

void RecommendationEngine::loadHistory(const std::map<int, std::vector<Meal>>& history) {
    userHistory = history;
}

bool RecommendationEngine::isAllergenFree(const Food& food, const User& user) const {
    for (const auto& allergen : user.getAllergens()) {
        if (food.hasTag(allergen)) {
            return false;
        }
    }
    return true;
}

double RecommendationEngine::calculateFoodScore(const Food& food, const User& user,
                                                 const std::string& mealType,
                                                 double remainingCalories,
                                                 double remainingProtein,
                                                 double remainingCarbs,
                                                 double remainingFat) const {
    double score = 100.0;
    
    if (!isAllergenFree(food, user)) {
        return -1000.0;
    }
    
    for (const auto& avoidedTag : user.getAvoidedTags()) {
        if (food.hasTag(avoidedTag)) {
            score -= 50.0;
        }
    }
    
    for (const auto& preferredTag : user.getPreferredTags()) {
        if (food.hasTag(preferredTag)) {
            score += 30.0;
        }
    }
    
    double calorieDiff = std::abs(food.getCalories() - remainingCalories);
    score -= calorieDiff * 0.1;
    
    double proteinDiff = std::abs(food.getProtein() - remainingProtein);
    score -= proteinDiff * 0.5;
    
    double carbDiff = std::abs(food.getCarbohydrates() - remainingCarbs);
    score -= carbDiff * 0.3;
    
    double fatDiff = std::abs(food.getFat() - remainingFat);
    score -= fatDiff * 0.4;
    
    if (mealType == "breakfast" && food.getCategory() == "主食") {
        score += 20.0;
    } else if (mealType == "lunch" && food.getCategory() == "肉类") {
        score += 15.0;
    } else if (mealType == "dinner" && food.getCategory() == "蔬菜") {
        score += 15.0;
    }
    
    return score;
}

std::vector<Food> RecommendationEngine::filterFoodsByCategory(const std::string& category) const {
    std::vector<Food> filtered;
    for (const auto& food : foodDatabase) {
        if (food.getCategory() == category) {
            filtered.push_back(food);
        }
    }
    return filtered;
}

Meal RecommendationEngine::recommendMeal(const User& user, const std::string& mealType,
                                          double targetCalories, double targetProtein,
                                          double targetCarbs, double targetFat) {
    Meal meal(0, user.getId(), "", mealType);
    meal.setIsRecommended(true);
    
    double remainingCalories = targetCalories;
    double remainingProtein = targetProtein;
    double remainingCarbs = targetCarbs;
    double remainingFat = targetFat;
    
    std::vector<std::string> categories;
    if (mealType == "breakfast") {
        categories = {"主食", "蛋类", "饮品"};
    } else if (mealType == "lunch") {
        categories = {"主食", "肉类", "蔬菜", "汤"};
    } else if (mealType == "dinner") {
        categories = {"主食", "蔬菜", "豆制品"};
    } else {
        categories = {"水果", "坚果"};
    }
    
    for (const auto& category : categories) {
        std::vector<Food> categoryFoods = filterFoodsByCategory(category);
        if (categoryFoods.empty()) continue;
        
        std::vector<std::pair<double, Food>> scoredFoods;
        for (const auto& food : categoryFoods) {
            double score = calculateFoodScore(food, user, mealType,
                                             remainingCalories / categories.size(),
                                             remainingProtein / categories.size(),
                                             remainingCarbs / categories.size(),
                                             remainingFat / categories.size());
            if (score > -500) {
                scoredFoods.push_back({score, food});
            }
        }
        
        if (!scoredFoods.empty()) {
            std::sort(scoredFoods.begin(), scoredFoods.end(),
                     [](const auto& a, const auto& b) { return a.first > b.first; });
            
            meal.addFood(scoredFoods[0].second);
            remainingCalories -= scoredFoods[0].second.getCalories();
            remainingProtein -= scoredFoods[0].second.getProtein();
            remainingCarbs -= scoredFoods[0].second.getCarbohydrates();
            remainingFat -= scoredFoods[0].second.getFat();
        }
    }
    
    return meal;
}

std::vector<Meal> RecommendationEngine::recommendDailyMeals(const User& user, const std::string& date) {
    std::vector<Meal> dailyMeals;
    
    double breakfastCalories = user.getDailyCalorieGoal() * 0.3;
    double lunchCalories = user.getDailyCalorieGoal() * 0.4;
    double dinnerCalories = user.getDailyCalorieGoal() * 0.3;
    
    double breakfastProtein = user.getDailyProteinGoal() * 0.25;
    double lunchProtein = user.getDailyProteinGoal() * 0.4;
    double dinnerProtein = user.getDailyProteinGoal() * 0.35;
    
    double breakfastCarbs = user.getDailyCarbGoal() * 0.3;
    double lunchCarbs = user.getDailyCarbGoal() * 0.4;
    double dinnerCarbs = user.getDailyCarbGoal() * 0.3;
    
    double breakfastFat = user.getDailyFatGoal() * 0.3;
    double lunchFat = user.getDailyFatGoal() * 0.4;
    double dinnerFat = user.getDailyFatGoal() * 0.3;
    
    Meal breakfast = recommendMeal(user, "breakfast", breakfastCalories, 
                                   breakfastProtein, breakfastCarbs, breakfastFat);
    breakfast.setDate(date);
    dailyMeals.push_back(breakfast);
    
    Meal lunch = recommendMeal(user, "lunch", lunchCalories,
                              lunchProtein, lunchCarbs, lunchFat);
    lunch.setDate(date);
    dailyMeals.push_back(lunch);
    
    Meal dinner = recommendMeal(user, "dinner", dinnerCalories,
                               dinnerProtein, dinnerCarbs, dinnerFat);
    dinner.setDate(date);
    dailyMeals.push_back(dinner);
    
    return dailyMeals;
}

std::vector<Food> RecommendationEngine::getAlternativeFoods(const Food& food, const User& user, int count) {
    std::vector<std::pair<double, Food>> scoredFoods;
    
    for (const auto& candidate : foodDatabase) {
        if (candidate.getId() == food.getId()) continue;
        if (candidate.getCategory() != food.getCategory()) continue;
        
        double score = calculateFoodScore(candidate, user, "",
                                         food.getCalories(),
                                         food.getProtein(),
                                         food.getCarbohydrates(),
                                         food.getFat());
        if (score > -500) {
            scoredFoods.push_back({score, candidate});
        }
    }
    
    std::sort(scoredFoods.begin(), scoredFoods.end(),
             [](const auto& a, const auto& b) { return a.first > b.first; });
    
    std::vector<Food> alternatives;
    for (size_t i = 0; i < std::min((size_t)count, scoredFoods.size()); ++i) {
        alternatives.push_back(scoredFoods[i].second);
    }
    
    return alternatives;
}

void RecommendationEngine::displayRecommendationStats(const User& user) const {
    auto it = userHistory.find(user.getId());
    if (it == userHistory.end() || it->second.empty()) {
        std::cout << "\n暂无历史记录。" << std::endl;
        return;
    }
    
    const auto& meals = it->second;
    double avgCalories = 0, avgProtein = 0, avgCarbs = 0, avgFat = 0;
    
    for (const auto& meal : meals) {
        avgCalories += meal.getTotalCalories();
        avgProtein += meal.getTotalProtein();
        avgCarbs += meal.getTotalCarbs();
        avgFat += meal.getTotalFat();
    }
    
    int count = meals.size();
    avgCalories /= count;
    avgProtein /= count;
    avgCarbs /= count;
    avgFat /= count;
    
    std::cout << "\n╔══════════════════════════════════════════╗" << std::endl;
    std::cout << "║        历史饮食统计                        ║" << std::endl;
    std::cout << "╠══════════════════════════════════════════╣" << std::endl;
    std::cout << "║  记录总数: " << count << " 餐" << std::endl;
    std::cout << "║  平均热量: " << (int)avgCalories << " kcal" << std::endl;
    std::cout << "║  平均蛋白质: " << (int)avgProtein << " g" << std::endl;
    std::cout << "║  平均碳水: " << (int)avgCarbs << " g" << std::endl;
    std::cout << "║  平均脂肪: " << (int)avgFat << " g" << std::endl;
    std::cout << "╚══════════════════════════════════════════╝" << std::endl;
}
