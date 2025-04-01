
#include <iostream>
#include "include/config/ConfigManager.h"
#include "include/utils/ErrorHandler.h"
#include "include/cli/CLIManager.h"

int main(int argc, char* argv[]) {
    std::cout << "PiChat CLI Test" << std::endl;
    
    // 初始化错误处理器
    ErrorHandler& errorHandler = ErrorHandler::getInstance();
    
    // 初始化配置管理器
    ConfigManager& configManager = ConfigManager::getInstance();
    
    // 如果有命令行参数
    if (argc > 1) {
        // 处理命令行参数
        CLIManager& cliManager = CLIManager::getInstance();
        return cliManager.parseAndExecute(argc, argv);
    }

    std::cout << "No command specified. Use --help for usage information." << std::endl;
    return 0;
}
