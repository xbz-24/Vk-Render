//
// Created by daily on 27-12-23.
//
#ifndef INC_3DLOADERVK_SHADERS_HPP
#define INC_3DLOADERVK_SHADERS_HPP
#include <vulkan/vulkan.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
namespace vkUtil
{
    std::vector<char> readFile(std::string filename, bool debug);
    vk::ShaderModule createModule(std::string filename, vk::Device device, bool debug);
}

#endif //INC_3DLOADERVK_SHADERS_HPP
