# Functions and Pre-build  -----------------------------------

# Stop messy in source builds
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)

if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt." )
endif()

# Start Build Config -----------------------------------
cmake_minimum_required(VERSION 3.11)
set(CMAKE_SKIP_RPATH true)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE ON)

# Library output name for the final binaries
set(LIBHDBPP_EXAMPLE_NAME "libhdb++example")

# Versioning
set(VERSION_MAJOR "0")
set(VERSION_MINOR "0")
set(VERSION_PATCH "0")
set(VERSION_METADATA "")
set(VERSION_STRING ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

# Add any include paths from the command line
list(APPEND INCLUDE_PATHS ${CMAKE_INCLUDE_PATH})
list(APPEND INCLUDE_PATHS ${CMAKE_SOURCE_DIR})
list(APPEND LIBRARY_PATHS ${CMAKE_LIBRARY_PATH})

# Start the project
project(libhdbpp_example VERSION ${VERSION_STRING} LANGUAGES CXX)

# Require this for the libpqxx build, also this project is c++14 minimum
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build options
option(BUILD_UNIT_TESTS "Build unit tests" OFF)
option(ENABLE_CLANG "Enable clang code and layout analysis" OFF)

if(BUILD_UNIT_TESTS)
    message(STATUS "Unit tests will be built")
endif(BUILD_UNIT_TESTS)

# arch install definitions
include(GNUInstallDirs)

message(STATUS "Searching for libraries...")

# allow pkg-config to search the CMAKE_PREFIX_PATH 
set(PKG_CONFIG_USE_CMAKE_PREFIX_PATH ON)
list(APPEND CMAKE_PREFIX_PATH "/usr")

# Find Dependencies  ---------------------
include(cmake/FindLibraries.cmake)

# First find tango if it has not already been found. Returns an interface library
# called TangoInterfaceLibrary
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(Tango)

# Find the libhdbpp headers
find_package(libhdbpp)

# If not installed then get them
if(NOT libhdbpp_FOUND)
    include(FetchContent)

    FetchContent_Declare(
      libhdbpp
      GIT_REPOSITORY https://github.com/tango-controls-hdbpp/libhdbpp.git
      GIT_TAG        master
    )

    FetchContent_GetProperties(libhdbpp)
    if(NOT libhdbpp_POPULATED)
        FetchContent_Populate(libhdbpp)
        add_subdirectory(${libhdbpp_SOURCE_DIR} ${libhdbpp_BINARY_DIR})
    endif()

    add_library(libhdbpp::libhdbpp_headers ALIAS libhdbpp_headers)

endif(NOT libhdbpp_FOUND)

# Thirdparty Integration -----------------------------------

# Code Analysis -----------------------------------
if(ENABLE_CLANG)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    # To find clang, find_program will search your PATH environment variable.
    # Ensure if you have a non-standard clang install, that it has been added
    # to your path.
    find_program(CLANG_TIDY_EXE 
        NAMES "clang-tidy" 
        DOC "Path to clang-tidy executable")

    if(NOT CLANG_TIDY_EXE)
        message(STATUS "clang-tidy not found.")
    else(NOT CLANG_TIDY_EXE)
        message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
        set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}")
    endif(NOT CLANG_TIDY_EXE)
endif(ENABLE_CLANG)

# Source -----------------------------------

add_subdirectory(src)

# Build Targets  -----------------------------------

# Export target name for subdirectory build
get_directory_property(hasParent PARENT_DIRECTORY)
if(hasParent)
    set(BACKEND_TARGET_SHARED "libhdbpp_example_shared_library" PARENT_SCOPE)
    set(BACKEND_TARGET_STATIC "libhdbpp_example_static_library" PARENT_SCOPE)
endif(hasParent)
set(BACKEND_TARGET_SHARED "libhdbpp_example_shared_library")
set(BACKEND_TARGET_STATIC "libhdbpp_example_static_library")

