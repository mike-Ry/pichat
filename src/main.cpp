#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <vector>
#include <functional>
#include <future>
#include <mutex>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#ifdef _WIN32
#include <Windows.h>
#else
#include <signal.h>
#include <unistd.h>
#endif

#include "include/config/ConfigManager.h"
#include "include/utils/ErrorHandler.h"
#include "include/cli/CLIManager.h"
#include "include/voice/VoiceManager.h"

using json = nlohmann::json;

// Global flag for service mode
bool g_running = true;

// Message struct for chat
struct Message {
    std::string role;    // "user" or "assistant"
    std::string content; // Message content
};

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

/**
 * @brief Callback for handling HTTP response data
 *
 * @param contents Pointer to the data
 * @param size Size of each data element
 * @param nmemb Number of data elements
 * @param userp Pointer to the user data
 * @return size_t Number of bytes processed
 */
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

/**
 * @brief Callback for handling streaming data with improved error handling
 *
 * @param contents Pointer to the data
 * @param size Size of each data element
 * @param nmemb Number of data elements
 * @param userp Pointer to the user data
 * @return size_t Number of bytes processed
 */
static size_t StreamingWriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    auto callback = reinterpret_cast<std::function<void(const std::string&)>*>(userp);
    std::string data((char*)contents, size * nmemb);

    // Each line starts with "data: " for SSE
    if (data.find("data: ") == 0) {
        data = data.substr(6); // Skip "data: "

        // Skip keep-alive lines and end markers
        if (data != "[DONE]" && !data.empty() && data != "\n") {
            try {
                // Strip any trailing characters that might corrupt the JSON
                size_t jsonEnd = data.find_last_of('}');
                if (jsonEnd != std::string::npos && jsonEnd < data.length() - 1) {
                    data = data.substr(0, jsonEnd + 1);
                }

                json responseJson = json::parse(data);

                // Extract content from the response
                if (responseJson.contains("choices") && responseJson["choices"].is_array() &&
                    responseJson["choices"].size() > 0 &&
                    responseJson["choices"][0].contains("delta") &&
                    responseJson["choices"][0]["delta"].contains("content")) {

                    std::string content = responseJson["choices"][0]["delta"]["content"];
                    (*callback)(content);
                }
            }
            catch (const json::parse_error& e) {
                // Just ignore parse errors for streaming responses
            }
        }
    }

    return size * nmemb;
}

/**
 * @brief Create JSON request body for DeepSeek API
 *
 * @param messages Vector of messages
 * @param model Model name (default: "deepseek-chat")
 * @param temperature Temperature for response generation (default: 0.7)
 * @param maxTokens Maximum number of tokens (default: 1000)
 * @param stream Whether to stream the response (default: false)
 * @return std::string JSON request body
 */
std::string createChatRequestBody(
    const std::vector<Message>& messages,
    const std::string& model = "deepseek-chat",
    float temperature = 0.7,
    int maxTokens = 1000,
    bool stream = false
) {
    json requestBody;

    // Convert messages to JSON format
    json messagesJson = json::array();
    for (const auto& message : messages) {
        messagesJson.push_back({
            {"role", message.role},
            {"content", message.content}
            });
    }

    requestBody["model"] = model;
    requestBody["messages"] = messagesJson;
    requestBody["temperature"] = temperature;
    requestBody["max_tokens"] = maxTokens;
    requestBody["stream"] = stream;

    return requestBody.dump();
}

/**
 * @brief Perform chat completion using DeepSeek API
 *
 * @param apiKey API key for authentication
 * @param messages Vector of messages
 * @param model Model name (default: "deepseek-chat")
 * @param temperature Temperature for response generation (default: 0.7)
 * @param maxTokens Maximum number of tokens (default: 1000)
 * @return std::string Response from the API
 */
