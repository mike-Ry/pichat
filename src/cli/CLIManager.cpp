#include "cli/CLIManager.h"
#include "config/ConfigManager.h"
#include "utils/ErrorHandler.h"
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

#ifdef _WIN32
#include <Windows.h>
#include <tchar.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#endif

namespace fs = std::filesystem;

// Process ID file
const std::string PID_FILE = [] {
#ifdef _WIN32
    // Windows path
    char appData[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appData))) {
        return std::string(appData) + "\\PiChat\\pichat.pid";
    }
    return "pichat.pid";  // Fallback
#else
    // Unix path
    std::string homeDir = getenv("HOME") ? getenv("HOME") : "";
    return homeDir + "/.pichat/pichat.pid";
#endif
    }();

CLIManager::CLIManager() : appName("pichat") {
    // Register default commands
    registerCommand("--help", "Show this help message",
        [this](const std::vector<std::string>& args) {
            printUsage();
            return 0;
        });

    registerCommand("--start", "Start PiChat service",
        [this](const std::vector<std::string>& args) {
            // Check if already running
            if (isServiceRunning()) {
                std::cout << "PiChat is already running" << std::endl;
                return 0;
            }

            // Check if API key is set
            ConfigManager& config = ConfigManager::getInstance();
            std::string apiKey = config.getApiKey();
            if (apiKey.empty()) {
                std::cerr << "Error: API key not set. Use --set-key to configure." << std::endl;
                return 1;
            }

            // Start service
            if (startService()) {
                std::cout << "PiChat is running" << std::endl;
                return 0;
            }
            else {
                std::cerr << "Failed to start PiChat" << std::endl;
                return 1;
            }
        });

    registerCommand("--stop", "Stop PiChat service",
        [this](const std::vector<std::string>& args) {
            // Check if running
            if (!isServiceRunning()) {
                std::cout << "PiChat is not running" << std::endl;
                return 0;
            }

            // Stop service
            if (stopService()) {
                std::cout << "PiChat stopped" << std::endl;
                return 0;
            }
            else {
                std::cerr << "Failed to stop PiChat" << std::endl;
                return 1;
            }
        });

    registerCommand("--status", "Check PiChat service status",
        [this](const std::vector<std::string>& args) {
            if (isServiceRunning()) {
                std::cout << "PiChat is running" << std::endl;
            }
            else {
                std::cout << "PiChat is not running" << std::endl;
            }
            return 0;
        });

    registerCommand("--set-key", "Set DeepSeek API key",
        [this](const std::vector<std::string>& args) {
            if (args.size() < 1) {
                std::cerr << "Error: API key not provided" << std::endl;
                std::cout << "Usage: " << appName << " --set-key <API_KEY>" << std::endl;
                return 1;
            }

            std::string apiKey = args[0];
            ConfigManager& config = ConfigManager::getInstance();
            if (config.setApiKey(apiKey)) {
                std::cout << "API key updated successfully" << std::endl;
                return 0;
            }
            else {
                std::cerr << "Failed to update API key" << std::endl;
                return 1;
            }
        });
}

CLIManager& CLIManager::getInstance() {
    static CLIManager instance;
    return instance;
}

int CLIManager::parseAndExecute(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage();
        return 1;
    }

    std::string command = argv[1];
    auto it = commands.find(command);

    if (it == commands.end()) {
        return handleUnknownCommand(command);
    }

    // Extract arguments
    std::vector<std::string> args;
    for (int i = 2; i < argc; i++) {
        args.push_back(argv[i]);
    }

    // Execute command
    return it->second.handler(args);
}

void CLIManager::registerCommand(const std::string& name,
    const std::string& description,
    std::function<int(const std::vector<std::string>&)> handler) {
    Command command{ name, description, handler };
    commands[name] = command;
}

void CLIManager::printUsage() const {
    std::cout << "Usage: " << appName << " <command> [options]" << std::endl;
    std::cout << "Commands:" << std::endl;

    // Print all commands
    for (const auto& [name, command] : commands) {
        std::cout << "  " << name << "\t" << command.description << std::endl;
    }
}

std::string CLIManager::getAppName() const {
    return appName;
}

void CLIManager::setAppName(const std::string& name) {
    appName = name;
}

