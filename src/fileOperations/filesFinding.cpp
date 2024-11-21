#include "fileOperations/filesFinding.h"
#include <windows.h>


void fileOperations::make_model_index(filesPaths& data) {

    uint32_t index = -1;
    data.Index[""] = index++;

    for (std::string path : data.fullPaths) {
        data.Index[path] = index++;
    }

}

void fileOperations::hash_model_paths(filesPaths& data) {

    std::hash<std::string> hasher;
    size_t k = 0;
    for (std::string path : data.fullPaths) {
        uint64_t h = hasher(path);
        data.hash[path] = h;
        data.hashToIndex[h] = k++;

    }
}

void fileOperations::list_files_in_directory(const std::string& path, filesPaths& data, std::vector<std::string> extensions) {
    char ch[MAX_PATH];
    std::string project_path = std::string(PROJECT_DIR);
    std::string final_path = project_path + path;
    try {

        
        // Rekurencyjne przeszukiwanie folderów i podfolderów
        for (const auto& entry : std::filesystem::recursive_directory_iterator(final_path)) {
            // Sprawdź, czy element jest plikiem
            if (std::filesystem::is_regular_file(entry.path())) {
                // Pobierz rozszerzenie pliku
                std::string extension = entry.path().extension().string();

                 // Sprawdź, czy rozszerzenie znajduje się w liście rozszerzeń
                if (std::find(extensions.begin(), extensions.end(), extension) != extensions.end()) {
                    // Dodaj pełną ścieżkę do wektora
                    data.fullPaths.push_back(entry.path().string());

                    // Dodaj nazwę pliku bez rozszerzenia do drugiego wektora
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


