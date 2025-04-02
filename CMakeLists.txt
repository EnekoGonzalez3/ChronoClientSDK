cmake_minimum_required(VERSION 3.13)
project(ChronoClientLib LANGUAGES C CXX)

# Set the C++ standard.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include GNUInstallDirs to use standard installation directories.
include(GNUInstallDirs)

# --- Find External Dependencies ---

# Find pkg-config (required for Margo and json-c).
find_package(PkgConfig REQUIRED)

# Margo via pkg-config.
pkg_check_modules(MARGO REQUIRED margo)
message(STATUS "Margo found, version: ${MARGO_VERSION}")

# Create an INTERFACE imported target for Margo.
add_library(Margo::margo INTERFACE IMPORTED)
set_target_properties(Margo::margo PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${MARGO_INCLUDE_DIRS}"
    INTERFACE_LINK_LIBRARIES "${MARGO_LIBRARIES}"
    INTERFACE_LINK_DIRECTORIES "${MARGO_LIBRARY_DIRS}"
)

# Find Thallium.
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

# (From ChronoCommon) Find MPI (if ChronoCommon sources need it).
find_package(MPI REQUIRED COMPONENTS C CXX)
message(STATUS "MPI found: ${MPI_C_INCLUDE_DIRS}")

# (From ChronoCommon) Find spdlog.
find_package(spdlog REQUIRED)
message(STATUS "spdlog found: ${spdlog_INCLUDE_DIRS}")

# (From ChronoCommon) Find json-c using pkg-config.
pkg_check_modules(JSONC REQUIRED json-c)
message(STATUS "json-c found: ${JSONC_INCLUDE_DIRS}")

# (From ChronoCommon) Find HDF5 (with C++ component).
find_package(HDF5 REQUIRED COMPONENTS CXX)
message(STATUS "HDF5 found: ${HDF5_INCLUDE_DIRS}")

# --- Gather Sources ---
# List all .cpp files that will form the chronolog_client library.
set(SOURCES
    src/ChronologClient.cpp
    src/ConfigurationManager.cpp
    src/StoryChunk.cpp
    src/chrono_monitor.cpp
    src/ChronologClientImpl.cpp
    src/ClientQueryService.cpp
    src/PlaybackQueryRpcClient.cpp
    src/StorytellerClient.cpp
)

# --- Create the Library Target ---

add_library(chronolog_client STATIC ${SOURCES})

# Specify include directories.
# PUBLIC: headers that are part of the installed API (e.g. chronolog_client.h).
# PRIVATE: internal dependencies (from ChronoCommon).
target_include_directories(chronolog_client
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${THALLIUM_INCLUDE_DIRS}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${JSONC_INCLUDE_DIRS}
        ${spdlog_INCLUDE_DIRS}
        ${HDF5_INCLUDE_DIRS}
)

# --- Link Dependencies ---

# Link Thallium and Margo. Also link spdlog (from ChronoCommon).
if(TARGET Thallium::thallium)
    target_link_libraries(chronolog_client
        PUBLIC Thallium::thallium Margo::margo spdlog::spdlog
    )
else()
    target_link_libraries(chronolog_client
        PUBLIC ${THALLIUM_LIBRARIES} Margo::margo spdlog::spdlog
    )
endif()

# Link HDF5.
if(TARGET HDF5::HDF5)
    target_link_libraries(chronolog_client PRIVATE HDF5::HDF5)
else()
    target_link_libraries(chronolog_client PRIVATE ${HDF5_LIBRARIES})
endif()

# Link json-c libraries.
target_link_libraries(chronolog_client PRIVATE ${JSONC_LIBRARIES})

# (Optional) Link MPI if needed.
# target_link_libraries(chronolog_client PRIVATE MPI::MPI_C MPI::MPI_CXX)

# --- Build Example Executable ---

add_executable(example_app examples/main.cpp)
target_link_libraries(example_app PRIVATE chronolog_client)

# --- Installation Rules ---

# Install the chronolog_client target.
install(TARGETS chronolog_client
    EXPORT ChronoClientSDKTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install public headers.
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Export the targets.
install(EXPORT ChronoClientSDKTargets
    FILE ChronoClientSDKTargets.cmake
    NAMESPACE ChronoClientSDK::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ChronoClientSDK
)

# Configure the package config file.
include(CMakePackageConfigHelpers)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/ChronoClientSDKConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/ChronoClientSDKConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ChronoClientSDK
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/ChronoClientSDKConfigVersion.cmake
  VERSION 1.0.0
  COMPATIBILITY AnyNewerVersion
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/ChronoClientSDKConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/ChronoClientSDKConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ChronoClientSDK
)