#ifndef INC_3DLOADERVK_SHADERS_HPP
#define INC_3DLOADERVK_SHADERS_HPP

#include <vulkan/vulkan.hpp>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

namespace vkutil
{
    std::vector<char> read_file(std::string filename, bool debug);
    vk::ShaderModule create_module(const std::string& filename, vk::Device device, bool debug);
}

#endif //INC_3DLOADERVK_SHADERS_HPP
