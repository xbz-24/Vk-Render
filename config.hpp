#ifndef INC_3DLOADERVK_CONFIG_HPP
#define INC_3DLOADERVK_CONFIG_HPP

#include <vulkan/vulkan.hpp>

struct BufferInput
{
    size_t size;
    vk::BufferUsageFlags usage;
    vk::Device logical_device;
    vk::PhysicalDevice physical_device;
};

struct Buffer
{
    vk::Buffer buffer;
    vk::DeviceMemory buffer_memory;
};

#endif //INC_3DLOADERVK_CONFIG_HPP