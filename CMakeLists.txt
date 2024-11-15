cmake_minimum_required(VERSION 3.10)

# Nazwa projektu
project(VulkanApp)

# Ustawienie standardu C++
set(CMAKE_CXX_STANDARD 20)
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

# Assimp
FetchContent_Declare(
  assimp
  GIT_REPOSITORY https://github.com/assimp/assimp.git
  GIT_TAG master # Ostatnia stabilna wersja
)
FetchContent_MakeAvailable(assimp)
# ImGui
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG master  # Ostatnia stabilna wersja
)
FetchContent_MakeAvailable(imgui)

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
    "src/Scene/ECS/ecs.cpp"
    # Dodaj pliki ImGui
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
    "src/View/vkUtil/renderpass.cpp"
    "src/View/vkInit/commands.cpp"
    "src/View/vkInit/synchronizer.cpp"
    "src/View/vkInit/framebuffer.cpp"
    "src/View/Interface/interface.cpp"
    "src/Scene/ECS/components/transformComponent.cpp"
    "src/Scene/ECS/components/scriptComponent.cpp"
    "src/Scene/ECS/components/meshComponent.cpp"
    "src/Scene/ECS/components/renderComponent.cpp"
    "src/Scene/ECS/components/physicsComponent.cpp"
    "src/View/vkVertexData/vertexMenagerie.cpp"
    "src/MultithreatedSystems/ThumbnailTask.cpp"
    "src/View/vkMesh/meshTypes.cpp"
    "src/View/Interface/Thumbs/thumbRenderer.cpp"
    "src/View/vkInit/vkPipeline/pipelineBuilder.cpp"
    "src/View/vkInit/vkPipeline/shaders.cpp"
    "src/View/vkMesh/vertexMenagerie.cpp"
    "src/View/vkMesh/mesh.cpp"
    "src/View/vkMesh/meshLoader.cpp"
    "src/Converters/assimpConverter.cpp"
    "src/Scene/ECS/components/componentFabric.cpp"
    "src/View/vkImage/texture.cpp"
    "src/View/Interface/Commands/command.cpp"
    "src/View/Interface/Commands/commandHistory.cpp"
    "src/View/Interface/Commands/filesystemCommand.cpp"
    "src/View/Interface/Commands/sceneCommand.cpp"
    "src/View/vkImage/textureTypes.cpp"
    "src/View/vkMesh/meshManager.cpp"
    "src/View/vkMesh/vertexFormat.cpp"
    "src/View/RenderStructs/projection.cpp"
    "src/Scene/ECS/components/textureComponent.cpp"
    "src/View/Interface/Thumbs/thumbsManager.cpp"
    "src/View/Interface/Thumbs/thumbs.cpp"
    "src/View/Interface/Thumbs/texturesThumbs.cpp"
    "src/View/Interface/Thumbs/iconsThumbs.cpp"
    "src/View/Interface/Thumbs/meshesThumbs.cpp"
    )

# Dodanie bibliotek do aplikacji
# Dodaj biblioteki do kompilacji
target_link_libraries(${PROJECT_NAME}
   Vulkan::Vulkan
  glfw
  glm
  assimp
)
# Ustawienia dla kompilacji w systemie Linux
if(UNIX)
    target_link_libraries(${PROJECT_NAME} dl pthread)
endif()

# Ustawienie katalogu z plikami nagłówkowymi
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${Vulkan_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/include
    PRIVATE ${imgui_SOURCE_DIR})

# Zdefiniuj ścieżki do folderów 'resource' i 'core'
set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/resources")
set(CORE_DIR "${CMAKE_SOURCE_DIR}/core")

# Skopiuj folder 'resource' do folderu build w czasie konfiguracji
file(COPY ${RESOURCE_DIR} DESTINATION ${CMAKE_BINARY_DIR})

# Skopiuj folder 'core' do folderu build w czasie konfiguracji
file(COPY ${CORE_DIR} DESTINATION ${CMAKE_BINARY_DIR})

# Używamy CMAKE_SOURCE_DIR, aby uzyskać katalog główny projektu
add_definitions(-DPROJECT_DIR="${CMAKE_SOURCE_DIR}")

# Opcjonalne: Ustawienia kompilatora (dodatkowe flagi)
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()
