add_executable(gitl)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

#target_link_libraries(main PRIVATE Qt::Widgets Qt6::Widgets Qt::WidgetsPrivate Qt6::WidgetsPrivate)

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
