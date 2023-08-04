cmake_minimum_required(VERSION 3.24)

project(Chess3D)

enable_language(C)
enable_language(CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

if(APPLE)
    enable_language(OBJC)
    find_library(APPLICATIONSERVICES_FRAMEWORK ApplicationServices)
endif()

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

target_link_libraries(${PROJECT_NAME} Engine)
if(APPLE)
    target_link_libraries(${PROJECT_NAME} ${APPLICATIONSERVICES_FRAMEWORK})
endif()

if(APPLE)
    set(LIB_EXT dylib)
    set(LIB_PREFIX lib)
else()
    set(LIB_EXT dll)
    set(LIB_PREFIX)
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_BINARY_DIR}/GameEngine/libEngine.${LIB_EXT}
    ${CMAKE_BINARY_DIR}/
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_BINARY_DIR}/GameEngine/vendors/SDL/${LIB_PREFIX}SDL3.${LIB_EXT}
    ${CMAKE_BINARY_DIR}

    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_BINARY_DIR}/GameEngine/vendors/SDL_image/${LIB_PREFIX}SDL3_image.${LIB_EXT}
    ${CMAKE_BINARY_DIR}/
)