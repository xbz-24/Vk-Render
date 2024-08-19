#ifndef INC_3DLOADERVK_INSTANCE_HPP
#define INC_3DLOADERVK_INSTANCE_HPP
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <vector>
#include <iostream>

namespace vkinit
{
    bool supported(std::vector<const char*>& extensions, std::vector<const char*>& layers, bool debug);

    vk::Instance make_instance(bool debug, const char* applicationName);
}
#endif //INC_3DLOADERVK_INSTANCE_HPP