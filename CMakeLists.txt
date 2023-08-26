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

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

add_subdirectory(GameEngine)
include_directories(
    include/
    externals/net/
    GameEngine
    GameEngine/vendors/SDL/include/
    GameEngine/vendors/SDL_image/include/
    GameEngine/vendors/glad/include/ GameEngine/vendors/glm/
    GameEngine/vendors/imgui/
)

add_subdirectory(externals/soci)

file(GLOB NET_SOURCES "external/net/*.hpp")

file(GLOB SERVER_SOURCES
    "source/ChessServer.cpp" "include/ChessServer.hpp"
    "source/ChessDataBaseOperations.cpp" "include/ChessDataBaseOperations.hpp"
    "source/ChessSessionServer.cpp" "include/ChessSessionServer.hpp" "include/ChessSessionServer.hpp"
)

file(GLOB APP_SOURCES "source/*.cpp" "include/*.hpp")
list(REMOVE_ITEM APP_SOURCES ${SERVER_SOURCES})

link_directories(
    ${PROJECT_SOURCE_DIR}/build/GameEngine/
    ${PROJECT_SOURCE_DIR}/build/GameEngine/vendors/SDL/
    ${PROJECT_SOURCE_DIR}/build/GameEngine/vendors/SDL_image/
)

add_executable(${PROJECT_NAME} ${APP_SOURCES} ${NET_SOURCES})
add_executable(${PROJECT_NAME}_SERVER ${SERVER_SOURCES} ${NET_SOURCES})

target_link_libraries(${PROJECT_NAME} Engine)
if(APPLE)
    target_link_libraries(${PROJECT_NAME} ${APPLICATIONSERVICES_FRAMEWORK})
endif()

if(Boost_FOUND)
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
    target_link_libraries(${PROJECT_NAME}_SERVER ${Boost_LIBRARIES})
    if(WIN32)
        target_link_libraries(${PROJECT_NAME} wsock32 ws2_32)
        target_link_libraries(${PROJECT_NAME}_SERVER wsock32 ws2_32)
    endif()
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/externals/soci/cmake/modules)
find_package(MySQL REQUIRED)
target_include_directories(${PROJECT_NAME}_SERVER PUBLIC externals/soci/include ${MYSQL_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME}_SERVER PUBLIC ${CMAKE_BINARY_DIR}/bin)
target_link_libraries(${PROJECT_NAME}_SERVER soci_mysql)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE:Engine> ${CMAKE_BINARY_DIR}
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE:SDL3-shared> ${CMAKE_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE:SDL3_image> ${CMAKE_BINARY_DIR}
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE:soci_core> ${CMAKE_BINARY_DIR}

    COMMAND ${CMAKE_COMMAND} -E copy
    $<TARGET_FILE:soci_mysql> ${CMAKE_BINARY_DIR}
)