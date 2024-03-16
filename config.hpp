//
// Created by daily on 27-12-23.
//
#ifndef INC_3DLOADERVK_CONFIG_HPP
#define INC_3DLOADERVK_CONFIG_HPP
#include <vulkan/vulkan.hpp>
#include "vma.hpp"

struct BufferInput{
    size_t size;
    vk::BufferUsageFlags usage;
    vk::Device logical_device;
    vk::PhysicalDevice physical_device;
};

struct Buffer {
    VkBuffer buffer;
    VmaAllocation allocation;
};

#endif //INC_3DLOADERVK_CONFIG_HPP
