add_executable(gitl)

if (CMAKE_SYSTEM_NAME STREQUAL Windows)
  # Find the QtWidgets library
  set(QtPath $ENV{QtPath})
  if(NOT QtPath) 
    message(FATAL_ERROR "Please define the environment variable `Qt` with the path to Qt 5.9.0 - i.e. export Qt = C:/Qt/5.9")
  endif()

  set(CMAKE_PREFIX_PATH ${QtPath}/msvc2019_64/)
endif()

find_package(Qt6 COMPONENTS Widgets REQUIRED)

target_link_libraries(gitl 
  PUBLIC 
    Qt6::Widgets
    toolwindowmanager
    cppgit2
)

add_subdirectory(Framework)
add_subdirectory(Models)
add_subdirectory(Widgets)

target_include_directories(gitl 
PUBLIC 
    ${DepsDir}/toolwindowmanager/src
    ./
)

target_compile_features(gitl PUBLIC cxx_std_20)

target_sources(gitl
PRIVATE
    GitHelper.cpp
    GitHelper.hpp
    main.cpp
)
