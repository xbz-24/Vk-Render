//
// Created by daily on 19-01-24.
//

#ifndef INC_3DLOADERVK_QUAD_MESH_HPP
#define INC_3DLOADERVK_QUAD_MESH_HPP
#include <vulkan/vulkan.hpp>
#include <vector>
#include "memory.hpp"
#include "config.hpp"
#include "vma.hpp"


class QuadMesh
{
public:
    QuadMesh(vk::Device logical_device, 
             vk::PhysicalDevice physical_device,
             VmaAllocator allocator);
    ~QuadMesh();
    Buffer vertex_buffer;
private:
    vk::Device logical_device_;
    VmaAllocator allocator_;
};

#endif //INC_3DLOADERVK_QUAD_MESH_HPP
