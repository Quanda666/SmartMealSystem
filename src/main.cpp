#include "WebServer.h"
#include "User.h"
#include "Food.h"
#include "Meal.h"
#include "Database.h"
#include "RecommendationEngine.h"
#include <iostream>
#include <thread>
#include <memory>

// 全局数据库实例
std::unique_ptr<Database> g_db;
std::unique_ptr<RecommendationEngine> g_engine;

class WebMealRecommendationServer : public SimpleWebServer {
private:
    Database& db;
    RecommendationEngine& engine;

public:
    WebMealRecommendationServer(int port, Database& database, RecommendationEngine& engine) 
        : SimpleWebServer(port), db(database), engine(engine) {}

private:
    std::string handleLogin(const std::string& request) override {
        // 简单的登录验证逻辑
        try {
            // 从请求中提取用户名和密码（这里简化处理）
            auto users = db.getAllUsers();
            bool found = false;
            
            // 这里应该解析JSON请求体
            // 为了演示，假设有用户名和密码
            if (request.find("username") != std::string::npos && 
                request.find("password") != std::string::npos) {
                found = true; // 模拟登录成功
            }
            
            if (found) {
                return "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: 43\r\n\r\n{\"success\":true,\"message\":\"登录成功\"}";
            } else {
                return "HTTP/1.1 401 OK\r\nContent-Type: application/json\r\nContent-Length: 44\r\n\r\n{\"success\":false,\"message\":\"登录失败\"}";
            }
        } catch (...) {
            return "HTTP/1.1 500 OK\r\nContent-Type: application/json\r\nContent-Length: 46\r\n\r\n{\"success\":false,\"message\":\"服务器错误\"}";
        }
    }
    
    std::string handleRegister(const std::string& request) override {
        // 注册逻辑
        try {
            // 这里应该解析JSON请求体并创建新用户
            return "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: 43\r\n\r\n{\"success\":true,\"message\":\"注册成功\"}";
        } catch (...) {
            return "HTTP/1.1 500 OK\r\nContent-Type: application/json\r\nContent-Length: 46\r\n\r\n{\"success\":false,\"message\":\"服务器错误\"}";
        }
    }
};

int main() {
    std::cout << "============================================" << std::endl;
    std::cout << "                                      " << std::endl;
    std::cout << "          智能配餐推荐系统 Web版            " << std::endl;
    std::cout << "                                      " << std::endl;
    std::cout << "     Intelligent Meal Recommendation System " << std::endl;
    std::cout << "                                      " << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << std::endl;
    
    try {
        // 初始化数据库
        auto db_ptr = std::make_unique<Database>("data/users.txt", "data/foods.txt", "data/meals.txt");
        
        if (!db_ptr->loadFoods()) {
            std::cout << "首次运行，初始化数据..." << std::endl;
            db_ptr->initializeSampleData();
        }
        db_ptr->loadUsers();
        db_ptr->loadMeals();
        
        // 初始化推荐引擎
        auto engine_ptr = std::make_unique<RecommendationEngine>();
        engine_ptr->setFoodDatabase(db_ptr->getAllFoods());
        
        std::map<int, std::vector<Meal>> allHistory;
        for (const auto& meal : db_ptr->getAllMeals()) {
            allHistory[meal.getUserId()].push_back(meal);
        }
        engine_ptr->loadHistory(allHistory);
        
        // 保存全局实例
        g_db = std::move(db_ptr);
        g_engine = std::move(engine_ptr);
        
        std::cout << "数据加载完成！" << std::endl;
        std::cout << "正在启动Web服务器..." << std::endl;
        std::cout << "请在浏览器中访问: http://localhost:8080" << std::endl;
        std::cout << "按 Ctrl+C 停止服务器" << std::endl;
        std::cout << std::endl;
        
        // 启动Web服务器
        WebMealRecommendationServer server(8080, *g_db, *g_engine);
        server.start();
        
    } catch (const std::exception& e) {
        std::cerr << "服务器启动失败: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}