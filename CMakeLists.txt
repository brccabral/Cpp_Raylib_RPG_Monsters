cmake_minimum_required(VERSION 3.20)
project(RPG_Monsters)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)

find_package(raylib REQUIRED)
find_package(tmx REQUIRED)

add_executable(${PROJECT_NAME} code/main.cpp
        code/game.cpp)
target_link_libraries(${PROJECT_NAME} raylib tmx m)
install(TARGETS ${PROJECT_NAME})

if (WIN32)
    target_include_directories(${PROJECT_NAME} PRIVATE ${RAYLIB_PATH}/include)
    target_link_directories(${PROJECT_NAME} PRIVATE ${RAYLIB_PATH}/lib)
    target_link_libraries(${PROJECT_NAME} gdi32 msvcrt winmm)
    set(COMPILE_FLAGS "/NODEFAULTLIB:libcmt")
endif ()

# copy resources to Build folder for debugging
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
)

# copy resources to Install location
install(DIRECTORY
        ${PROJECT_SOURCE_DIR}/resources
        DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}
)
