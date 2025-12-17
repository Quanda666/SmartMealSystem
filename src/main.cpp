#include "../include/WebServer.h"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <cstdio>
#endif

static void ensureConsoleVisibleInDebug() {
#ifdef _WIN32
#ifdef _DEBUG
    if (GetConsoleWindow() != NULL) {
        return;
    }

    if (!AttachConsole(ATTACH_PARENT_PROCESS)) {
        AllocConsole();
    }

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    FILE* fp = nullptr;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);
#endif
#endif
}

int main() {
    ensureConsoleVisibleInDebug();

    WebServer server(8000, "www");
    server.start();
    return 0;
}
