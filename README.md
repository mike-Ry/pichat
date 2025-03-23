# PiChat Project

PiChat is a C++ application that provides an interface to the DeepSeek AI API for natural language processing. The application supports both interactive chat mode and service mode, allowing users to interact with the AI model through a command-line interface.

## Features

- Interactive chat mode with real-time responses
- Service mode for running as a background process
- CLI commands for managing the application
- Configuration management for storing API keys and settings
- Error handling for robust operation

## Prerequisites

To build and run PiChat, you'll need:

- CMake (version 3.8 or later)
- Visual Studio 2019 or later (on Windows)
- libcurl (for HTTP requests)
- nlohmann_json library (for JSON parsing)
- C++17 compatible compiler

## Building the Project

### Installing Dependencies

First, you need to install the required dependencies. The recommended way is to use vcpkg:

1. Install vcpkg:
```powershell
cd C:\
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
```

2. Install required packages:
```powershell
.\vcpkg install curl:x64-windows
.\vcpkg install nlohmann-json:x64-windows
```

3. Integrate with Visual Studio:
```powershell
.\vcpkg integrate install
```

### Building with CMake

1. Clone the repository:
```powershell
git clone https://github.com/yourusername/pichat.git
cd pichat
```

2. Create a build directory and run CMake:
```powershell
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
```

3. Build the project:
```powershell
cmake --build . --config Release
```

## Running PiChat

The compiled executable will be located in `build\Pichat\Release\PiChat.exe`.

### Setting up the DeepSeek API Key

Before using PiChat, you need to set up your DeepSeek API key:

```powershell
.\PiChat.exe --set-key YOUR_API_KEY
```

Replace `YOUR_API_KEY` with your actual DeepSeek API key. For this project, we're using the key: `sk-3ce8d607f9db409eaece8f07bce72c61`.

### Interactive Mode

To start a chat session with the AI:

```powershell
.\PiChat.exe --interactive
```

In interactive mode:
- Type your message and press Enter to send
- Type 'clear' to reset the conversation history
- Type 'exit' to quit the application

### Service Mode

To run PiChat as a background service:

```powershell
.\PiChat.exe --start
```

To check if the service is running:

```powershell
.\PiChat.exe --status
```

To stop the service:

```powershell
.\PiChat.exe --stop
```

## Configuration

PiChat stores its configuration in:
- Windows: `%APPDATA%\PiChat\config.json`
- Unix: `~/.pichat/config.json`

The configuration file stores:
- DeepSeek API key
- Language preferences
- Other application settings

## Project Structure

```
/pichat
  /include                 # Header files
    /cli                   # Command-line interface headers
    /config                # Configuration management headers
    /utils                 # Utility headers
  /src                     # Source files
    /cli                   # CLI implementation
    /config                # Configuration implementation
    /utils                 # Utility implementation
    main.cpp               # Main application entry point
  /build                   # Build directory
  CMakeLists.txt           # CMake configuration
  README.md                # This file
```

## Troubleshooting

### Unicode Character Display Issues

If you see strange characters or placeholders instead of emojis in the AI responses, this is likely a character encoding issue with your terminal. Try:

1. Using Windows Terminal instead of Command Prompt
2. Ensuring your terminal's font supports Unicode characters
3. Setting your terminal's character encoding to UTF-8

### API Connection Issues

If you're experiencing issues connecting to the DeepSeek API:

1. Verify your API key is correct
2. Check your internet connection
3. Make sure your firewall isn't blocking the connection
4. Verify the model name is correct (should be "deepseek-chat")

## Advanced Usage

### Command-line Arguments

PiChat supports the following commands:

- `--help`: Show help message
- `--start`: Start PiChat service
- `--stop`: Stop PiChat service
- `--status`: Check service status
- `--set-key <API_KEY>`: Set DeepSeek API key
- `--interactive`: Start interactive chat mode
- `--service`: Run as a service (usually invoked by --start)

### Working with Different Models

The DeepSeek API provides different AI models. By default, PiChat uses the "deepseek-chat" model, but you can modify the code to use other models like "deepseek-reasoner" if desired.

## Future Improvements

Potential improvements for the project:

1. Adding a graphical user interface
2. Supporting multiple chat sessions
3. Adding voice input/output capabilities
4. Implementing local LLM support
5. Adding more customization options

## License

[Your license information here]

## Acknowledgments

- This project uses the DeepSeek API
- Thanks to the creators of libcurl and nlohmann_json
- [Any other acknowledgments]
