//
// Created by daily on 19-01-24.
//

#ifndef VKLOADER_CUBEMESH_HPP
#define VKLOADER_CUBEMESH_HPP
#include <vulkan/vulkan.h>
#include <vector>
#include "memory.hpp"

class CubeMesh {
public:
    CubeMesh(vk::Device logical_device, vk::PhysicalDevice physical_device);
    ~CubeMesh();
    Buffer vertex_buffer;
private:
    vk::Device logical_device_;
};


#endif //VKLOADER_CUBEMESH_HPP
