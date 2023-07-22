cmake_minimum_required(VERSION 3.24)

project(Chess3D)

enable_language(C)
enable_language(CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")

add_subdirectory(GameEngine)
include_directories(
    include/
    GameEngine
    GameEngine/vendors/SDL/include/
    GameEngine/vendors/SDL_image/include/
    GameEngine/vendors/glad/include/ GameEngine/vendors/glm/
    GameEngine/vendors/imgui/
)

file(GLOB SOURCES "source/*.cpp" "include/*.hpp")

link_directories(
    ${PROJECT_SOURCE_DIR}/build/GameEngine/
    ${PROJECT_SOURCE_DIR}/build/GameEngine/vendors/SDL/
    ${PROJECT_SOURCE_DIR}/build/GameEngine/vendors/SDL_image/
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} imgui)
target_link_libraries(${PROJECT_NAME} SDL3 SDL3_image)
target_link_libraries(${PROJECT_NAME} libEngine)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_BINARY_DIR}/GameEngine/libEngine.dll
            ${CMAKE_CURRENT_BINARY_DIR}/libEngine.dll
)