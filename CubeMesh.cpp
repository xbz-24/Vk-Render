//
// Created by daily on 19-01-24.
//

#include "CubeMesh.hpp"

CubeMesh::CubeMesh(vk::Device logical_device, vk::PhysicalDevice physical_device) {
    this->logical_device_ = logical_device;

    /* vk primitive topology triangle list */

    std::vector<float> vertices = {
            // positions          // colors
            -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, // Back face
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,

            -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // Front face
            0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
    };

    std::vector<uint32_t> indices = {
            // Back face
            0, 1, 2, 2, 3, 0,
            // Front face
            4, 5, 6, 6, 7, 4,
            // Left face
            0, 3, 7, 7, 4, 0,
            // Right face
            1, 2, 6, 6, 5, 1,
            // Top face
            3, 2, 6, 6, 7, 3,
            // Bottom face
            0, 1, 5, 5, 4, 0
    };

    BufferInput inputChunk;
    inputChunk.logical_device = logical_device;
    inputChunk.physical_device = physical_device;
    inputChunk.size = sizeof(float)* vertices.size();
    inputChunk.usage = vk::BufferUsageFlagBits::eVertexBuffer;
    vertex_buffer = vkutil::createBuffer(inputChunk);
    void* memoryLocation = logical_device_.mapMemory(vertex_buffer.buffer_memory, 0, inputChunk.size);
    memcpy(memoryLocation, vertices.data(), inputChunk.size);
    logical_device_.unmapMemory(vertex_buffer.buffer_memory);
}

CubeMesh::~CubeMesh() {
    logical_device_.destroyBuffer(vertex_buffer.buffer);
    logical_device_.freeMemory(vertex_buffer.buffer_memory);
}