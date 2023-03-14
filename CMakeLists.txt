cmake_minimum_required(VERSION 3.24)

project(Chaess3D)

enable_language(C)
enable_language(CXX)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(GameEngine)
include_directories(
    GameEngine
    GameEngine/vendors/SDL/include/
    GameEngine/vendors/SDL_image/include/
    GameEngine/vendors/glad/include/ GameEngine/vendors/glm/
    GameEngine/vendors/imgui/
)

file(GLOB SOURCES "source/*.cpp" "include/*.hpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    -LC:/Users/hlebushkek/Documents/Chess3D/build/GameEngine/
    -LC:/Users/hlebushkek/Documents/Chess3D/build/GameEngine/vendors/SDL/
    -LC:/Users/hlebushkek/Documents/Chess3D/build/GameEngine/vendors/SDL_image/
)

target_link_libraries(${PROJECT_NAME} imgui)
target_link_libraries(${PROJECT_NAME} SDL3 SDL3_image)
target_link_libraries(${PROJECT_NAME} libEngine)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_CURRENT_BINARY_DIR}/GameEngine/libEngine.dll
            ${CMAKE_CURRENT_BINARY_DIR}/libEngine.dll
)