std::string chatCompletion(
    const std::string& apiKey,
    const std::vector<Message>& messages,
    const std::string& model = "deepseek-chat",
    float temperature = 0.7,
    int maxTokens = 1000
) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        return "Error: Failed to initialize CURL";
    }

    std::string responseData;
    std::string requestBody = createChatRequestBody(messages, model, temperature, maxTokens, false);

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    std::string authHeader = "Authorization: Bearer " + apiKey;
    headers = curl_slist_append(headers, authHeader.c_str());

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.deepseek.com/v1/chat/completions");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestBody.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        return std::string("CURL error: ") + curl_easy_strerror(res);
    }

    try {
        json responseJson = json::parse(responseData);
        if (responseJson.contains("choices") && responseJson["choices"].is_array() &&
            responseJson["choices"].size() > 0 &&
            responseJson["choices"][0].contains("message") &&
            responseJson["choices"][0]["message"].contains("content")) {

            return responseJson["choices"][0]["message"]["content"];
        }
        else if (responseJson.contains("error")) {
            return "API Error: " + responseJson["error"]["message"].get<std::string>();
        }
        else {
            return "Error: Invalid response format\n" + responseData;
        }
    }
    catch (const json::parse_error& e) {
        return std::string("JSON parse error: ") + e.what() + "\nResponse: " + responseData;
    }
}

/**
 * @brief Perform streaming chat completion using DeepSeek API
 *
 * @param apiKey API key for authentication
 * @param messages Vector of messages
 * @param callback Function to call with each chunk of the response
 * @param model Model name (default: "deepseek-chat")
 * @param temperature Temperature for response generation (default: 0.7)
 * @param maxTokens Maximum number of tokens (default: 1000)
 * @return std::string Full response from the API
 */
std::string streamingChatCompletion(
    const std::string& apiKey,
    const std::vector<Message>& messages,
    std::function<void(const std::string&)> callback,
    const std::string& model = "deepseek-chat",
    float temperature = 0.7,
    int maxTokens = 1000
) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        return "Error: Failed to initialize CURL";
    }

    std::string requestBody = createChatRequestBody(messages, model, temperature, maxTokens, true);
    std::string fullResponse;

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    std::string authHeader = "Authorization: Bearer " + apiKey;
    headers = curl_slist_append(headers, authHeader.c_str());

    // Create a wrapper function that both updates the full response and calls the user callback
    auto wrappedCallback = [&fullResponse, callback](const std::string& chunk) {
        fullResponse += chunk;
        callback(chunk);
        };

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.deepseek.com/v1/chat/completions");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestBody.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, StreamingWriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &wrappedCallback);

    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        return std::string("CURL error: ") + curl_easy_strerror(res);
    }

    return fullResponse;
}

/**
 * @class ChatSession
 * @brief Manages conversation with DeepSeek API
 */
class ChatSession {
public:
    ChatSession() : apiKey("") {}

    /**
     * @brief Initialize the chat session with an API key
     * @param apiKey API key for authentication
     * @return true if initialization is successful
     */
    bool initialize(const std::string& apiKey) {
        this->apiKey = apiKey;
        return !apiKey.empty();
    }

    /**
     * @brief Send a message and get a response from the API
     * @param message Message to send
     * @return std::string Response from the API
     */
    std::string sendMessage(const std::string& message) {
        // Add user message to history
        history.push_back({ "user", message });

        // Get response from API
        std::string response = chatCompletion(apiKey, history);

        // Add assistant message to history
        history.push_back({ "assistant", response });

        return response;
    }

    /**
     * @brief Send a message and get a streaming response from the API
     * @param message Message to send
     * @param callback Function to call with each chunk of the response
     * @return std::string Full response from the API
     */
    std::string sendMessageStreaming(
        const std::string& message,
        std::function<void(const std::string&)> callback
    ) {
        // Add user message to history
        history.push_back({ "user", message });

        // Get streaming response from API
        std::string response = streamingChatCompletion(apiKey, history, callback);

        // Add assistant message to history
        history.push_back({ "assistant", response });

        return response;
    }

    /**
     * @brief Clear the chat history
     */
    void clearHistory() {
        history.clear();
    }

private:
    std::string apiKey;
    std::vector<Message> history;
    std::string model = "deepseek-chat";
};

/**
 * @brief Run the application in voice mode
 */
