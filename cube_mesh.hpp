#ifndef VKLOADER_CUBE_MESH_HPP
#define VKLOADER_CUBE_MESH_HPP

#include <vulkan/vulkan.h>
#include <vector>
#include "memory.hpp"

class [[maybe_unused]] CubeMesh {
public:
    [[maybe_unused]] CubeMesh(vk::Device logical_device, vk::PhysicalDevice physical_device);
    ~CubeMesh();
    Buffer vertex_buffer;
private:
    vk::Device logical_device_;
};


#endif //VKLOADER_CUBE_MESH_HPP
