#ifndef FOOD_H
#define FOOD_H

#include <string>
#include <vector>
#include <set>

class Food {
private:
    int id;
    std::string name;
    double calories;          // calories (kcal)
    double protein;           // protein (g)
    double carbohydrates;     // carbohydrates (g)
    double fat;               // fat (g)
    double fiber;             // fiber (g)
    std::set<std::string> tags; // taste tags: spicy, sweet, salty, sour, etc.
    std::string category;     // category: staple food, vegetables, meat, fruit, etc.

public:
    Food();
    Food(int id, const std::string& name, double cal, double prot, 
         double carb, double fat, double fiber, 
         const std::set<std::string>& tags, const std::string& category);

    // Getters
    int getId() const { return id; }
    std::string getName() const { return name; }
    double getCalories() const { return calories; }
    double getProtein() const { return protein; }
    double getCarbohydrates() const { return carbohydrates; }
    double getFat() const { return fat; }
    double getFiber() const { return fiber; }
    std::set<std::string> getTags() const { return tags; }
    std::string getCategory() const { return category; }

    // Setters
    void setId(int id) { this->id = id; }
    void setName(const std::string& name) { this->name = name; }
    void setCalories(double cal) { this->calories = cal; }
    void setProtein(double prot) { this->protein = prot; }
    void setCarbohydrates(double carb) { this->carbohydrates = carb; }
    void setFat(double fat) { this->fat = fat; }
    void setFiber(double fiber) { this->fiber = fiber; }
    void setTags(const std::set<std::string>& tags) { this->tags = tags; }
    void setCategory(const std::string& cat) { this->category = cat; }

    void addTag(const std::string& tag);
    bool hasTag(const std::string& tag) const;
    
    void displayInfo() const;
    std::string toString() const;
};

#endif
