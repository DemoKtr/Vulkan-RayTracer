#include "fileOperations/files.h"
#include <iostream>
#include <fstream>
#include <string>

bool fileOperations::file_exists(const std::string& file_path) {

	std::ifstream file(file_path);
	return file.good();  // Zwraca true, jeœli plik istnieje

}
