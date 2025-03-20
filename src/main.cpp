#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <Windows.h>
#else
#include <signal.h>
#include <unistd.h>
#endif

#include "config/ConfigManager.h"
#include "utils/ErrorHandler.h"
#include "cli/CLIManager.h"

// Global flag for service mode
bool g_running = true;

// Signal handler for service mode
#ifdef _WIN32
BOOL WINAPI consoleHandler(DWORD signal) {
    if (signal == CTRL_C_EVENT || signal == CTRL_BREAK_EVENT) {
        g_running = false;
        return TRUE;
    }
    return FALSE;
}
#else
void signalHandler(int signum) {
    g_running = false;
}
#endif

// Service mode
void runService() {
    // Setup signal handling
#ifdef _WIN32
    SetConsoleCtrlHandler(consoleHandler, TRUE);
#else
    signal(SIGTERM, signalHandler);
    signal(SIGINT, signalHandler);
#endif

    std::cout << "PiChat service started" << std::endl;

    // Main service loop
    while (g_running) {
        // In a real application, this would handle incoming requests
        // This is just a placeholder
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "PiChat service stopped" << std::endl;
}

// Interactive mode
void runInteractive() {
    std::cout << "PiChat Interactive Mode" << std::endl;
    std::cout << "Type 'exit' to quit" << std::endl;

    // Basic interactive loop - to be replaced with ChatSession
    std::string input;
    while (true) {
        std::cout << "\nYou: ";
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        }

        if (!input.empty()) {
            std::cout << "PiChat: Sorry, chat functionality is not yet implemented." << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    // Initialize error handler
    ErrorHandler& errorHandler = ErrorHandler::getInstance();

    // Check if we're running in service mode
    if (argc > 1 && std::string(argv[1]) == "--service") {
        runService();
        return 0;
    }

    // Check if we're running in interactive mode
    if (argc > 1 && std::string(argv[1]) == "--interactive") {
        runInteractive();
        return 0;
    }

    // Process CLI commands
    CLIManager& cliManager = CLIManager::getInstance();
    return cliManager.parseAndExecute(argc, argv);
}