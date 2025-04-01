// src/utils/IconGenerator.cpp
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

// 非常简单的PNG文件头 (1x1像素透明PNG)
const unsigned char PNG_HEADER[] = {
    0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D,
    0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
    0x08, 0x06, 0x00, 0x00, 0x00, 0x1F, 0x15, 0xC4, 0x89, 0x00, 0x00, 0x00,
    0x0A, 0x49, 0x44, 0x41, 0x54, 0x08, 0xD7, 0x63, 0x60, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x01, 0xE2, 0x21, 0xBC, 0x33, 0x00, 0x00, 0x00, 0x00, 0x49,
    0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82
};

// 生成简单的PNG图标文件
bool generateIcon(const std::string& filepath) {
    std::ofstream outfile(filepath, std::ios::binary);
    if (!outfile) {
        std::cerr << "Error: Cannot create file " << filepath << std::endl;
        return false;
    }

    outfile.write(reinterpret_cast<const char*>(PNG_HEADER), sizeof(PNG_HEADER));
    if (outfile.bad()) {
        std::cerr << "Error: Cannot write to file " << filepath << std::endl;
        return false;
    }

    outfile.close();
    std::cout << "Generated icon: " << filepath << std::endl;
    return true;
}

// 创建所需的图标文件
void createRequiredIcons() {
    // 确保图标目录存在
    std::string iconDir = "icons";
    fs::create_directories(iconDir);

    // 需要创建的图标文件列表
    std::vector<std::string> iconFiles = {
        "app_icon.png",
        "user.png",
        "bot.png",
        "send.png",
        "new_chat.png",
        "settings.png"
    };

    // 为每个图标生成文件
    for (const auto& icon : iconFiles) {
        std::string iconPath = iconDir + "/" + icon;
        if (!fs::exists(iconPath)) {
            generateIcon(iconPath);
        }
    }
}

// 在main.cpp中调用这个函数
