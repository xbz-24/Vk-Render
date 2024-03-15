/**
 * @file triangle_mesh.cpp
 * @brief
 * @date Created on 02-01-24.
 * @author Renato German Chavez Chicoma
 */
#include "triangle_mesh.hpp"

TriangleMesh::TriangleMesh(vk::Device logical_device, vk::PhysicalDevice physical_device)
{
    this->logical_device_ = logical_device;
    std::vector<float> vertices = {
        {
            0.0f, -0.05f, 0.0f, 1.0f, 0.0f,
            0.05f, 0.05f, 0.0f, 1.0f, 0.0f,
            -0.05f, 0.05f, 0.0f, 1.0f, 0.0f
        }
    };
    BufferInput inputChunk;
    inputChunk.logical_device = logical_device;
    inputChunk.physical_device = physical_device;
    inputChunk.size = sizeof(float) * vertices.size();
    inputChunk.usage = vk::BufferUsageFlagBits::eVertexBuffer;
    vertex_buffer = vkutil::createBuffer(inputChunk);
    void* memoryLocation = logical_device.mapMemory(vertex_buffer.buffer_memory, 0, inputChunk.size);
    memcpy(memoryLocation, vertices.data(), inputChunk.size);
    logical_device.unmapMemory(vertex_buffer.buffer_memory);
}
TriangleMesh::~TriangleMesh()
{
    logical_device_.destroyBuffer(vertex_buffer.buffer);
    logical_device_.freeMemory(vertex_buffer.buffer_memory);
}
