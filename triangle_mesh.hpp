//
// Created by daily on 02-01-24.
//
#ifndef INC_3DLOADERVK_TRIANGLE_MESH_HPP
#define INC_3DLOADERVK_TRIANGLE_MESH_HPP
#include "config.hpp"
#include "memory.hpp"
#include <vulkan/vulkan.hpp>

class TriangleMesh
{
public:
    TriangleMesh(vk::Device logical_device , vk::PhysicalDevice physical_device);
    ~TriangleMesh();
    Buffer vertex_buffer;
private:
    vk::Device logical_device_;
};

#endif //INC_3DLOADERVK_TRIANGLE_MESH_HPP
