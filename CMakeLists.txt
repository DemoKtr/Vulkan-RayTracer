cmake_minimum_required(VERSION 3.10)

# Nazwa projektu
project(VulkanApp)

# Ustawienie standardu C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Znajdowanie pakietu Vulkan
find_package(Vulkan REQUIRED)

# FetchContent to sposób na automatyczne pobieranie zależności
include(FetchContent)
set(GLFW_VULKAN_STATIC ON)
# Pobieranie GLFW
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG latest
)
FetchContent_MakeAvailable(glfw)

# Pobieranie GLM
FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG master
)
FetchContent_MakeAvailable(glm)

# Źródła projektu
set(SOURCES
    src/main.cpp
    src/app.cpp
)

# Tworzenie aplikacji
add_executable(${PROJECT_NAME} ${SOURCES} "src/View/engine.cpp" "src/Player/camera.cpp" "src/Scene/scene.cpp" "src/Scene/sceneObject.cpp" "src/View/vkInit/instance.cpp" "src/View/vkInit/logging.cpp")

# Dodanie bibliotek do aplikacji
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan glfw glm)

# Ustawienia dla kompilacji w systemie Linux
if(UNIX)
    target_link_libraries(${PROJECT_NAME} dl pthread)
endif()

# Ustawienie katalogu z plikami nagłówkowymi
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${GLFW_INCLUDE_DIRS} 
    ${Vulkan_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
PUBLIC ${CMAKE_SOURCE_DIR}/include)
# Opcjonalne: Ustawienia kompilatora (dodatkowe flagi)
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()
