#pragma once

#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>



namespace vkImage {
    struct TexturesNames {
        std::vector<std::string> fileNames;
        std::vector<std::string> fullPaths;
        std::unordered_map<std::string, int> modelIndex;



        int getModelIndex(std::string path) {
            return modelIndex[path];
        }
    };


    void make_texture_index(TexturesNames& data);

    void hash_texture_paths(TexturesNames& data);

    void listTexturesFilesInDirectory(const std::string& path, TexturesNames& data);

}

