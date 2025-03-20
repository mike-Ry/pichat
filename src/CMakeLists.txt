# Core library (Config, Error Handling)
add_library(pichat_core
    config/ConfigManager.cpp
    utils/ErrorHandler.cpp
)

# CLI library
add_library(pichat_cli
    cli/CLIManager.cpp
)

# implement these when we created following files:
# API library
# add_library(pichat_api
#    api/DeepSeekAPI.cpp
# )

# Chat library
# add_library(pichat_chat
#    chat/ChatSession.cpp
# )

# Voice library
# add_library(pichat_voice
#    voice/VoiceManager.cpp
# )

# GUI library
# add_library(pichat_gui
#    gui/GUIManager.cpp
# )

# Link dependencies
target_link_libraries(pichat_cli PRIVATE pichat_core)