#pragma once
#include <string>

namespace PythonUtils {
    std::string execCommand(const std::string& cmd, int& exitCode);
    std::string findPythonPath();
}
