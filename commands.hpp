#ifndef INC_3DLOADERVK_COMMANDS_HPP
#define INC_3DLOADERVK_COMMANDS_HPP

#include <vulkan/vulkan.hpp>
#include <iostream>
#include "frame.hpp"
#include "queue_families.hpp"

namespace vkinit
{
    struct commandBufferInputChunk
    {
        vk::Device device;
        vk::CommandPool command_pool;
        std::vector<vkutil::SwapChainFrame>& frames;
    };

    vk::CommandPool make_command_pool(vk::Device device, vk::PhysicalDevice physical_device, vk::SurfaceKHR surface, bool debug);

    vk::CommandBuffer make_command_buffer(commandBufferInputChunk input_chunk, bool debug);

    void make_frame_command_buffer(commandBufferInputChunk input_chunk, bool debug);
}

#endif //INC_3DLOADERVK_COMMANDS_HPP