int CLIManager::handleUnknownCommand(const std::string& command) {
    std::cerr << "Unknown command: " << command << std::endl;
    printUsage();
    return 1;
}

// Helper methods for service management
bool CLIManager::isServiceRunning() {
    if (!fs::exists(PID_FILE)) {
        return false;
    }

    // Read PID from file
    std::ifstream pidFile(PID_FILE);
    int pid;
    pidFile >> pid;

    if (pid <= 0) {
        fs::remove(PID_FILE);
        return false;
    }

    // Check if process exists
#ifdef _WIN32
    // Windows process check
    HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
    if (process) {
        CloseHandle(process);
        return true;
    }
    else {
        // Process doesn't exist, remove PID file
        fs::remove(PID_FILE);
        return false;
    }
#else
    // Unix process check
    if (kill(pid, 0) == 0) {
        return true;
    }
    else {
        // Process doesn't exist, remove PID file
        fs::remove(PID_FILE);
        return false;
    }
#endif
}

bool CLIManager::startService() {
#ifdef _WIN32
    // Windows service start
    // Get path to executable
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);

    // Create process
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Command line: "<path>\pichat.exe --service"
    std::string cmdLine = std::string(exePath) + " --service";

    // Create process
    if (!CreateProcessA(NULL, (LPSTR)cmdLine.c_str(), NULL, NULL, FALSE,
        CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        std::cerr << "Error: Failed to create process, error code: " << GetLastError() << std::endl;
        return false;
    }

    // Write PID to file
    std::string pidDir = fs::path(PID_FILE).parent_path().string();
    if (!fs::exists(pidDir)) {
        fs::create_directories(pidDir);
    }

    std::ofstream pidFile(PID_FILE);
    pidFile << pi.dwProcessId;

    // Close handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return true;
#else
    // Unix service start
    // Get path to executable
    char exePath[PATH_MAX];
    if (readlink("/proc/self/exe", exePath, sizeof(exePath) - 1) == -1) {
        std::cerr << "Error: Could not determine executable path" << std::endl;
        return false;
    }

    // Fork process
    pid_t pid = fork();

    if (pid < 0) {
        // Fork failed
        std::cerr << "Error: Failed to fork process" << std::endl;
        return false;
    }
    else if (pid > 0) {
        // Parent process - write PID to file
        std::string pidDir = fs::path(PID_FILE).parent_path().string();
        if (!fs::exists(pidDir)) {
            fs::create_directories(pidDir);
        }

        std::ofstream pidFile(PID_FILE);
        pidFile << pid;
        return true;
    }
    else {
        // Child process - become daemon
        setsid();

        // Close standard file descriptors
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        // Redirect to /dev/null
        int fd = open("/dev/null", O_RDWR);
        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);

        // Execute service
        execl(exePath, "pichat", "--service", NULL);

        // If exec fails, exit
        exit(1);
    }

    return false;
#endif
}

bool CLIManager::stopService() {
    if (!fs::exists(PID_FILE)) {
        return true;
    }

    // Read PID from file
    std::ifstream pidFile(PID_FILE);
    int pid;
    pidFile >> pid;

    if (pid <= 0) {
        fs::remove(PID_FILE);
        return true;
    }

#ifdef _WIN32
    // Windows process termination
    HANDLE process = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (process) {
        if (TerminateProcess(process, 0)) {
            CloseHandle(process);
            fs::remove(PID_FILE);
            return true;
        }
        else {
            CloseHandle(process);
            std::cerr << "Error: Failed to terminate process, error code: " << GetLastError() << std::endl;
            return false;
        }
    }
    else {
        // Process doesn't exist
        fs::remove(PID_FILE);
        return true;
    }
#else
    // Unix process termination
    // Send SIGTERM
    if (kill(pid, SIGTERM) == 0) {
        // Wait for process to terminate
        int retry = 10;
        while (retry > 0 && kill(pid, 0) == 0) {
            usleep(500000); // 0.5 seconds
            retry--;
        }

        // If process still running, force kill
        if (retry == 0 && kill(pid, 0) == 0) {
            kill(pid, SIGKILL);
        }

        fs::remove(PID_FILE);
        return true;
    }
    else {
        // Process doesn't exist
        fs::remove(PID_FILE);
        return true;
    }
#endif
}