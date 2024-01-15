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
    TriangleMesh(vk::Device logicalDevice , vk::PhysicalDevice physicalDevice);
    ~TriangleMesh();
    Buffer vertexBuffer;
private:
    vk::Device logicalDevice;
};

#endif //INC_3DLOADERVK_TRIANGLE_MESH_HPP
