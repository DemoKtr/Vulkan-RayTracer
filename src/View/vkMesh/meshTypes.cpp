#include "View/vkMesh/meshTypes.h"
#include <windows.h>

void make_model_index(modelNames& data) {

    uint32_t index = -1;
    data.modelIndex[""] = index++;

    for (std::string path : data.fullPaths) {
        data.modelIndex[path] = index++;
    }

}

void hash_model_paths(modelNames& data) {

    std::hash<std::string> hasher;
    for (std::string path : data.fullPaths) {
        data.hash[path] = hasher(path);
    }
}

void listMeshesFilesInDirectory(const std::string& path, modelNames& data) {
    char ch[MAX_PATH];
    std::string project_path = std::string(PROJECT_DIR);
    std::string final_path = project_path + path;
    try {

        
        // Rekurencyjne przeszukiwanie folderów i podfolderów
        for (const auto& entry : std::filesystem::recursive_directory_iterator(final_path)) {
            // SprawdŸ, czy element jest plikiem
            if (std::filesystem::is_regular_file(entry.path())) {
                // Pobierz rozszerzenie pliku
                std::string extension = entry.path().extension().string();

                // SprawdŸ, czy rozszerzenie to ".obj" lub ".fbx"
                if (extension == ".obj" || extension == ".fbx") {
                    // Dodaj pe³n¹ œcie¿kê do wektora
                    data.fullPaths.push_back(entry.path().string());

                    // Dodaj nazwê pliku bez rozszerzenia do drugiego wektora
                    data.fileNames.push_back(entry.path().stem().string());
                }
            }
        }
    }
    catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    make_model_index(data);
    hash_model_paths(data);
}
