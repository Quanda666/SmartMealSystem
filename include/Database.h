#ifndef DATABASE_H
#define DATABASE_H

#include "User.h"
#include "Food.h"
#include "Meal.h"
#include <vector>
#include <map>
#include <string>

class Database {
private:
    std::vector<User> users;
    std::vector<Food> foods;
    std::map<int, std::vector<Meal>> mealHistory;
    
    std::string usersFile;
    std::string foodsFile;
    std::string mealsFile;

    std::vector<std::string> split(const std::string& str, char delimiter) const;
    std::set<std::string> parseTagString(const std::string& tagStr) const;
    std::string tagsToString(const std::set<std::string>& tags) const;

public:
    Database();
    Database(const std::string& usersFile, const std::string& foodsFile, const std::string& mealsFile);

    bool loadUsers();
    bool loadFoods();
    bool loadMeals();
    
    bool saveUsers();
    bool saveFoods();
    bool saveMeals();
    
    bool addUser(const User& user);
    bool updateUser(const User& user);
    User* findUserByUsername(const std::string& username);
    User* findUserById(int id);
    
    bool addFood(const Food& food);
    bool updateFood(const Food& food);
    Food* findFoodById(int id);
    std::vector<Food> getAllFoods() const;
    std::vector<Food> searchFoodsByName(const std::string& keyword) const;
    
    bool addMeal(const Meal& meal);
    std::vector<Meal> getUserMeals(int userId) const;
    std::vector<Meal> getUserMealsByDate(int userId, const std::string& date) const;
    
    int getNextUserId() const;
    int getNextFoodId() const;
    int getNextMealId() const;

    void initializeSampleData();
};

#endif
