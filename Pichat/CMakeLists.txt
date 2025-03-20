cmake_minimum_required(VERSION 3.12)
project(PiChat VERSION 0.1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Find required packages for Windows
if(WIN32)
    # Windows-specific libraries might be needed
    # find_package(...)
endif()

# Add subdirectories
add_subdirectory(src)

# Main executable target
add_executable(PiChat src/main.cpp)

# Link libraries
target_link_libraries(PiChat 
    PRIVATE 
    pichat_core
    pichat_cli
)

# Installation rules
install(TARGETS PiChat DESTINATION bin)