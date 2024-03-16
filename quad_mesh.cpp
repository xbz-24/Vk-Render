/**
 * @file quad_mesh.cpp
 * @brief
 * @date Created on 19-01-24.
 * @author Renato German Chavez Chicoma
 */
#include "quad_mesh.hpp"

QuadMesh::QuadMesh(vk::Device logical_device, 
                   vk::PhysicalDevice physical_device,
                   VmaAllocator allocator){
    this->logical_device_ = logical_device;
    this->allocator_ = allocator;
    std::vector<float> vertices = {
        //eTriangleList
        //            -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        //            0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        //            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        //
        //            0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        //            0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
        //            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
        
        //eTriangleStrip
            -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f
    };
    BufferInput inputChunk;
    inputChunk.logical_device = logical_device;
    inputChunk.physical_device = physical_device;
    inputChunk.size = sizeof(float) * vertices.size();
    inputChunk.usage = vk::BufferUsageFlagBits::eVertexBuffer;
    
    vertex_buffer = vkutil::createBuffer(allocator, inputChunk);
    
    void* memoryLocation;
    vmaMapMemory(allocator, vertex_buffer.allocation, &memoryLocation);
    memcpy(memoryLocation, vertices.data(), inputChunk.size);
    vmaUnmapMemory(allocator, vertex_buffer.allocation);
}

QuadMesh::~QuadMesh(){
    //    logical_device_.destroyBuffer(vertex_buffer.buffer);
    //    logical_device_.freeMemory(vertex_buffer.buffer);
//    vmaDestroyBuffer(allocator_, vertex_buffer.buffer, vertex_buffer.allocation);
}
