#ifndef INC_3DLOADERVK_COMMANDS_HPP
#define INC_3DLOADERVK_COMMANDS_HPP

#include <vulkan/vulkan.hpp>
#include <iostream>
#include "frame.hpp"
#include "queue_families.hpp"

namespace vkInit
{
    struct commandBufferInputChunk
    {
        vk::Device device;
        vk::CommandPool commandPool;
        std::vector<vkUtil::SwapChainFrame>& frames;
    };

    vk::CommandPool make_command_pool(vk::Device device, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, bool debug);

    vk::CommandBuffer make_command_buffer(commandBufferInputChunk inputChunk, bool debug);

    void make_frame_command_buffer(commandBufferInputChunk inputChunk, bool debug);
}

#endif //INC_3DLOADERVK_COMMANDS_HPP
