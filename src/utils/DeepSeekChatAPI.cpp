// src/utils/DeepSeekChatAPI.cpp
#include "include/utils/DeepSeekAPI.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

// Callback for handling HTTP response data
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp) {
    userp->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// Callback for handling streaming data with improved error handling
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
            catch (const json::parse_error&) {
                // Just ignore parse errors for streaming responses
            }
        }
    }

    return size * nmemb;
}

// Create JSON request body for DeepSeek API
static std::string createChatRequestBody(
    const std::vector<Message>& messages,
    const std::string& model,
    float temperature,
    int maxTokens,
    bool stream
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

// 实现 chatCompletion 函数
std::string chatCompletion(
    const std::string& apiKey,
    const std::vector<Message>& messages,
    const std::string& model,
    float temperature,
    int maxTokens
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

// 实现 streamingChatCompletion 函数
std::string streamingChatCompletion(
    const std::string& apiKey,
    const std::vector<Message>& messages,
    std::function<void(const std::string&)> callback,
    const std::string& model,
    float temperature,
    int maxTokens
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
