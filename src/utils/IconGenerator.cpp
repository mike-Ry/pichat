// src/utils/IconGenerator.cpp
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

// �ǳ��򵥵�PNG�ļ�ͷ (1x1����͸��PNG)
const unsigned char PNG_HEADER[] = {
    0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D,
    0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
    0x08, 0x06, 0x00, 0x00, 0x00, 0x1F, 0x15, 0xC4, 0x89, 0x00, 0x00, 0x00,
    0x0A, 0x49, 0x44, 0x41, 0x54, 0x08, 0xD7, 0x63, 0x60, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x01, 0xE2, 0x21, 0xBC, 0x33, 0x00, 0x00, 0x00, 0x00, 0x49,
    0x45, 0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82
};

// ���ɼ򵥵�PNGͼ���ļ�
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

// ���������ͼ���ļ�
void createRequiredIcons() {
    // ȷ��ͼ��Ŀ¼����
    std::string iconDir = "icons";
    fs::create_directories(iconDir);

    // ��Ҫ������ͼ���ļ��б�
    std::vector<std::string> iconFiles = {
        "app_icon.png",
        "user.png",
        "bot.png",
        "send.png",
        "new_chat.png",
        "settings.png"
    };

    // Ϊÿ��ͼ�������ļ�
    for (const auto& icon : iconFiles) {
        std::string iconPath = iconDir + "/" + icon;
        if (!fs::exists(iconPath)) {
            generateIcon(iconPath);
        }
    }
}

// ��main.cpp�е����������
