
#include <iostream>
#include "include/config/ConfigManager.h"
#include "include/utils/ErrorHandler.h"
#include "include/cli/CLIManager.h"

int main(int argc, char* argv[]) {
    std::cout << "PiChat CLI Test" << std::endl;
    
    // ��ʼ����������
    ErrorHandler& errorHandler = ErrorHandler::getInstance();
    
    // ��ʼ�����ù�����
    ConfigManager& configManager = ConfigManager::getInstance();
    
    // ����������в���
    if (argc > 1) {
        // ���������в���
        CLIManager& cliManager = CLIManager::getInstance();
        return cliManager.parseAndExecute(argc, argv);
    }

    std::cout << "No command specified. Use --help for usage information." << std::endl;
    return 0;
}
