cmake_minimum_required(VERSION 3.13)
project(sfml_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POLICY_VERSION_MINIMUM  3.5)

enable_language(C)

include_directories("C:/lua-5.4.7/src")
include_directories(libs)

add_executable(sfml-test
        src/main.cpp
        src/Utils.cpp
)

add_subdirectory(libs/lua)

include(FetchContent)

FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.2
        GIT_SHALLOW ON
        OVERRIDE_FIND_PACKAGE
)

set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.91.9b
)

# cкачиваем ImGui-SFML
FetchContent_Declare(
        imgui-sfml
        GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
        GIT_TAG 2.6.x
)

# активируем ImGui и ПЕРЕДАЕМ путь к нему в imgui-sfml
FetchContent_MakeAvailable(imgui)
set(IMGUI_DIR ${imgui_SOURCE_DIR} CACHE PATH "Path to ImGui source" FORCE)

FetchContent_MakeAvailable(SFML)

# теперь активируем ImGui-SFML (он уже будет знать, где ImGui)
set(IMGUI_SFML_FIND_SFML OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(imgui-sfml)


set(LUA_BUILD_BINARY OFF CACHE BOOL "" FORCE)

target_include_directories(sfml-test PRIVATE
        ${imgui-sfml_SOURCE_DIR}
        ${imgui_SOURCE_DIR}
)

# привязываем
target_link_libraries(sfml-test PRIVATE
        lua
        sfml-graphics
        sfml-window
        sfml-audio
        sfml-system
        ImGui-SFML::ImGui-SFML
)