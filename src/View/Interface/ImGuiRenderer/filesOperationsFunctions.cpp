#include "View/Interface/ImGuiRenderer/filesOperationsFunctions.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_vulkan.h>
#include <thread>
#include "fstream"
#include <atomic>
#include "fileOperations/files.h"
#include <Scene/Objects/PrefabManager.h>
#include <fileOperations/serializations.h>

void vkImGui::render_file_save_window(std::string path,fileOperations::FileType::Type type,bool& isOpen) {
      // Flaga monitoruj¹ca tworzenie pliku
    static char buffer[128] = "";
    std::string filename;
   
    ImGui::OpenPopup("Create Script");
    if (ImGui::BeginPopup("Create Script")) {
        ImGui::Text("Enter the name of the file:");


        ImGui::InputText("##Filename", buffer, IM_ARRAYSIZE(buffer));

        if (ImGui::Button("Create")) {
            filename = buffer;
            
            create_file_function(type,path,filename);

            isOpen = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
            isOpen = false;
        }

        ImGui::EndPopup();
    }
    std::cout << "oja" << std::endl;
}

void vkImGui::create_file_function(fileOperations::FileType::Type type, std::string& path, std::string& filename) {

    std::vector<std::string> extensions = fileOperations::FileType::getExtensions(type);

    for (std::string ext : extensions) {
        std::string fullPath = path + "//" + filename + ext;
        if (fileOperations::file_exists(fullPath)) {
            std::cout << "Plik ju¿ istnieje: " << fullPath << std::endl;
        }
        else {
            std::ofstream file(fullPath);
            switch (type) {

            case fileOperations::FileType::Type::Script: {

                if (ext == ".cpp") {
                    file << "#include \"\ " + filename + ext;
                }
                file.close();
            }
            case fileOperations::FileType::Type::Prefab: {
                file.close();
                vkPrefab::PrefabManager& prefabManager = vkPrefab::PrefabManager::getInstance();
                prefabManager.createPrefab(fullPath,true);                
                std::cout << "test1" << std::endl;
                fileOperations::saveToFile(fullPath, prefabManager.getActivePrefab(fullPath).get()->getScene());
                std::cout << "test3" << std::endl;
            }

            default: {

            }

                   if (!file) {
                       std::cerr << "Nie uda³o siê utworzyæ pliku: " << fullPath << std::endl;
                   }
                   // Tworzenie pustego pliku
            }
        }


    }
    std::cout << "jaciesunem" << std::endl;
}