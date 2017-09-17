//
// Created by MTesseracT on 15-9-2017.
//

#ifndef VULKANENGINE_UTILITY_HPP
#define VULKANENGINE_UTILITY_HPP

#include <vector>
#include <string>
#include <fstream>
#include <exception>

class Utility
{
public:
    static std::vector<char> ReadFile(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error("failed to open file!");
        }

        std::cout << "Found file: " << filename << ", Reading it now!" << std::endl;

        size_t fileSize = (size_t) file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return buffer;
    }
};

#endif //VULKANENGINE_UTILITY_HPP
