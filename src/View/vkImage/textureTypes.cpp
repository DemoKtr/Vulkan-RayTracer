#include "View/vkImage/textureTypes.h"

void vkImage::make_texture_index(TexturesNames& data) {
    uint32_t index = -1;
    data.modelIndex[""] = index++;

    for (std::string path : data.fullPaths) {
        data.modelIndex[path] = index++;
    }
}

void vkImage::hash_texture_paths(TexturesNames& data){

}

void vkImage::listTexturesFilesInDirectory(const std::string& path, TexturesNames& data){
    char ch[256];
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
                if (extension == ".png" || extension == ".jpg") {
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

    make_texture_index(data);

}


