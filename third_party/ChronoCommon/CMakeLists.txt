cmake_minimum_required(VERSION 3.10)
project(ChronoCommon LANGUAGES CXX)

# Set the C++ standard for ChronoCommon.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create a library target for ChronoCommon.
# Assume that the library only consists of headers and maybe some source files.
# Here, we add all the source files in the include directory that need compilation.
file(GLOB CHRONOCOMMON_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.cpp")

add_library(ChronoCommon STATIC ${CHRONOCOMMON_SOURCES})

# Specify include directories for ChronoCommon.
target_include_directories(ChronoCommon PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)