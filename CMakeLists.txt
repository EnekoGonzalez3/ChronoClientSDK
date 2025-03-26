cmake_minimum_required(VERSION 3.10)
project(ChronoClientSDK LANGUAGES CXX)

# Set the C++ standard.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Thallium.
find_package(Thallium REQUIRED)
message(STATUS "Thallium found: ${THALLIUM_INCLUDE_DIRS}")

# If THALLIUM_INCLUDE_DIRS is empty, try to locate thallium.hpp manually.
if(NOT THALLIUM_INCLUDE_DIRS)
    find_path(THALLIUM_INCLUDE_DIRS NAMES thallium.hpp)
    if(THALLIUM_INCLUDE_DIRS)
        message(STATUS "Found thallium.hpp in ${THALLIUM_INCLUDE_DIRS}")
    else()
        message(FATAL_ERROR "Could not find thallium.hpp. Please set THALLIUM_INCLUDE_DIRS manually!")
    endif()
endif()

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
    ${THALLIUM_INCLUDE_DIRS}  # Use the detected Thallium include directory.
)

# Link ChronoCommon and Thallium with your SDK library.
if(TARGET Thallium::thallium)
    # If the imported target Thallium::thallium is available, link to it.
    target_link_libraries(chronolog_client PUBLIC ChronoCommon Thallium::thallium)
else()
    target_link_libraries(chronolog_client PUBLIC ChronoCommon ${THALLIUM_LIBRARIES})
endif()

# Build an example executable that links against your client library.
add_executable(example_app examples/main.cpp)
target_link_libraries(example_app PRIVATE chronolog_client)