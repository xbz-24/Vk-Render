#include "cube_mesh.hpp"

[[maybe_unused]] CubeMesh::CubeMesh(vk::Device logical_device, vk::PhysicalDevice physical_device) : logical_device_(logical_device) {
    std::vector<float> vertices =
    {
       -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
       -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
       -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
    };
    std::vector<uint32_t> indices =
    {
       0, 1, 2, 2, 3, 0,
       4, 5, 6, 6, 7, 4,
       0, 3, 7, 7, 4, 0,
       1, 2, 6, 6, 5, 1,
       3, 2, 6, 6, 7, 3,
       0, 1, 5, 5, 4, 0
    };
    BufferInput inputChunk;

    inputChunk.logical_device = logical_device;
    inputChunk.physical_device = physical_device;
    inputChunk.size = sizeof(float)* vertices.size();
    inputChunk.usage = vk::BufferUsageFlagBits::eVertexBuffer;

    vertex_buffer = vkutil::create_buffer(inputChunk);

    void* memoryLocation = logical_device_.mapMemory(vertex_buffer.buffer_memory, 0, inputChunk.size);
    memcpy(memoryLocation, vertices.data(), inputChunk.size);

    logical_device_.unmapMemory(vertex_buffer.buffer_memory);
}
CubeMesh::~CubeMesh() {
    logical_device_.destroyBuffer(vertex_buffer.buffer);
    logical_device_.freeMemory(vertex_buffer.buffer_memory);
}