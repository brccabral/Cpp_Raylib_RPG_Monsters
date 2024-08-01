cmake_minimum_required(VERSION 3.20)
project(RPG_Monsters)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)

find_package(raylib REQUIRED)
find_package(rygame REQUIRED)
find_package(tmx REQUIRED)
find_package(raylib-tmx REQUIRED)


add_executable(${PROJECT_NAME} code/main.cpp
        code/game.cpp
        code/game.h
        code/sprite.cpp
        code/entities.cpp
)

target_link_libraries(${PROJECT_NAME} raylib rygame tmx raylib-tmx)

if (WIN32)
    target_link_libraries(${PROJECT_NAME} gdi32 winmm)
    set(COMPILE_FLAGS "/NODEFAULTLIB:libcmt")
else ()
    target_link_libraries(${PROJECT_NAME} m)
endif ()

# copy resources to Build folder for debugging
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
)

option(INSTALL_GAME "Whether or not to install the game" OFF)
if (INSTALL_GAME)
    install(TARGETS ${PROJECT_NAME})
    # copy resources to Install location
    install(DIRECTORY
            ${PROJECT_SOURCE_DIR}/resources
            DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}
    )
endif ()
