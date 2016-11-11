#pragma once

#include <iostream>
#include <fstream>
#include <string>

class Utils {
public:
	static bool read(const std::string &path, std::string &output) {
		std::ifstream ifs(path);
		if (ifs.is_open()) {
			char buff[512];
			while (ifs.getline(buff, sizeof(buff)) ){
				output.append(buff);
				output.append("\n");
			}
			ifs.close();
			return true;
		}
		else {
			std::cout << "open file: \"" << path << "\" error!" << std::endl;
		}

		return false;
	}
};