# src/CMakeLists.txt

# 添加更多Qt搜索路径
set(CMAKE_PREFIX_PATH 
    "C:/vcpkg/installed/x64-windows"
    "C:/vcpkg/installed/x64-windows/lib/cmake/Qt5"
    "C:/vcpkg/installed/x64-windows/lib/cmake/Qt5Widgets"
    "C:/vcpkg/installed/x64-windows/lib/cmake/Qt5Core"
    "C:/vcpkg/installed/x64-windows/lib/cmake/Qt5Gui"
    ${CMAKE_PREFIX_PATH}
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 关闭自动MOC，我们将手动处理
set(CMAKE_AUTOMOC OFF)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 添加vcpkg包含路径和库路径
include_directories(
    "C:/vcpkg/installed/x64-windows/include"
    "C:/vcpkg/installed/x64-windows/include/qt5"
    "C:/vcpkg/installed/x64-windows/include/qt5/QtWidgets"
    "C:/vcpkg/installed/x64-windows/include/qt5/QtCore"
    "C:/vcpkg/installed/x64-windows/include/qt5/QtGui"
    ${CMAKE_BINARY_DIR}  # 添加构建目录，确保能找到自动生成的头文件
)

link_directories(
    "C:/vcpkg/installed/x64-windows/lib"
    "C:/vcpkg/installed/x64-windows/bin"  # 添加bin目录，DLL可能在这里
)

# Find required packages
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Qt5 COMPONENTS Widgets Core Gui REQUIRED)

# 显示Qt库位置，帮助调试
message(STATUS "Qt5_DIR: ${Qt5_DIR}")
message(STATUS "Qt5Widgets_INCLUDE_DIRS: ${Qt5Widgets_INCLUDE_DIRS}")
message(STATUS "Qt5Core_LIBRARIES: ${Qt5Core_LIBRARIES}")
message(STATUS "Qt5Widgets_LIBRARIES: ${Qt5Widgets_LIBRARIES}")
message(STATUS "Qt5Gui_LIBRARIES: ${Qt5Gui_LIBRARIES}")

# 确保common目录存在
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../include/common)

# 确保源文件目录结构存在
file(MAKE_DIRECTORY 
    ${CMAKE_CURRENT_SOURCE_DIR}/config
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/cli
    ${CMAKE_CURRENT_SOURCE_DIR}/voice
    ${CMAKE_CURRENT_SOURCE_DIR}/gui
)

# 需要被MOC处理的头文件
set(MOC_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/gui/MainWindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/gui/SettingsDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/utils/DeepSeekAPI.h  # 添加DeepSeekAPI.h以处理Q_OBJECT宏
)

# 创建MOC输出目录
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/moc)

# 为每个头文件手动生成MOC源文件
foreach(HEADER ${MOC_HEADERS})
    # 获取文件名(不带路径和扩展名)
    get_filename_component(HEADER_NAME ${HEADER} NAME_WE)
    
    # 设置MOC输出文件路径
    set(MOC_OUTPUT_FILE ${CMAKE_BINARY_DIR}/moc/moc_${HEADER_NAME}.cpp)
    
    # 添加自定义命令来生成MOC文件
    add_custom_command(
        OUTPUT ${MOC_OUTPUT_FILE}
        COMMAND Qt5::moc
        ARGS -o ${MOC_OUTPUT_FILE} ${HEADER}
        DEPENDS ${HEADER}
        VERBATIM
    )
    
    list(APPEND MOC_SOURCES ${MOC_OUTPUT_FILE})
endforeach()

# UI files
set(UI_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/MainWindow.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/SettingsDialog.ui
)

# 处理UI文件
qt5_wrap_ui(UI_HEADERS ${UI_FILES})

# 核心源文件
set(CORE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/config/ConfigManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/ErrorHandler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/DeepSeekAPI.cpp  # DeepSeekAPI.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/DeepSeekChatAPI.cpp  # 添加新的DeepSeekChatAPI.cpp
)

# CLI源文件
set(CLI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/cli/CLIManager.cpp
)

# 语音功能源文件
set(VOICE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/voice/VoiceManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/voice/SpeechRecognizer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/voice/TextToSpeech.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/voice/CommandProcessor.cpp
)

# GUI源文件
set(GUI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/MainWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/gui/SettingsDialog.cpp  # 确保SettingsDialog.cpp已被包含
)

# 所有源文件
set(SOURCES
    ${CORE_SOURCES}
    ${CLI_SOURCES}
    ${VOICE_SOURCES}
    ${GUI_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${MOC_SOURCES}  # 特别添加MOC生成的源文件
)

# 创建可执行文件
add_executable(pichat 
    ${SOURCES} 
    ${UI_HEADERS}
)

# 包含目录
target_include_directories(pichat PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_BINARY_DIR}  # 包含构建目录
    ${CMAKE_BINARY_DIR}/moc  # 包含MOC输出目录
    ${CMAKE_BINARY_DIR}/src  # UI文件可能生成在这里
    "C:/vcpkg/installed/x64-windows/include"
    "C:/vcpkg/installed/x64-windows/include/qt5"
)

# 链接所有必要的库
target_link_libraries(pichat PRIVATE 
    Qt5::Widgets  # 优先链接Qt库
    Qt5::Core
    Qt5::Gui
    CURL::libcurl 
    nlohmann_json::nlohmann_json
)

# Windows特定库（语音识别需要）
if(WIN32)
    target_link_libraries(pichat PRIVATE 
        ole32
        oleaut32
    )
endif()

# MSVC编译器选项
if(MSVC)
    # 使用多线程DLL运行时
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    
    # 禁用某些警告
    add_compile_options(/wd4251 /wd4275 /wd4819)
    
    # Unicode支持
    add_definitions(-DUNICODE -D_UNICODE)
endif()

# 复制Qt DLL和插件到输出目录
if(WIN32)
    add_custom_command(TARGET pichat POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:Qt5::Widgets>
            $<TARGET_FILE:Qt5::Core>
            $<TARGET_FILE:Qt5::Gui>
            $<TARGET_FILE_DIR:pichat>
    )
    add_custom_command(TARGET pichat POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            $<TARGET_FILE_DIR:pichat>/platforms
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "C:/vcpkg/installed/x64-windows/plugins/platforms/qwindows.dll"
            $<TARGET_FILE_DIR:pichat>/platforms/
    )
    
    # 复制Qt样式插件到输出目录
    add_custom_command(TARGET pichat POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
            $<TARGET_FILE_DIR:pichat>/styles
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "C:/vcpkg/installed/x64-windows/plugins/styles/qwindowsvistastyle.dll"
            $<TARGET_FILE_DIR:pichat>/styles/
    )
endif()
