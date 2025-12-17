#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <atomic>
#include <string>

class SimpleWebServer {
private:
    int port;
    SOCKET server_socket;
    std::atomic<bool> running;

public:
    SimpleWebServer(int port = 8080) : port(port), server_socket(INVALID_SOCKET), running(false) {}
    
    ~SimpleWebServer() {
        cleanup();
    }
    
    void start();
    void stop();
    
private:
    bool initialize();
    void run();
    void handleClient(SOCKET client_socket);
    std::string handleHttpRequest(const std::string& request);
    std::string serveIndexPage();
    std::string handleLogin(const std::string& request);
    std::string handleRegister(const std::string& request);
    void cleanup();
};

#endif