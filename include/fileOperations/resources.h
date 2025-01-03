#pragma once
#include "fileOperations/filesFinding.h"
#include <vector>
#include <string>
#include <mutex>
namespace fileOperations {



    class FilesManager {
    public:

        // Publiczny dost�p do instancji singletona
        static FilesManager& getInstance() {
            static FilesManager instance; // Instancja singletona
            return instance;
        }

        // Dost�p do plik�w
        filesPaths& getMeshesNames() { return meshesNames; }
        filesPaths& getTexturesNames() { return texturesNames; }
        filesPaths& getCppNames() { return cppNames; }
        filesPaths& getDllNames() { return dllNames; }
        filesPaths& getFontNames() { return FontNames; }

    private:
        // Prywatne zmienne
        filesPaths meshesNames;
        filesPaths texturesNames;
        filesPaths cppNames;
        filesPaths dllNames;
        filesPaths FontNames;

        // Prywatne konstruktory dla singletona
        FilesManager() = default;
        ~FilesManager() = default;

        // Usuni�cie mo�liwo�ci kopiowania i przenoszenia
        FilesManager(const FilesManager&) = delete;
        FilesManager& operator=(const FilesManager&) = delete;
        FilesManager(FilesManager&&) = delete;
        FilesManager& operator=(FilesManager&&) = delete;
    };
}
	