#include "../include/Food.h"
#include <iostream>
#include <sstream>
#include <iomanip>

Food::Food() : id(0), name(""), calories(0), protein(0), 
               carbohydrates(0), fat(0), fiber(0), category("") {}

Food::Food(int id, const std::string& name, double cal, double prot, 
           double carb, double fat, double fiber, 
           const std::set<std::string>& tags, const std::string& category)
    : id(id), name(name), calories(cal), protein(prot), 
      carbohydrates(carb), fat(fat), fiber(fiber), 
      tags(tags), category(category) {}

void Food::addTag(const std::string& tag) {
    tags.insert(tag);
}

bool Food::hasTag(const std::string& tag) const {
    return tags.find(tag) != tags.end();
}

void Food::displayInfo() const {
    std::cout << "========================================" << std::endl;
    std::cout << "  Food: " << name << " (ID: " << id << ")" << std::endl;
    std::cout << "  Category: " << category << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << "  Nutrition (per 100g):" << std::endl;
    std::cout << "    Calories:  " << std::fixed << std::setprecision(1) 
              << calories << " kcal" << std::endl;
    std::cout << "    Protein:   " << protein << " g" << std::endl;
    std::cout << "    Carbs:     " << carbohydrates << " g" << std::endl;
    std::cout << "    Fat:       " << fat << " g" << std::endl;
    std::cout << "    Fiber:     " << fiber << " g" << std::endl;
    
    if (!tags.empty()) {
        std::cout << "  Tags: ";
        bool first = true;
        for (const auto& tag : tags) {
            if (!first) std::cout << ", ";
            std::cout << tag;
            first = false;
        }
        std::cout << std::endl;
    }
    std::cout << "========================================" << std::endl;
}

std::string Food::toString() const {
    std::stringstream ss;
    ss << id << "|" << name << "|" << calories << "|" << protein << "|"
       << carbohydrates << "|" << fat << "|" << fiber << "|";
    
    bool first = true;
    for (const auto& tag : tags) {
        if (!first) ss << ",";
        ss << tag;
        first = false;
    }
    ss << "|" << category;
    
    return ss.str();
}