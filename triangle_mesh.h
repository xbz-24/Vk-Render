//
// Created by daily on 02-01-24.
//

#ifndef INC_3DLOADERVK_TRIANGLE_MESH_H
#define INC_3DLOADERVK_TRIANGLE_MESH_H
#include "config.hpp"
#include "memory.h"

class TriangleMesh{
public:
    TriangleMesh(vk::Device logicalDevice , vk::PhysicalDevice physicalDevice);
    ~TriangleMesh();
    Buffer vertexBuffer;
private:
    vk::Device logicalDevice;
};

#endif //INC_3DLOADERVK_TRIANGLE_MESH_H
