#include "utils.h"
#include <fstream>
#include <iostream>

std::string readFile(std::string filename) {
	std::string result;
	std::ifstream is(filename, std::ifstream::binary);
	if(is) {
		is.seekg(0, is.end);
		int length = is.tellg();
		is.seekg(0, is.beg);
		char* buffer = new char[length + 1];
		is.read(buffer, length);
		if(is) {
			std::cout << "read successfully" << std::endl;
			std::string stringData(buffer, length);
			delete[] buffer;
			return stringData;
		}
		else {
			std::cout << "error, only " << is.gcount() << " could be read" << std::endl;
		}
	
	}
	else {
		std::cout << "error reading" << std::endl;
		return "";
	}

	return result;
}

std::vector<unsigned short> parseData(std::string stringData) {
	std::vector<unsigned short> data;
	for(unsigned int i = 0; i < stringData.size(); i += 2) {
		unsigned short value = *((unsigned short*) &stringData[i]);
		data.push_back(value);
	}
	return data;
}