void runVoiceMode() {
    std::cout << "PiChat Voice Mode" << std::endl;
    std::cout << "Say 'exit' to quit, or use voice commands" << std::endl;

    ErrorHandler& errorHandler = ErrorHandler::getInstance();
    ConfigManager& configManager = ConfigManager::getInstance();

    // Get API key
    std::string apiKey = configManager.getApiKey();
    if (apiKey.empty()) {
        errorHandler.logError("API key not set. Use --set-key to configure.");
        std::cerr << "Error: API key not set. Use --set-key to configure." << std::endl;
        return;
    }

    // Initialize chat session
    ChatSession chatSession;
    if (!chatSession.initialize(apiKey)) {
        errorHandler.logError("Failed to initialize chat session. Check your API key.");
        std::cerr << "Error: Failed to initialize chat session. Check your API key." << std::endl;
        return;
    }

    // Initialize voice manager
    VoiceManager& voiceManager = VoiceManager::getInstance();
    if (!voiceManager.initialize()) {
        errorHandler.logError("Failed to initialize voice subsystem.");
        std::cerr << "Error: Failed to initialize voice subsystem." << std::endl;
        return;
    }

    // Set up voice callback
    voiceManager.startListening([&chatSession, &voiceManager](const std::string& text) {
        if (text == "exit") {
            voiceManager.stopListening();
            return;
        }

        std::cout << "You (voice): " << text << std::endl;
        std::cout << "PiChat: ";

        // Get API response
        std::string response = chatSession.sendMessage(text);
        std::cout << response << std::endl;

        // Speak the response
        voiceManager.speak(response);
        });

    // Wait for exit command
    std::string input;
    while (voiceManager.isListening()) {
        std::getline(std::cin, input);
        if (input == "exit") {
            voiceManager.stopListening();
            break;
        }
    }

    voiceManager.shutdown();
}

/**
 * @brief Run the application in service mode
 */
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

/**
 * @brief Run the application in interactive mode with DeepSeek API
 */
void runInteractive() {
    std::cout << "PiChat Interactive Mode" << std::endl;
    std::cout << "Type 'exit' to quit, 'clear' to clear chat history" << std::endl;

    ErrorHandler& errorHandler = ErrorHandler::getInstance();
    ConfigManager& configManager = ConfigManager::getInstance();

    // Get API key
    std::string apiKey = configManager.getApiKey();
    if (apiKey.empty()) {
        errorHandler.logError("API key not set. Use --set-key to configure.");
        std::cerr << "Error: API key not set. Use --set-key to configure." << std::endl;
        return;
    }

    // Initialize chat session
    ChatSession chatSession;
    if (!chatSession.initialize(apiKey)) {
        errorHandler.logError("Failed to initialize chat session. Check your API key.");
        std::cerr << "Error: Failed to initialize chat session. Check your API key." << std::endl;
        return;
    }

    // Basic interactive loop
    std::string input;
    while (true) {
        std::cout << "\nYou: ";
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        }

        if (input == "clear") {
            chatSession.clearHistory();
            std::cout << "Chat history cleared." << std::endl;
            continue;
        }

        if (!input.empty()) {
            std::cout << "PiChat: ";

            // Use non-streaming API for main responses
            std::string response = chatSession.sendMessage(input);
            std::cout << response << std::endl;

            // Alternatively, use streaming API for real-time responses
            /*
            chatSession.sendMessageStreaming(input, [](const std::string& chunk) {
                std::cout << chunk << std::flush;
            });
            std::cout << std::endl;
            */
        }
    }
}

/**
 * @brief Main function
 *
 * @param argc Argument count
 * @param argv Argument vector
 * @return int Exit code
 */
int main(int argc, char* argv[]) {
    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_ALL);

    // Initialize error handler
    ErrorHandler& errorHandler = ErrorHandler::getInstance();

    // Initialize ConfigManager
    ConfigManager& configManager = ConfigManager::getInstance();

    // Check if we're running in voice mode
    if (argc > 1 && std::string(argv[1]) == "--voice") {
        runVoiceMode();
        curl_global_cleanup();
        return 0;
    }

    // Check if we're running in service mode
    if (argc > 1 && std::string(argv[1]) == "--service") {
        runService();
        curl_global_cleanup();
        return 0;
    }

    // Check if we're running in interactive mode
    if (argc > 1 && std::string(argv[1]) == "--interactive") {
        runInteractive();
        curl_global_cleanup();
        return 0;
    }

    // Process CLI commands
    CLIManager& cliManager = CLIManager::getInstance();
    int result = cliManager.parseAndExecute(argc, argv);

    // Clean up libcurl
    curl_global_cleanup();

    return result;
}