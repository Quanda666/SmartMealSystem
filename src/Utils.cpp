#include "../include/Utils.h"
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

std::string Utils::getCurrentDate() {
    time_t now = time(0);
    struct tm timeinfo;
    
#ifdef _WIN32
    localtime_s(&timeinfo, &now);
#else
    localtime_r(&now, &timeinfo);
#endif
    
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", &timeinfo);
    return std::string(buffer);
}

void Utils::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Utils::pause() {
    std::cout << "\n按 Enter 键继续...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

std::string Utils::trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

void Utils::displayProgressBar(double current, double target, const std::string& label) {
    int barWidth = 30;
    double progress = (target > 0) ? (current / target) : 0;
    if (progress > 1.0) progress = 1.0;
    
    std::cout << "  " << std::left << std::setw(12) << label << " [";
    
    int pos = static_cast<int>(barWidth * progress);
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "█";
        else std::cout << "░";
    }
    
    std::cout << "] " << std::fixed << std::setprecision(1) 
              << (progress * 100.0) << "% (" 
              << (int)current << "/" << (int)target << ")" << std::endl;
}

void Utils::displayNutritionChart(double calories, double protein, double carbs, double fat) {
    std::cout << "\n╔════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║             营养成分分布                         ║" << std::endl;
    std::cout << "╠════════════════════════════════════════════════╣" << std::endl;
    
    displayProgressBar(protein * 4, calories, "蛋白质");
    displayProgressBar(carbs * 4, calories, "碳水化合物");
    displayProgressBar(fat * 9, calories, "脂肪");
    
    std::cout << "╠════════════════════════════════════════════════╣" << std::endl;
    std::cout << "║  总热量: " << std::fixed << std::setprecision(1) 
              << calories << " kcal" << std::endl;
    std::cout << "║  蛋白质: " << protein << " g (" 
              << (int)((protein * 4 / calories) * 100) << "%)" << std::endl;
    std::cout << "║  碳水化合物: " << carbs << " g (" 
              << (int)((carbs * 4 / calories) * 100) << "%)" << std::endl;
    std::cout << "║  脂肪: " << fat << " g (" 
              << (int)((fat * 9 / calories) * 100) << "%)" << std::endl;
    std::cout << "╚════════════════════════════════════════════════╝" << std::endl;
}

std::string Utils::getInput(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return trim(input);
}

int Utils::getIntInput(const std::string& prompt) {
    while (true) {
        std::string input = getInput(prompt);
        try {
            return std::stoi(input);
        } catch (...) {
            std::cout << "输入无效，请输入一个整数。" << std::endl;
        }
    }
}

double Utils::getDoubleInput(const std::string& prompt) {
    while (true) {
        std::string input = getInput(prompt);
        try {
            return std::stod(input);
        } catch (...) {
            std::cout << "输入无效，请输入一个数字。" << std::endl;
        }
    }
}
