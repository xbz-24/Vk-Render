//
// Created by daily on 27-12-23.
//
#ifndef INC_3DLOADERVK_CONFIG_HPP
#define INC_3DLOADERVK_CONFIG_HPP
#include <vulkan/vulkan.hpp>

struct BufferInput
{
    size_t size;
    vk::BufferUsageFlags usage;
    vk::Device logicalDevice;
    vk::PhysicalDevice physicalDevice;
};

struct Buffer
{
    vk::Buffer buffer;
    vk::DeviceMemory bufferMemory;
};

#endif //INC_3DLOADERVK_CONFIG_HPP