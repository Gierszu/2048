#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

namespace game {
	class FileManager {
	public:
		FileManager() {}
		~FileManager() {}

		std::vector<int> readFile(std::string fileName);
		int readSingleFile(std::string fileName);
		void writeFile(std::string fileName, std::vector<int> output);
		void writeFile(std::string fileName, int output);

	private:

	};
}