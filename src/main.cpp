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
            std::cout << "First run, initializing data..." << std::endl;
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
        
        std::cout << "\nThank you for using the Intelligent Meal Recommendation System! Goodbye!" << std::endl;
    }

private:
    void showWelcome() {
        Utils::clearScreen();
        std::cout << "============================================" << std::endl;
        std::cout << "                                      " << std::endl;
        std::cout << "     Intelligent Meal Recommendation System v1.0" << std::endl;
        std::cout << "                                      " << std::endl;
        std::cout << "============================================" << std::endl;
        std::cout << std::endl;
    }

    bool loginMenu() {
        std::cout << "\n=================================" << std::endl;
        std::cout << "         Welcome                   " << std::endl;
        std::cout << "=================================" << std::endl;
        std::cout << "  1. User Login                      " << std::endl;
        std::cout << "  2. User Registration                      " << std::endl;
        std::cout << "  0. Exit System                      " << std::endl;
        std::cout << "=================================" << std::endl;
        
        int choice = Utils::getIntInput("\nPlease select (0-2): ");
        
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
                std::cout << "Invalid choice, please try again." << std::endl;
        }
        return true;
    }

    void login() {
        std::cout << "\n=== User Login ===" << std::endl;
        std::string username = Utils::getStringInput("Please enter username: ");
        std::string password = Utils::getStringInput("Please enter password: ");
        
        auto users = db.getAllUsers();
        for (auto& user : users) {
            if (user.getUsername() == username && user.getPassword() == password) {
                currentUser = &user;
                std::cout << "\nLogin successful!" << std::endl;
                return;
            }
        }
        
        std::cout << "\nLogin failed! Invalid username or password." << std::endl;
        Utils::pause();
    }

    void registerUser() {
        std::cout << "\n=== User Registration ===" << std::endl;
        
        std::string username = Utils::getStringInput("Please enter username: ");
        
        // Check if username already exists
        auto users = db.getAllUsers();
        for (const auto& user : users) {
            if (user.getUsername() == username) {
                std::cout << "\nUsername already exists! Please choose a different username." << std::endl;
                Utils::pause();
                return;
            }
        }
        
        std::string password = Utils::getStringInput("Please enter password: ");
        std::string confirmPassword = Utils::getStringInput("Please confirm password: ");
        
        if (password != confirmPassword) {
            std::cout << "\nPasswords do not match!" << std::endl;
            Utils::pause();
            return;
        }
        
        int userId = db.getNextUserId();
        User newUser(userId, username, password);
        
        // Get user profile information
        std::cout << "\nPlease provide your profile information:" << std::endl;
        
        newUser.setAge(Utils::getIntInput("Age: "));
        newUser.setWeight(Utils::getDoubleInput("Weight (kg): "));
        newUser.setHeight(Utils::getDoubleInput("Height (cm): "));
        
        std::cout << "\nGender (1. Male, 2. Female): ";
        int genderChoice = Utils::getIntInput("");
        newUser.setGender(genderChoice == 1 ? "male" : "female");
        
        std::cout << "\nActivity Level: " << std::endl;
        std::cout << "1. Sedentary (little or no exercise)" << std::endl;
        std::cout << "2. Light (light exercise 1-3 days/week)" << std::endl;
        std::cout << "3. Moderate (moderate exercise 3-5 days/week)" << std::endl;
        std::cout << "4. Active (hard exercise 6-7 days a week)" << std::endl;
        std::cout << "5. Very Active (very hard exercise, physical job)" << std::endl;
        
        int activityChoice = Utils::getIntInput("Please select (1-5): ");
        std::vector<std::string> activityLevels = {"sedentary", "light", "moderate", "active", "very_active"};
        if (activityChoice >= 1 && activityChoice <= 5) {
            newUser.setActivityLevel(activityLevels[activityChoice - 1]);
        } else {
            newUser.setActivityLevel("moderate");
        }
        
        // Calculate nutrition goals
        newUser.calculateNutritionGoals();
        
        // Add to database
        if (db.saveUser(newUser)) {
            std::cout << "\nRegistration successful!" << std::endl;
            std::cout << "You can now log in with your credentials." << std::endl;
        } else {
            std::cout << "\nRegistration failed! Please try again." << std::endl;
        }
        
        Utils::pause();
    }

    void mainMenu() {
        Utils::clearScreen();
        std::cout << "\n=================================" << std::endl;
        std::cout << "    Main Menu - Welcome " << currentUser->getUsername() << std::endl;
        std::cout << "=================================" << std::endl;
        std::cout << "  1. View Profile                      " << std::endl;
        std::cout << "  2. Get Meal Recommendations         " << std::endl;
        std::cout << "  3. View Food Database               " << std::endl;
        std::cout << "  4. View Meal History                " << std::endl;
        std::cout << "  5. Nutrition Statistics             " << std::endl;
        std::cout << "  6. Set Preferences                  " << std::endl;
        std::cout << "  0. Logout                      " << std::endl;
        std::cout << "=================================" << std::endl;
        
        int choice = Utils::getIntInput("\nPlease select (0-6): ");
        
        switch (choice) {
            case 1:
                viewProfile();
                break;
            case 2:
                getRecommendations();
                break;
            case 3:
                viewFoodDatabase();
                break;
            case 4:
                viewMealHistory();
                break;
            case 5:
                viewNutritionStats();
                break;
            case 6:
                setPreferences();
                break;
            case 0:
                logout();
                break;
            default:
                std::cout << "Invalid choice, please try again." << std::endl;
        }
    }

    void viewProfile() {
        Utils::clearScreen();
        currentUser->displayProfile();
        
        std::cout << "\nDo you want to update your profile? (y/n): ";
        std::string choice = Utils::getStringInput("");
        if (choice == "y" || choice == "Y") {
            updateProfile();
        }
        
        Utils::pause();
    }

    void updateProfile() {
        std::cout << "\n=== Update Profile ===" << std::endl;
        
        std::cout << "Current age: " << currentUser->getAge() << std::endl;
        std::cout << "New age (or press Enter to keep): ";
        std::string input = Utils::getStringInput("");
        if (!input.empty()) {
            currentUser->setAge(std::stoi(input));
        }
        
        std::cout << "Current weight: " << currentUser->getWeight() << " kg" << std::endl;
        std::cout << "New weight (or press Enter to keep): ";
        input = Utils::getStringInput("");
        if (!input.empty()) {
            currentUser->setWeight(std::stod(input));
        }
        
        std::cout << "Current height: " << currentUser->getHeight() << " cm" << std::endl;
        std::cout << "New height (or press Enter to keep): ";
        input = Utils::getStringInput("");
        if (!input.empty()) {
            currentUser->setHeight(std::stod(input));
        }
        
        // Recalculate nutrition goals
        currentUser->calculateNutritionGoals();
        
        // Save to database
        if (db.updateUser(*currentUser)) {
            std::cout << "\nProfile updated successfully!" << std::endl;
        } else {
            std::cout << "\nFailed to update profile!" << std::endl;
        }
    }

    void getRecommendations() {
        Utils::clearScreen();
        std::cout << "\n=== Meal Recommendations ===" << std::endl;
        std::cout << "Select meal type:" << std::endl;
        std::cout << "1. Breakfast" << std::endl;
        std::cout << "2. Lunch" << std::endl;
        std::cout << "3. Dinner" << std::endl;
        std::cout << "4. Snacks" << std::endl;
        
        int mealChoice = Utils::getIntInput("\nPlease select (1-4): ");
        std::vector<std::string> mealTypes = {"breakfast", "lunch", "dinner", "snacks"};
        
        if (mealChoice < 1 || mealChoice > 4) {
            std::cout << "Invalid choice!" << std::endl;
            Utils::pause();
            return;
        }
        
        std::string mealType = mealTypes[mealChoice - 1];
        
        // Get meal recommendation
        Meal recommendedMeal = engine.recommendMeal(*currentUser, mealType,
                                                   currentUser->getDailyCalorieGoal() / 3,
                                                   currentUser->getDailyProteinGoal() / 3,
                                                   currentUser->getDailyCarbGoal() / 3,
                                                   currentUser->getDailyFatGoal() / 3);
        
        // Display recommendation
        std::cout << "\n=== Recommended " << mealType << " ===" << std::endl;
        recommendedMeal.displayMeal();
        
        // Ask if user wants to save this meal
        std::cout << "\nDo you want to save this meal? (y/n): ";
        std::string saveChoice = Utils::getStringInput("");
        if (saveChoice == "y" || saveChoice == "Y") {
            int mealId = db.getNextMealId();
            recommendedMeal.setId(mealId);
            recommendedMeal.setDate(Utils::getStringInput("Enter date (YYYY-MM-DD): "));
            
            if (db.saveMeal(recommendedMeal)) {
                std::cout << "Meal saved successfully!" << std::endl;
                engine.addToHistory(currentUser->getId(), recommendedMeal);
            } else {
                std::cout << "Failed to save meal!" << std::endl;
            }
        }
        
        Utils::pause();
    }

    void viewFoodDatabase() {
        Utils::clearScreen();
        std::cout << "\n=== Food Database ===" << std::endl;
        
        auto foods = db.getAllFoods();
        
        // Group foods by category
        std::map<std::string, std::vector<Food>> foodsByCategory;
        for (const auto& food : foods) {
            foodsByCategory[food.getCategory()].push_back(food);
        }
        
        std::cout << "Available categories:" << std::endl;
        int i = 1;
        std::vector<std::string> categories;
        for (const auto& pair : foodsByCategory) {
            std::cout << i << ". " << pair.first << " (" << pair.second.size() << " foods)" << std::endl;
            categories.push_back(pair.first);
            i++;
        }
        std::cout << "0. Back to main menu" << std::endl;
        
        int categoryChoice = Utils::getIntInput("\nPlease select category (0-" + std::to_string(categories.size()) + "): ");
        
        if (categoryChoice == 0) {
            return;
        } else if (categoryChoice >= 1 && categoryChoice <= categories.size()) {
            std::string selectedCategory = categories[categoryChoice - 1];
            showFoodsByCategory(selectedCategory, foodsByCategory[selectedCategory]);
        } else {
            std::cout << "Invalid choice!" << std::endl;
        }
        
        Utils::pause();
    }

    void showFoodsByCategory(const std::string& category, const std::vector<Food>& foods) {
        Utils::clearScreen();
        std::cout << "\n=== " << category << " ===" << std::endl;
        
        for (size_t i = 0; i < foods.size(); ++i) {
            std::cout << "\n" << (i + 1) << ". " << foods[i].getName() << std::endl;
            std::cout << "   Calories: " << foods[i].getCalories() << " kcal" << std::endl;
            std::cout << "   Protein: " << foods[i].getProtein() << " g" << std::endl;
        }
        
        std::cout << "\nEnter food number to view details (0 to go back): ";
        int choice = Utils::getIntInput("");
        
        if (choice >= 1 && choice <= foods.size()) {
            foods[choice - 1].displayInfo();
        }
    }

    void viewMealHistory() {
        Utils::clearScreen();
        std::cout << "\n=== Meal History ===" << std::endl;
        
        auto meals = db.getMealsByUser(currentUser->getId());
        
        if (meals.empty()) {
            std::cout << "No meal history found." << std::endl;
            Utils::pause();
            return;
        }
        
        // Group meals by date
        std::map<std::string, std::vector<Meal>> mealsByDate;
        for (const auto& meal : meals) {
            mealsByDate[meal.getDate()].push_back(meal);
        }
        
        for (const auto& dateGroup : mealsByDate) {
            std::cout << "\n=== " << dateGroup.first << " ===" << std::endl;
            
            for (const auto& meal : dateGroup.second) {
                std::cout << meal.getMealType() << ": " << meal.getTotalCalories() << " kcal" << std::endl;
                
                // Show food details
                auto foods = meal.getFoods();
                for (const auto& food : foods) {
                    std::cout << "  - " << food.getName() << std::endl;
                }
            }
        }
        
        Utils::pause();
    }

    void viewNutritionStats() {
        Utils::clearScreen();
        std::cout << "\n=== Nutrition Statistics ===" << std::endl;
        
        auto meals = db.getMealsByUser(currentUser->getId());
        
        if (meals.empty()) {
            std::cout << "No meal data found for statistics." << std::endl;
            Utils::pause();
            return;
        }
        
        // Calculate total nutrition consumed
        double totalCalories = 0, totalProtein = 0, totalCarbs = 0, totalFat = 0;
        int mealCount = 0;
        
        for (const auto& meal : meals) {
            totalCalories += meal.getTotalCalories();
            totalProtein += meal.getTotalProtein();
            totalCarbs += meal.getTotalCarbs();
            totalFat += meal.getTotalFat();
            mealCount++;
        }
        
        std::cout << "Overall Nutrition Summary:" << std::endl;
        std::cout << "Total meals recorded: " << mealCount << std::endl;
        std::cout << "Average calories per meal: " << (totalCalories / mealCount) << std::endl;
        std::cout << "Average protein per meal: " << (totalProtein / mealCount) << " g" << std::endl;
        std::cout << "Average carbs per meal: " << (totalCarbs / mealCount) << " g" << std::endl;
        std::cout << "Average fat per meal: " << (totalFat / mealCount) << " g" << std::endl;
        
        // Show progress towards daily goals
        std::cout << "\nProgress towards daily goals:" << std::endl;
        std::cout << "Calories: ";
        Utils::displayProgressBar((totalCalories / mealCount) / currentUser->getDailyCalorieGoal() * 100, 30);
        std::cout << std::endl;
        
        std::cout << "Protein: ";
        Utils::displayProgressBar((totalProtein / mealCount) / currentUser->getDailyProteinGoal() * 100, 30);
        std::cout << std::endl;
        
        std::cout << "Carbs: ";
        Utils::displayProgressBar((totalCarbs / mealCount) / currentUser->getDailyCarbGoal() * 100, 30);
        std::cout << std::endl;
        
        std::cout << "Fat: ";
        Utils::displayProgressBar((totalFat / mealCount) / currentUser->getDailyFatGoal() * 100, 30);
        std::cout << std::endl;
        
        Utils::pause();
    }

    void setPreferences() {
        Utils::clearScreen();
        std::cout << "\n=== Set Preferences ===" << std::endl;
        
        std::cout << "Current preferred tags: ";
        auto preferredTags = currentUser->getPreferredTags();
        if (preferredTags.empty()) {
            std::cout << "None" << std::endl;
        } else {
            for (const auto& tag : preferredTags) {
                std::cout << tag << " ";
            }
            std::cout << std::endl;
        }
        
        std::cout << "Current avoided tags: ";
        auto avoidedTags = currentUser->getAvoidedTags();
        if (avoidedTags.empty()) {
            std::cout << "None" << std::endl;
        } else {
            for (const auto& tag : avoidedTags) {
                std::cout << tag << " ";
            }
            std::cout << std::endl;
        }
        
        std::cout << "\nAvailable tags: light, spicy, sweet, salty, sour, fresh, aromatic" << std::endl;
        
        std::cout << "\n1. Add preferred tag" << std::endl;
        std::cout << "2. Remove preferred tag" << std::endl;
        std::cout << "3. Add avoided tag" << std::endl;
        std::cout << "4. Remove avoided tag" << std::endl;
        std::cout << "5. Add allergen" << std::endl;
        std::cout << "6. Remove allergen" << std::endl;
        std::cout << "0. Back to main menu" << std::endl;
        
        int choice = Utils::getIntInput("\nPlease select (0-6): ");
        
        std::string tag;
        auto allergens = currentUser->getAllergens();
        
        switch (choice) {
            case 1:
                tag = Utils::getStringInput("Enter tag to add to preferences: ");
                currentUser->addPreferredTag(tag);
                break;
            case 2:
                tag = Utils::getStringInput("Enter tag to remove from preferences: ");
                currentUser->getPreferredTags().erase(tag);
                break;
            case 3:
                tag = Utils::getStringInput("Enter tag to avoid: ");
                currentUser->addAvoidedTag(tag);
                break;
            case 4:
                tag = Utils::getStringInput("Enter tag to remove from avoid list: ");
                currentUser->getAvoidedTags().erase(tag);
                break;
            case 5:
                tag = Utils::getStringInput("Enter allergen: ");
                currentUser->addAllergen(tag);
                break;
            case 6:
                tag = Utils::getStringInput("Enter allergen to remove: ");
                allergens.erase(tag);
                break;
            case 0:
                return;
            default:
                std::cout << "Invalid choice!" << std::endl;
                Utils::pause();
                return;
        }
        
        // Save changes to database
        if (db.updateUser(*currentUser)) {
            std::cout << "\nPreferences updated successfully!" << std::endl;
        } else {
            std::cout << "\nFailed to update preferences!" << std::endl;
        }
        
        Utils::pause();
    }

    void logout() {
        std::cout << "\nLogging out..." << std::endl;
        currentUser = nullptr;
        std::cout << "Logged out successfully!" << std::endl;
        Utils::pause();
    }
};

int main() {
    try {
        MealRecommendationSystem system;
        system.run();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}