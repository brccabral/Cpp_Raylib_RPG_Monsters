cmake_minimum_required(VERSION 3.20)
project(RPG_Monsters)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)

find_package(raylib REQUIRED)
find_package(tmx REQUIRED)

set(PLATFORM "Desktop" CACHE STRING "Desktop, Android, Web")
message("PLATFORM ${PLATFORM}")

add_executable(${PROJECT_NAME} code/main.cpp
        code/game.cpp
        code/sprite.cpp
        code/entities.cpp
        code/groups.cpp
        code/dialogtree.cpp
        code/dialogsprite.cpp
        code/timer.cpp
        code/monster.cpp
        code/monsterindex.cpp
        code/battle.cpp
        code/raylib_utils.cpp
        code/support.cpp
        code/transitiontarget.cpp
        code/evolution.cpp
        code/tiledfont.cpp
)
target_link_libraries(${PROJECT_NAME} raylib tmx)

if (PLATFORM STREQUAL "Desktop")
    target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_DESKTOP)
endif ()

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

install(TARGETS ${PROJECT_NAME})

# copy resources to Install location
install(DIRECTORY
        ${PROJECT_SOURCE_DIR}/resources
        DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}
)
