#ifndef INC_3DLOADERVK_QUAD_MESH_HPP
#define INC_3DLOADERVK_QUAD_MESH_HPP

#include <vulkan/vulkan.hpp>
#include <vector>
#include "memory.hpp"

class QuadMesh
{
public:
    QuadMesh(vk::Device logical_device, vk::PhysicalDevice physical_device);
    ~QuadMesh();
    Buffer vertex_buffer;
private:
    vk::Device logical_device_;
};

#endif //INC_3DLOADERVK_QUAD_MESH_HPP
