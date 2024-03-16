/**
 * @file device.cpp
 * @brief Defines functions for creating Vulkan devices.
 * @author Renato German Chavez Chicoma
 * @date Created on 27-12-23.
 */
#include "commands.hpp"
namespace vkinit {
    /**
     * @brief Creates a Vulkan command pool.
     *
     * Sets up a Vulkan command pool using the provided device_, physical device_, and surface_.
     * It is used for allocating command buffers.
     *
     * @param device The Vulkan logical device_.
     * @param physical_device The Vulkan physical device_.
     * @param surface The Vulkan surface_.
     * @param debug Flag indicating whether to enable debug logging.
     * @return A Vulkan command pool object.
     */
    VulkanResult<vk::CommandPool> make_command_pool(vk::Device device,
                                                     vk::PhysicalDevice physical_device,
                                                     vk::SurfaceKHR surface,
                                                     bool debug){
        vkutil::QueueFamilyIndices queueFamilyIndices = vkutil::findQueueFamilies(physical_device, 
                                                                                  surface,
                                                                                  debug);
        vk::CommandPoolCreateInfo poolInfo = {
        };
//        poolInfo.flags = vk::CommandPoolCreateFlags() |
//                         vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
        poolInfo.flags = vk::CommandPoolCreateFlags();
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
        try {
            return VulkanResult<vk::CommandPool>(device.createCommandPool(poolInfo));
        }
        catch(const vk::SystemError &err) {
            std::string errorMessage = std::format("Failed to create Command Pool: {}",err.what());
            if (debug) {
                std::cout << std::format("{}\n", errorMessage);
            }
            return VulkanResult<vk::CommandPool>(errorMessage);
        }
    }
    /**
     * @brief Allocates Vulkan command buffers.
     *
     * Allocates and sets up primary level command buffers for each frame in the input chunk.
     * It also allocates a main command buffer for the awpplication.
     *
     * @param input_chunk The command buffer input parameters.
     * @param debug Flag indicating whether to enable debug logging.
     * @return The main Vulkan command buffer.
     */
    VulkanResult<vk::CommandBuffer> make_command_buffer(commandBufferInputChunk input_chunk,
                                                         bool debug){
        vk::CommandBufferAllocateInfo allocInfo = {
        };
        allocInfo.commandPool = input_chunk.command_pool;
        allocInfo.level = vk::CommandBufferLevel::ePrimary;
        allocInfo.commandBufferCount = 1;
        try {
            if(debug) {
                std::cout << std::format("Allocating main Command Buffer\n");
            }
            return VulkanResult<vk::CommandBuffer>(input_chunk.device.allocateCommandBuffers(allocInfo)[0]);
        }
        catch (const vk::SystemError &err) {
            std::string errorMessage = std::format("Failed to create Command Buffer: {}", err.what());
            if(debug){
                std::cout << std::format("{}\n", errorMessage);
            }
            return VulkanResult<vk::CommandBuffer>(errorMessage);
        }
    }
    void make_frame_command_buffer(commandBufferInputChunk input_chunk, 
                                   bool debug) {
        vk::CommandBufferAllocateInfo allocInfo = {
        };
        allocInfo.commandPool = input_chunk.command_pool;
        allocInfo.level = vk::CommandBufferLevel::ePrimary;
        allocInfo.commandBufferCount = 1;
        for (std::vector<vkutil::SwapChainFrame>::size_type i = 0; i < input_chunk.frames.size(); i++) {
            try {
                input_chunk.frames[i].commandbuffer = input_chunk.device.allocateCommandBuffers(allocInfo)[0];
            }
            catch (vk::SystemError &err) {
                if (debug) {
                    std::cout << std::format("Failed to allocate command buffer for frame {}\n", i);
                }
            }
        }

    }
}
