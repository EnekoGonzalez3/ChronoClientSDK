cmake_minimum_required(VERSION 3.10)
project(ChronoCommon LANGUAGES C CXX)

# Set the C++ standard for ChronoCommon.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Find External Dependencies ---

# Find MPI so that MPI targets (e.g., MPI::MPI_C) are defined.
find_package(MPI REQUIRED COMPONENTS C CXX)
message(STATUS "MPI found: ${MPI_C_INCLUDE_DIRS}")

# Find Thallium. (This assumes Thallium provides either an imported target or sets THALLIUM_LIBRARIES.)
find_package(Thallium REQUIRED)
message(STATUS "Thallium found: ${THALLIUM_INCLUDE_DIRS}")

# Find spdlog using its config file.
find_package(spdlog REQUIRED)
message(STATUS "spdlog found: ${spdlog_INCLUDE_DIRS}")

# Find json-c using pkg-config.
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONC REQUIRED json-c)
message(STATUS "json-c found: ${JSONC_INCLUDE_DIRS}")

# Find HDF5 (we need the C++ component for H5Cpp.h)
find_package(HDF5 REQUIRED COMPONENTS CXX)
message(STATUS "HDF5 found: ${HDF5_INCLUDE_DIRS}")

# --- Gather ChronoCommon Sources ---
file(GLOB CHRONOCOMMON_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/include/*.cpp")

# Create a static library target for ChronoCommon.
add_library(ChronoCommon STATIC ${CHRONOCOMMON_SOURCES})

# --- Specify Include Directories ---
target_include_directories(ChronoCommon PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Propagate external dependency include directories so ChronoCommon's sources can find:
#   - Thallium headers (e.g. thallium/serialization/stl/string.hpp)
#   - json-c headers (e.g. json-c/json.h)
#   - spdlog headers (e.g. spdlog/common.h)
#   - HDF5 headers (e.g. H5Cpp.h)
target_include_directories(ChronoCommon PRIVATE
    ${THALLIUM_INCLUDE_DIRS}
    ${JSONC_INCLUDE_DIRS}
    ${spdlog_INCLUDE_DIRS}
    ${HDF5_INCLUDE_DIRS}
)

# --- Link External Libraries ---
# Link against Thallium and spdlog.
if(TARGET Thallium::Thallium)
    target_link_libraries(ChronoCommon PRIVATE Thallium::Thallium spdlog::spdlog)
else()
    target_link_libraries(ChronoCommon PRIVATE ${THALLIUM_LIBRARIES} spdlog::spdlog)
endif()

# Link against HDF5 libraries. If HDF5 provides an imported target, use that.
if(TARGET HDF5::HDF5)
    target_link_libraries(ChronoCommon PRIVATE HDF5::HDF5)
else()
    target_link_libraries(ChronoCommon PRIVATE ${HDF5_LIBRARIES})
endif()

# --- Installation Rules (Optional) ---
include(GNUInstallDirs)

install(TARGETS ChronoCommon
        EXPORT ChronoCommonTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT ChronoCommonTargets
        FILE ChronoCommonTargets.cmake
        NAMESPACE ChronoCommon::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ChronoCommon
)