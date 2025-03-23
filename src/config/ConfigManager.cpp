#include "include/config/ConfigManager.h"
#include <fstream>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

#ifdef _WIN32
#include <Windows.h>
#include <ShlObj.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

ConfigManager::ConfigManager() {
    // Determine configuration file path
#ifdef _WIN32
    // Windows: %APPDATA%\PiChat\config.json
    char appDataPath[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_APPDATA, NULL, 0, appDataPath))) {
        configFilePath = std::string(appDataPath) + "\\PiChat\\config.json";
    }
    else {
        configFilePath = "config.json"; // Fallback
    }
#else
    // Unix: ~/.pichat/config.json
    const char* homeDir = getenv("HOME");
    if (!homeDir) {
        struct passwd* pwd = getpwuid(getuid());
        if (pwd) {
            homeDir = pwd->pw_dir;
        }
    }
    if (homeDir) {
        configFilePath = std::string(homeDir) + "/.pichat/config.json";
    }
    else {
        configFilePath = "config.json"; // Fallback
    }
#endif

    // Ensure config directory exists
    ensureConfigDirectory();

    // Set default API key if not already set
    auto config = loadConfig();
    if (config.find("api_key") == config.end()) {
        config["api_key"] = "sk-3ce8d607f9db409eaece8f07bce72c61";
        saveConfig(config);
    }
}

ConfigManager& ConfigManager::getInstance() {
    static ConfigManager instance;
    return instance;
}

std::string ConfigManager::getApiKey() const {
    std::lock_guard<std::mutex> lock(configMutex);
    auto config = loadConfig();
    auto it = config.find("api_key");
    return (it != config.end()) ? it->second : "";
}

bool ConfigManager::setApiKey(const std::string& apiKey) {
    std::lock_guard<std::mutex> lock(configMutex);
    auto config = loadConfig();
    config["api_key"] = apiKey;
    return saveConfig(config);
}

std::string ConfigManager::getSetting(const std::string& key, const std::string& defaultValue) const {
    std::lock_guard<std::mutex> lock(configMutex);
    auto config = loadConfig();
    auto it = config.find(key);
    return (it != config.end()) ? it->second : defaultValue;
}

bool ConfigManager::setSetting(const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> lock(configMutex);
    auto config = loadConfig();
    config[key] = value;
    return saveConfig(config);
}

std::string ConfigManager::getCurrentLanguage() const {
    return getSetting("language", "en");
}

bool ConfigManager::setCurrentLanguage(const std::string& languageCode) {
    return setSetting("language", languageCode);
}

void ConfigManager::ensureConfigDirectory() const {
    fs::path configDir = fs::path(configFilePath).parent_path();
    if (!fs::exists(configDir)) {
        try {
            fs::create_directories(configDir);
        }
        catch (const fs::filesystem_error& e) {
            std::cerr << "Error creating config directory: " << e.what() << std::endl;
        }
    }
}

std::unordered_map<std::string, std::string> ConfigManager::loadConfig() const {
    std::unordered_map<std::string, std::string> config;

    if (!fs::exists(configFilePath)) {
        return config;
    }

    try {
        std::ifstream file(configFilePath);
        std::string line;
        while (std::getline(file, line)) {
            // Simple key=value parsing
            size_t delimPos = line.find('=');
            if (delimPos != std::string::npos) {
                std::string key = line.substr(0, delimPos);
                std::string value = line.substr(delimPos + 1);
                config[key] = value;
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error loading config: " << e.what() << std::endl;
    }

    return config;
}

bool ConfigManager::saveConfig(const std::unordered_map<std::string, std::string>& config) const {
    try {
        ensureConfigDirectory();
        std::ofstream file(configFilePath);

        for (const auto& [key, value] : config) {
            file << key << "=" << value << std::endl;
        }

        return file.good();
    }
    catch (const std::exception& e) {
        std::cerr << "Error saving config: " << e.what() << std::endl;
        return false;
    }
}