#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <set>
#include <map>

class User {
private:
    int id;
    std::string username;
    std::string password;
    int age;
    double weight;            // weight (kg)
    double height;            // height (cm)
    std::string gender;       // gender
    std::string activityLevel; // activity level: sedentary, light, moderate, active, very_active
    
    double dailyCalorieGoal;  // daily calorie goal
    double dailyProteinGoal;  // daily protein goal (g)
    double dailyCarbGoal;     // daily carb goal (g)
    double dailyFatGoal;      // daily fat goal (g)
    
    std::set<std::string> preferredTags;  // preferred taste tags
    std::set<std::string> avoidedTags;    // avoided taste tags
    std::set<std::string> allergens;      // allergens

public:
    User();
    User(int id, const std::string& username, const std::string& password);

    // Getters
    int getId() const { return id; }
    std::string getUsername() const { return username; }
    std::string getPassword() const { return password; }
    int getAge() const { return age; }
    double getWeight() const { return weight; }
    double getHeight() const { return height; }
    std::string getGender() const { return gender; }
    std::string getActivityLevel() const { return activityLevel; }
    double getDailyCalorieGoal() const { return dailyCalorieGoal; }
    double getDailyProteinGoal() const { return dailyProteinGoal; }
    double getDailyCarbGoal() const { return dailyCarbGoal; }
    double getDailyFatGoal() const { return dailyFatGoal; }
    std::set<std::string> getPreferredTags() const { return preferredTags; }
    std::set<std::string> getAvoidedTags() const { return avoidedTags; }
    std::set<std::string> getAllergens() const { return allergens; }

    // Setters
    void setId(int id) { this->id = id; }
    void setUsername(const std::string& username) { this->username = username; }
    void setPassword(const std::string& password) { this->password = password; }
    void setAge(int age) { this->age = age; }
    void setWeight(double weight) { this->weight = weight; }
    void setHeight(double height) { this->height = height; }
    void setGender(const std::string& gender) { this->gender = gender; }
    void setActivityLevel(const std::string& level) { this->activityLevel = level; }
    void setDailyCalorieGoal(double cal) { this->dailyCalorieGoal = cal; }
    void setDailyProteinGoal(double prot) { this->dailyProteinGoal = prot; }
    void setDailyCarbGoal(double carb) { this->dailyCarbGoal = carb; }
    void setDailyFatGoal(double fat) { this->dailyFatGoal = fat; }

    void addPreferredTag(const std::string& tag);
    void addAvoidedTag(const std::string& tag);
    void addAllergen(const std::string& allergen);
    
    void calculateNutritionGoals();
    void displayProfile() const;
    std::string toString() const;
};

#endif
