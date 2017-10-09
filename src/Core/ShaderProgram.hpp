//
// Created by MTesseracT on 9-10-2017.
//

#ifndef VULKANENGINE_SHADERPROGRAM_HPP
#define VULKANENGINE_SHADERPROGRAM_HPP

#include <vector>
#include <string>
#include <iostream>
#include <fstream>


class ShaderProgram
{
public:

private:
    std::vector<char> ReadShader(const std::string& p_filename);
};


#endif //VULKANENGINE_SHADERPROGRAM_HPP
