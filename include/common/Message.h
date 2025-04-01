#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

// ������Ϣ�ṹ��
struct Message {
    std::string role;    // "user" or "assistant"
    std::string content; // Message content

    // Default constructor
    Message() = default;

    // Constructor with parameters
    Message(const std::string& r, const std::string& c)
        : role(r), content(c) {
    }
};

#endif // MESSAGE_H
