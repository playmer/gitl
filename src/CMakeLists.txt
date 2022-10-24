qt_add_executable(gitl)

#target_link_libraries(main PRIVATE Qt::Widgets Qt6::Widgets Qt::WidgetsPrivate Qt6::WidgetsPrivate)

target_link_libraries(gitl 
  PUBLIC 
    Qt::Widgets
    toolwindowmanager
    tiny-process-library::tiny-process-library
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

if (${CMAKE_SYSTEM_NAME} STREQUAL Windows)
    # Qt Deployment stuff
    find_program(TOOL_WINDEPLOYQT_DEBUG NAMES windeployqt.debug.bat)
    find_program(TOOL_WINDEPLOYQT NAMES windeployqt)

    add_custom_command(TARGET gitl POST_BUILD
        COMMAND $<IF:$<CONFIG:DEBUG>,${TOOL_WINDEPLOYQT_DEBUG},${TOOL_WINDEPLOYQT}>
                $<TARGET_FILE:gitl>
        COMMENT "Running windeployqt..."
    )

elseif(${CMAKE_SYSTEM_NAME} STREQUAL Darwin)
    find_program(TOOL_MACDEPLOYQT_DEBUG NAMES macdeployqt.debug.bat)
    find_program(TOOL_MACDEPLOYQT NAMES macdeployqt)

    #add_custom_command(TARGET gitl POST_BUILD
    #    COMMAND $<IF:$<CONFIG:DEBUG>,${TOOL_MACDEPLOYQT_DEBUG},${TOOL_MACDEPLOYQT}>
    #            $<TARGET_FILE_NAME:gitl>
    #    COMMENT "Running macdeployqt..."
    #)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL Linux)
endif()