# Shared library --------
add_library(${BACKEND_TARGET_SHARED} SHARED ${SRC_FILES})

target_link_libraries(${BACKEND_TARGET_SHARED} 
    PUBLIC libhdbpp::libhdbpp_headers
    PRIVATE TangoInterfaceLibrary)

target_include_directories(${BACKEND_TARGET_SHARED} 
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE 
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
        ${INCLUDE_PATHS}
        "${PROJECT_BINARY_DIR}")

set_target_properties(${BACKEND_TARGET_SHARED} 
    PROPERTIES 
        OUTPUT_NAME hdb++example
        LINK_FLAGS "-Wl,--no-undefined"
        CXX_STANDARD 14
        POSITION_INDEPENDENT_CODE 1
        VERSION ${VERSION_STRING}
        SOVERSION ${VERSION_MAJOR})

if(DO_CLANG_TIDY)
    set_target_properties(${BACKEND_TARGET_SHARED} 
        PROPERTIES 
            CXX_CLANG_TIDY ${DO_CLANG_TIDY})
endif(DO_CLANG_TIDY)

target_compile_options(${BACKEND_TARGET_SHARED} 
    PRIVATE "$<$<CONFIG:DEBUG>:-g>")

# Static library --------
add_library(${BACKEND_TARGET_STATIC} STATIC EXCLUDE_FROM_ALL ${SRC_FILES})

target_link_libraries(${BACKEND_TARGET_STATIC} 
    INTERFACE libhdbpp::libhdbpp_headers
    PRIVATE TangoInterfaceLibrary)

target_include_directories(${BACKEND_TARGET_STATIC} 
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    PRIVATE 
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
        ${INCLUDE_PATHS}
        "${PROJECT_BINARY_DIR}")


set_target_properties(${BACKEND_TARGET_STATIC} 
    PROPERTIES 
        OUTPUT_NAME hdb++example
        LINK_FLAGS "-Wl,--no-undefined"
        CXX_STANDARD 14
        EXCLUDE_FROM_ALL 1)

target_compile_options(${BACKEND_TARGET_STATIC} 
    PRIVATE "$<$<CONFIG:DEBUG>:-g>")

# Install Config -----------------------------------
include(CMakePackageConfigHelpers)

install(
    TARGETS ${BACKEND_TARGET_SHARED}
    EXPORT libhdbpp-exampleTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

set(ConfigPackageLocation lib/cmake/libhdbpp-example)
set(Namespace libhdbpp::)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/libhdbpp-example/libhdbpp-exampleConfigVersion.cmake"
  VERSION ${VERSION_STRING}
  COMPATIBILITY AnyNewerVersion
)

export(EXPORT libhdbpp-exampleTargets
  FILE
    "${CMAKE_CURRENT_BINARY_DIR}/libhdbpp-example/libhdbpp-exampleTargets.cmake"
  NAMESPACE ${Namespace}
)


# generate the config file that is includes the exports
configure_package_config_file(cmake/libhdbpp-exampleConfig.cmake.in
  "${CMAKE_CURRENT_BINARY_DIR}/libhdbpp-example/libhdbpp-exampleConfig.cmake"
  INSTALL_DESTINATION
    ${ConfigPackageLocation}
  NO_SET_AND_CHECK_MACRO
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

install(EXPORT libhdbpp-exampleTargets
  FILE
    libhdbpp-exampleTargets.cmake
  NAMESPACE
    ${Namespace}
  DESTINATION
    ${ConfigPackageLocation}
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/libhdbpp-example/libhdbpp-exampleConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/libhdbpp-example/libhdbpp-exampleConfigVersion.cmake"
  DESTINATION
    ${ConfigPackageLocation}
)


# Tests -----------------------------------
if(BUILD_UNIT_TESTS)
    add_subdirectory(test)
endif(BUILD_UNIT_TESTS)

