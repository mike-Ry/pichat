cmake_minimum_required(VERSION 3.12)
project(PiChat VERSION 0.1.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)


# Link with the core libraries
if(TARGET pichat_core AND TARGET pichat_cli)
  # Main executable
  add_executable(PiChat ${CMAKE_CURRENT_SOURCE_DIR}/../src/main.cpp)
  target_link_libraries(PiChat PRIVATE 
      pichat_core 
      pichat_cli
      CURL::libcurl
      nlohmann_json::nlohmann_json
  )

  # Installation rules
  install(TARGETS PiChat DESTINATION bin)
endif()