/**
 * @file commands.hpp
 * @brief Provides utilities for creating Vulkan command pools and command buffers.
 * @date Created by Renato on 27-12-23.
 */
#ifndef INC_3DLOADERVK_COMMANDS_HPP
#define INC_3DLOADERVK_COMMANDS_HPP
#include <vulkan/vulkan.hpp>
#include <iostream>
#include "frame.hpp"
#include "queue_families.hpp"
/**
 * @namespace vkInit
 * @brief Provides utilities for creating Vulkan command pools and command buffers.
 * @date Created by Renato on 27-12-23.
 */
namespace vkInit
{
    /**
     * @struct commandBufferInputChunk
     * @brief Holds parameters required for creating Vulkan command buffers.
     *
     * This structure aggregates a Vulkan device, a command pool, and a vector of swap chain frames
     * to provide necessary inputs for command buffer allocation.
     */
    struct commandBufferInputChunk
    {
        vk::Device device;
        vk::CommandPool commandPool;
        std::vector<vkUtil::SwapChainFrame>& frames;
    };
    /**
     * @brief Creates a Vulkan command pool.
     *
     * Sets up a Vulkan command pool using the provided device, physical device, and surface.
     * It is used for allocating command buffers.
     *
     * @param device The Vulkan logical device.
     * @param physicalDevice The Vulkan physical device.
     * @param surface The Vulkan surface.
     * @param debug Flag indicating whether to enable debug logging.
     * @return A Vulkan command pool object.
     */
    vk::CommandPool make_command_pool(vk::Device device, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, bool debug)
    {
        vkUtil::QueueFamilyIndices queueFamilyIndices = vkUtil::findQueueFamilies(physicalDevice, surface, debug);
        vk::CommandPoolCreateInfo poolInfo = { };
        poolInfo.flags = vk::CommandPoolCreateFlags() | vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

        try
        {
            return device.createCommandPool(poolInfo);
        }
        catch(vk::SystemError &err)
        {
            if(debug)
            {
                std::cout << "Failed to create Command Pool" << std::endl;
            }
            return nullptr;
        }
    }
    /**
     * @brief Allocates Vulkan command buffers.
     *
     * Allocates and sets up primary level command buffers for each frame in the input chunk.
     * It also allocates a main command buffer for the awpplication.
     *
     * @param inputChunk The command buffer input parameters.
     * @param debug Flag indicating whether to enable debug logging.
     * @return The main Vulkan command buffer.
     */
    vk::CommandBuffer make_command_buffer(commandBufferInputChunk inputChunk, bool debug)
    {
        vk::CommandBufferAllocateInfo allocInfo = { };
        allocInfo.commandPool = inputChunk.commandPool;
        allocInfo.level = vk::CommandBufferLevel::ePrimary;
        allocInfo.commandBufferCount = 1;

        try
        {
            vk::CommandBuffer commandbuffer = inputChunk.device.allocateCommandBuffers(allocInfo)[0];
            if(debug)
            {
                std::cout << "Allocated main command buffer" << std::endl;
            }

            return commandbuffer;
        }
        catch(vk::SystemError &err)
        {
            if(debug)
            {
                std::cout << "Failed to allocate main command buffer" << std::endl;
            }

            return nullptr;
        }
    }
    void make_frame_command_buffer(commandBufferInputChunk inputChunk, bool debug)
    {
        vk::CommandBufferAllocateInfo allocInfo = { };
        allocInfo.commandPool = inputChunk.commandPool;
        allocInfo.level = vk::CommandBufferLevel::ePrimary;
        allocInfo.commandBufferCount = 1;

        for(int i = 0; i < inputChunk.frames.size(); i++)
        {
            try
            {
                inputChunk.frames[i].commandbuffer = inputChunk.device.allocateCommandBuffers(allocInfo)[0];
            }
            catch(vk::SystemError &err)
            {
                if(debug)
                {
                    std::cout << "Failed to allocate command buffer for frame " << i << std::endl;
                }
            }
        }

    }
}
#endif //INC_3DLOADERVK_COMMANDS_HPP
