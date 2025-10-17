cmake_minimum_required(VERSION 3.20)
project(RPG_Monsters)

include(GNUInstallDirs)

if (NOT TARGET rygame)
    find_package(rygame REQUIRED)
endif ()

add_executable(
    ${PROJECT_NAME}
    code/main.cpp
    code/game.cpp
    code/sprite.cpp
    code/entities.cpp
    code/groups.cpp
    code/support.cpp
    code/dialogtree.cpp
    code/monster.cpp
    code/monsterindex.cpp
    code/battle.cpp
    code/transitiontarget.cpp
    code/evolution.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE rygame)

if (WIN32)
    target_compile_options(${PROJECT_NAME} PRIVATE "/WX" # "/Wall"
    )
else ()
    # set(CMAKE_CXX_COMPILER "clang++") # Sprite::Kill() has [[nodiscard]]
    target_compile_options(
        ${PROJECT_NAME}
        PRIVATE "-Wall"
        "-Werror"
        "-Wpedantic"
        "-Wno-missing-braces"
        "-Wno-gnu-anonymous-struct"
        "-Wno-nested-anon-types")
    # target_compile_options(${PROJECT_NAME} PRIVATE "-pg") # gprof
    # target_link_options(${PROJECT_NAME} PRIVATE "-pg") # gprof
endif ()

# copy resources to Build folder for debugging
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/resources
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)

option(INSTALL_GAME "Whether or not to install the game" OFF)
if (INSTALL_GAME)
    install(TARGETS ${PROJECT_NAME})
    # copy resources to Install location
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/resources
            DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR})
endif ()

if (EMSCRIPTEN)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${PROJECT_NAME} PRIVATE -g3)
        target_link_options(${PROJECT_NAME} PRIVATE
                            -gsource-map --source-map-base http://127.0.0.1:8081/
                            -sASSERTIONS=2
        )
    endif ()
    target_link_options(
        ${PROJECT_NAME}
        PRIVATE
        -sALLOW_MEMORY_GROWTH=1
        -sEXPORTED_RUNTIME_METHODS=[requestFullscreen]
        -sUSE_GLFW=3
        -sSTACK_SIZE=131072
        -sASYNCIFY=1
        --shell-file
        "$ENV{EMSCRIPTEN}/src/shell_brc.html")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")

    set(resources_dir ${CMAKE_CURRENT_SOURCE_DIR}/resources)
    if (EXISTS ${resources_dir})
        # The local resources path needs to be mapped to /resources virtual path
        string(APPEND resources_dir "@resources")
        target_link_options(
            ${PROJECT_NAME}
            PRIVATE
            --preload-file
            "${resources_dir}")
    endif ()
endif ()
