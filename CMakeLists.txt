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

# Ustawienia dla GLFW
set(GLFW_VULKAN_STATIC ON)  # Włącz obsługę Vulkan w GLFW
set(GLFW_BUILD_EXAMPLES OFF) # Wyłącz przykłady GLFW, nie są potrzebne
set(GLFW_BUILD_TESTS OFF)    # Wyłącz testy GLFW, nie są potrzebne

# Pobieranie GLFW
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4   # Użyj stabilnej wersji GLFW zamiast latest
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
add_executable(${PROJECT_NAME} ${SOURCES} 
    "src/View/engine.cpp" 
    "src/Player/camera.cpp" 
    "src/Scene/scene.cpp" 
    "src/Scene/sceneObject.cpp" 
    "src/View/vkInit/instance.cpp" 
    "src/View/vkInit/logging.cpp"
    "src/View/vkInit/descriptors.cpp"
    "src/View/vkUtil/singleTimeCommands.cpp"
    "src/View/vkUtil/memory.cpp"
    "src/View/vkInit/device.cpp"
    "src/View/vkUtil/queues.cpp"
    "src/View/vkUtil/frame.cpp"
    "src/View/vkInit/swapchain.cpp"
    "src/View/vkImage/image.cpp"
    "src/View/vkVertexData/vertex.cpp"
    "src/View/vkVertexData/triangle.cpp"
    "src/View/vkVertexData/aabb.cpp"
    "src/View/vkVertexData/bvh.cpp"
    "src/Scene/ECS/component.cpp"
    "src/Scene/ECS/entity.cpp"
    "src/Scene/ECS/system.cpp"
    "src/Scene/ECS/ecs.cpp")

# Dodanie bibliotek do aplikacji
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan glfw glm)

# Ustawienia dla kompilacji w systemie Linux
if(UNIX)
    target_link_libraries(${PROJECT_NAME} dl pthread)
endif()

# Ustawienie katalogu z plikami nagłówkowymi
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${Vulkan_INCLUDE_DIRS}
PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Opcjonalne: Ustawienia kompilatora (dodatkowe flagi)
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()
