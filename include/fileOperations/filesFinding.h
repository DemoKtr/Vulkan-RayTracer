#pragma once


#include <iostream>
#include <filesystem>
#include <string>
#include <vector>
#include <unordered_map>


namespace fileOperations {
    struct filesPaths {
        std::vector<std::string> fileNames;
        std::vector<std::string> fullPaths;
        std::unordered_map<std::string, int> modelIndex;
        std::unordered_map<std::string, uint64_t> hash;


        int getModelIndex(std::string path) {
            return modelIndex[path];
        }
    };


    void make_model_index(filesPaths& data);

    void hash_model_paths(filesPaths& data);

    void listMeshesFilesInDirectory(const std::string& path, filesPaths& data, std::vector<std::string> extensions);
}


