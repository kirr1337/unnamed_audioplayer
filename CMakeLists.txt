cmake_minimum_required(VERSION 3.16)
project(sfml_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "C:\\SFML-2.6.2\\lib\\cmake\\SFML")

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

add_executable(sfml-test main.cpp
        main.cpp)

target_link_libraries(sfml-test PRIVATE sfml-graphics sfml-window sfml-audio sfml-system)

include(FetchContent)

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
set(IMGUI_DIR ${imgui_SOURCE_DIR})

# теперь активируем ImGui-SFML (он уже будет знать, где ImGui)
FetchContent_MakeAvailable(imgui-sfml)

# привязываем к проекту
target_link_libraries(sfml-test PRIVATE ImGui-SFML::ImGui-SFML)