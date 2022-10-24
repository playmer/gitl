cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR) 

# install vcpkg into our project
include(FetchContent)
FetchContent_Declare(
    vcpkg
    GIT_REPOSITORY https://github.com/playmer/vcpkg.git
    GIT_TAG        origin/playmer/AllTheThings
)
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    FetchContent_Populate(vcpkg)
    set(CMAKE_TOOLCHAIN_FILE "${vcpkg_SOURCE_DIR}/scripts/buildsystems/vcpkg.cmake")
endif()

project(gitl VERSION 1.0.0 LANGUAGES CXX)

add_definitions(-D_MBCS -DMBCS)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)  

find_package(tiny-process-library CONFIG REQUIRED)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()


set(DepsDir ${CMAKE_CURRENT_LIST_DIR}/deps)

OPTION(BUILD_SHARED_LIBS "Build Shared Library (OFF for Static)" OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(deps)
add_subdirectory(src)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT gitl) 