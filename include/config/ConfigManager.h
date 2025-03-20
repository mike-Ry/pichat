#pragma once

#include <string>
#include <unordered_map>
#include <mutex>

/**
 * @class ConfigManager
 * @brief Manages PiChat configuration settings
 *
 * This class handles loading, saving, and accessing configuration settings
 * such as API keys, language preferences, and other application settings.
 */
class ConfigManager {
public:
    /**
     * @brief Get the singleton instance of ConfigManager
     * @return Reference to the ConfigManager instance
     */
    static ConfigManager& getInstance();

    /**
     * @brief Get API key for DeepSeek API
     * @return API key string
     */
    std::string getApiKey() const;

    /**
     * @brief Set API key for DeepSeek API
     * @param apiKey The API key to set
     * @return true if successful, false otherwise
     */
    bool setApiKey(const std::string& apiKey);

    /**
     * @brief Get a configuration setting
     * @param key Setting name
     * @param defaultValue Default value if setting not found
     * @return Setting value or defaultValue if not found
     */
    std::string getSetting(const std::string& key, const std::string& defaultValue = "") const;

    /**
     * @brief Set a configuration setting
     * @param key Setting name
     * @param value Setting value
     * @return true if successful, false otherwise
     */
    bool setSetting(const std::string& key, const std::string& value);

    /**
     * @brief Get the current language setting
     * @return Language code (e.g., "en", "zh")
     */
    std::string getCurrentLanguage() const;

    /**
     * @brief Set the current language
     * @param languageCode Language code (e.g., "en", "zh")
     * @return true if successful, false otherwise
     */
    bool setCurrentLanguage(const std::string& languageCode);

private:
    // Private constructor for singleton pattern
    ConfigManager();

    // Delete copy constructor and assignment operator
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    // Configuration file path
    std::string configFilePath;

    // Mutex for thread safety
    mutable std::mutex configMutex;

    // Helper methods
    void ensureConfigDirectory() const;
    std::unordered_map<std::string, std::string> loadConfig() const;
    bool saveConfig(const std::unordered_map<std::string, std::string>& config) const;
};