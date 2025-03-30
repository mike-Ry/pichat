# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

# Core library (Config, Error Handling)
add_library(pichat_core
    ${CMAKE_CURRENT_SOURCE_DIR}/config/ConfigManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/utils/ErrorHandler.cpp
)

# CLI library
add_library(pichat_cli
    ${CMAKE_CURRENT_SOURCE_DIR}/cli/CLIManager.cpp
)

# Include directories
target_include_directories(pichat_core PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_include_directories(pichat_cli PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# Link dependencies
target_link_libraries(pichat_cli PRIVATE pichat_core)
target_link_libraries(pichat_core PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)


# Voice library
add_library(pichat_voice
    voice/VoiceManager.cpp
    voice/SpeechRecognizer.cpp
    voice/TextToSpeech.cpp
    voice/CommandProcessor.cpp
)

# Include directories
target_include_directories(pichat_voice PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# Link dependencies
target_link_libraries(pichat_voice PRIVATE 
    pichat_core
)

# Windows specific libraries for speech
if(WIN32)
    target_link_libraries(pichat_voice PRIVATE 
        ole32
        oleaut32
    )
endif()

# Define a macro to indicate we're using the simplified voice implementation
target_compile_definitions(pichat_voice PRIVATE SIMPLIFIED_VOICE_IMPL=1)