#include "../include/User.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

User::User() : id(0), username(""), password(""), age(0), weight(0), 
               height(0), gender(""), activityLevel("moderate"),
               dailyCalorieGoal(2000), dailyProteinGoal(50), 
               dailyCarbGoal(250), dailyFatGoal(65) {}

User::User(int id, const std::string& username, const std::string& password)
    : id(id), username(username), password(password), age(25), weight(65), 
      height(170), gender("male"), activityLevel("moderate"),
      dailyCalorieGoal(2000), dailyProteinGoal(50), 
      dailyCarbGoal(250), dailyFatGoal(65) {}

void User::addPreferredTag(const std::string& tag) {
    preferredTags.insert(tag);
}

void User::addAvoidedTag(const std::string& tag) {
    avoidedTags.insert(tag);
}

void User::addAllergen(const std::string& allergen) {
    allergens.insert(allergen);
}

void User::calculateNutritionGoals() {
    double bmr;
    if (gender == "male" || gender == "Male") {
        bmr = 10 * weight + 6.25 * height - 5 * age + 5;
    } else {
        bmr = 10 * weight + 6.25 * height - 5 * age - 161;
    }
    
    double activityMultiplier = 1.2;
    if (activityLevel == "sedentary") activityMultiplier = 1.2;
    else if (activityLevel == "light") activityMultiplier = 1.375;
    else if (activityLevel == "moderate") activityMultiplier = 1.55;
    else if (activityLevel == "active") activityMultiplier = 1.725;
    else if (activityLevel == "very_active") activityMultiplier = 1.9;
    
    dailyCalorieGoal = bmr * activityMultiplier;
    dailyProteinGoal = weight * 1.2;
    dailyCarbGoal = dailyCalorieGoal * 0.5 / 4;
    dailyFatGoal = dailyCalorieGoal * 0.25 / 9;
}

void User::displayProfile() const {
    std::cout << "\n===============================================" << std::endl;
    std::cout << "              User Profile                     " << std::endl;
    std::cout << "===============================================" << std::endl;
    std::cout << "  Username: " << std::left << std::setw(38) << username << std::endl;
    std::cout << "  Age:      " << std::left << std::setw(38) << age << std::endl;
    std::cout << "  Gender:   " << std::left << std::setw(38) << gender << std::endl;
    std::cout << "  Weight:   " << std::left << std::setw(38) << (std::to_string((int)weight) + " kg") << std::endl;
    std::cout << "  Height:   " << std::left << std::setw(38) << (std::to_string((int)height) + " cm") << std::endl;
    std::cout << "  Activity: " << std::left << std::setw(38) << activityLevel << std::endl;
    std::cout << "===============================================" << std::endl;
    std::cout << "  Daily Nutrition Goals:                       " << std::endl;
    std::cout << "    Calories:  " << std::left << std::setw(33) << (std::to_string((int)dailyCalorieGoal) + " kcal") << std::endl;
    std::cout << "    Protein:   " << std::left << std::setw(33) << (std::to_string((int)dailyProteinGoal) + " g") << std::endl;
    std::cout << "    Carbs:     " << std::left << std::setw(33) << (std::to_string((int)dailyCarbGoal) + " g") << std::endl;
    std::cout << "    Fat:       " << std::left << std::setw(33) << (std::to_string((int)dailyFatGoal) + " g") << std::endl;
    
    if (!preferredTags.empty()) {
        std::cout << "===============================================" << std::endl;
        std::cout << "  Preferred: ";
        std::stringstream ss;
        bool first = true;
        for (const auto& tag : preferredTags) {
            if (!first) ss << ", ";
            ss << tag;
            first = false;
        }
        std::cout << std::left << std::setw(38) << ss.str() << std::endl;
    }
    
    if (!avoidedTags.empty()) {
        std::cout << "  Avoided:   ";
        std::stringstream ss;
        bool first = true;
        for (const auto& tag : avoidedTags) {
            if (!first) ss << ", ";
            ss << tag;
            first = false;
        }
        std::cout << std::left << std::setw(38) << ss.str() << std::endl;
    }
    
    std::cout << "===============================================" << std::endl;
}

std::string User::toString() const {
    std::stringstream ss;
    ss << id << "|" << username << "|" << password << "|" << age << "|"
       << weight << "|" << height << "|" << gender << "|" << activityLevel << "|"
       << dailyCalorieGoal << "|" << dailyProteinGoal << "|" 
       << dailyCarbGoal << "|" << dailyFatGoal << "|";
    
    bool first = true;
    for (const auto& tag : preferredTags) {
        if (!first) ss << ",";
        ss << tag;
        first = false;
    }
    ss << "|";
    
    first = true;
    for (const auto& tag : avoidedTags) {
        if (!first) ss << ",";
        ss << tag;
        first = false;
    }
    ss << "|";
    
    first = true;
    for (const auto& allergen : allergens) {
        if (!first) ss << ",";
        ss << allergen;
        first = false;
    }
    
    return ss.str();
}