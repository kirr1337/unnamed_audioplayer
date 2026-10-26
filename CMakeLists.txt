cmake_minimum_required(VERSION 3.16)
project(sfml_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "C:\\SFML-2.6.2\\lib\\cmake\\SFML")

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)

add_executable(sfml-test main.cpp)

target_link_libraries(sfml-test PRIVATE sfml-graphics sfml-window sfml-audio sfml-system)

include(FetchContent)

# 1. Сначала скачиваем сам Dear ImGui
FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.91.9b # Версия, совместимая с imgui-sfml 2.6
)

# 2. Скачиваем ImGui-SFML
FetchContent_Declare(
        imgui-sfml
        GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
        GIT_TAG 2.6.x
)

# 3. Активируем ImGui и ПЕРЕДАЕМ путь к нему в imgui-sfml
FetchContent_MakeAvailable(imgui)
set(IMGUI_DIR ${imgui_SOURCE_DIR}) # <-- Вот та самая переменная, которую он просит!

# 4. Теперь активируем ImGui-SFML (он уже будет знать, где ImGui)
FetchContent_MakeAvailable(imgui-sfml)

# 5. Привязываем к проекту
target_link_libraries(sfml-test PRIVATE ImGui-SFML::ImGui-SFML)