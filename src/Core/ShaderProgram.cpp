//
// Created by MTesseracT on 9-10-2017.
//

#include "ShaderProgram.hpp"

std::vector<char> ShaderProgram::ReadShader(const std::string &p_filename)
{
    std::ifstream file(p_filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("failed to open file!");
    }

    std::cout << "Found file: " << p_filename << ", Reading it now!" << std::endl;

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}
