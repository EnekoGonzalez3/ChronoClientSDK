cmake_minimum_required(VERSION 3.10)
project(ChronoClientSDK LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optionally, find Thallium at the root (so you can verify it)
find_package(Thallium REQUIRED)
message(STATUS "Thallium found: ${THALLIUM_INCLUDE_DIRS}")

# Add ChronoCommon as a subdirectory.
add_subdirectory(third_party/ChronoCommon)

# Create the chronolog_client library from your source file.
add_library(chronolog_client
    src/chronolog_client.cpp
)

# Specify include directories for chronolog_client.
target_include_directories(chronolog_client PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/ChronoCommon/include
)

# Link ChronoCommon with your SDK library.
target_link_libraries(chronolog_client PUBLIC ChronoCommon)

# Build an example executable that links against your client library.
add_executable(example_app examples/main.cpp)
target_link_libraries(example_app PRIVATE chronolog_client)