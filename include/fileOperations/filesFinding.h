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
        std::unordered_map<std::string, uint32_t> Index;
        std::unordered_map<std::string, uint64_t> hash;
        std::unordered_map<uint64_t, uint32_t> hashToIndex;



        int getIndex(std::string path) {
            return Index[path];
        }
        int getIndex(uint64_t hash) {
            return hashToIndex[hash];
        }
    };


    void make_model_index(filesPaths& data);

    void hash_model_paths(filesPaths& data);

    void list_files_in_directory(const std::string& path, filesPaths& data, std::vector<std::string> extensions);
}


