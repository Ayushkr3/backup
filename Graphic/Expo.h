#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <array>
#include "Mouse.h"
std::array<float, 2> KeepAlive() {
	// Open the file for reading

	std::ifstream file("D:\\program\\vs\\CheckData\\delta_values.txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open file" << std::endl;
	}
	// Read data from the file
	 // Vector to store the extracted integers
	std::array<float, 2> xy = { 0,0 };
	std::string line;
	while (std::getline(file, line)) { // Read each line from the file
		std::stringstream ss(line); // Create a stringstream from the line

		std::string word;
		for (int i = 0; i < 2; ++i) { // Extract the first two words separated by comma
			if (std::getline(ss, word, ',')) {
				try {
					xy[i] = std::stof(word);
				 // Convert word to integer and add to vector
				}
				catch (const std::invalid_argument& e) {
					std::cerr << "Error converting string to integer: " << e.what() << std::endl;
					// Handle invalid input here
				}
				catch (const std::out_of_range& e) {
					std::cerr << "Error converting string to integer: " << e.what() << std::endl;
					// Handle out of range input here
				}
			}
		}
	}

	// Output the received data

	// Close the file
	file.close();
	return xy;
}

