#include "FileManager.h"

namespace game {
	std::vector<int> FileManager::readFile(std::string fileName){
		std::vector<int> Result;
		
		std::ifstream inFile;
		inFile.open(fileName);

		if (inFile.fail()) {
			std::cerr << "Error opening file : " << fileName << std::endl;
			return Result;
		}

		int value;
		while (!inFile.eof()) {
			inFile >> value;
			Result.push_back(value);
		}

		inFile.close();
		return Result;
	}

	int FileManager::readSingleFile(std::string fileName) {
		int result;

		std::ifstream inFile;
		inFile.open(fileName);

		if (inFile.fail()) {
			std::cerr << "Error opening file : " << fileName << std::endl;
			return -1;
		}

		inFile >> result;

		inFile.close();
		return result;
	}

	void FileManager::writeFile(std::string fileName, std::vector<int> output) {
		std::ofstream outFile;
		outFile.open(fileName);

		if (outFile.fail()) {
			std::cerr << "Error writing to file : " << fileName << std::endl;
			return;
		}

		for (int i = 0; i < output.size(); i++) {
			if (i == output.size() - 1) outFile << output[i];
			else outFile << output[i] << std::endl;
		}

		outFile.close();
		return;
	}

	void FileManager::writeFile(std::string fileName, int output) {
		std::ofstream outFile;
		outFile.open(fileName);

		if (outFile.fail()) {
			std::cerr << "Error writing to file : " << fileName << std::endl;
			return;
		}

		outFile << output;

		outFile.close();
		return;
	}
}