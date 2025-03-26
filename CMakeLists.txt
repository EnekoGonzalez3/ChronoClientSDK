cmake_minimum_required(VERSION 3.13)
project(ChronoClientSDK LANGUAGES CXX)

# Set the C++ standard.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Find Margo via pkg-config ---
find_package(PkgConfig REQUIRED)
pkg_check_modules(MARGO REQUIRED margo)
message(STATUS "Margo found, version: ${MARGO_VERSION}")

# Create an INTERFACE imported target for Margo.
add_library(Margo::margo INTERFACE IMPORTED)
set_target_properties(Margo::margo PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${MARGO_INCLUDE_DIRS}"
    INTERFACE_LINK_LIBRARIES "${MARGO_LIBRARIES}"
    INTERFACE_LINK_DIRECTORIES "${MARGO_LIBRARY_DIRS}"
)

# --- Find Thallium ---
find_package(Thallium REQUIRED)
message(STATUS "Thallium found: ${THALLIUM_INCLUDE_DIRS}")

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
    ${THALLIUM_INCLUDE_DIRS}
)

# Link ChronoCommon, Thallium, and Margo with your SDK library.
if(TARGET Thallium::thallium)
    target_link_libraries(chronolog_client PUBLIC ChronoCommon Thallium::thallium Margo::margo)
else()
    target_link_libraries(chronolog_client PUBLIC ChronoCommon ${THALLIUM_LIBRARIES} Margo::margo)
endif()

# Build an example executable that links against your client library.
add_executable(example_app examples/main.cpp)
target_link_libraries(example_app PRIVATE chronolog_client)