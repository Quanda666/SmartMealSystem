#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

class Utils {
public:
    static std::string getCurrentDate();
    static void clearScreen();
    static void pause();
    static std::string trim(const std::string& str);
    static void displayProgressBar(double current, double target, const std::string& label);
    static void displayNutritionChart(double calories, double protein, double carbs, double fat);
    static std::string getInput(const std::string& prompt);
    static int getIntInput(const std::string& prompt);
    static double getDoubleInput(const std::string& prompt);
};

#endif
