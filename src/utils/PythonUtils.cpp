#include "include/utils/PythonUtils.h"
#include <array>
#include <memory>
#include <fstream>

#ifdef _WIN32
#include <Windows.h>
#endif

namespace PythonUtils {
    std::string execCommand(const std::string& cmd, int& exitCode) {
        std::array<char, 512> buffer;
        std::string result;
        FILE* pipe = _popen(cmd.c_str(), "r");
        if (!pipe) {
            exitCode = -1;
            return "ERROR";
        }

        try {
            while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe) != nullptr) {
                result += buffer.data();
            }
        }
        catch (...) {
            _pclose(pipe);
            exitCode = -1;
            return "ERROR";
        }

        exitCode = _pclose(pipe);
        return result;
    }

    std::string findPythonPath() {
        int exitCode;

        // 检查Python路径是否可用
        if (std::ifstream("C:\\Users\\ryan\\111111111\\Scripts\\python.exe")) {
            return "C:\\Users\\ryan\\111111111\\Scripts\\python.exe";
        }

        // 尝试使用系统Python
        std::string result = execCommand("python --version", exitCode);
        if (exitCode == 0 && result.find("Python") != std::string::npos) {
            return "python";
        }

        return "";
    }
}
