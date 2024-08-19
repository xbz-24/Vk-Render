#include "commands.hpp"

namespace vkinit
{
    vk::CommandPool make_command_pool(vk::Device device, vk::PhysicalDevice physical_device, vk::SurfaceKHR surface, bool debug)
    {
        vkutil::QueueFamilyIndices queueFamilyIndices = vkutil::find_queue_families(physical_device, surface, debug);
        vk::CommandPoolCreateInfo poolInfo = { };
        poolInfo.flags = vk::CommandPoolCreateFlags() | vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

        try
        {
            return device.createCommandPool(poolInfo);
        }
        catch(const vk::SystemError &err)
        {
            if(debug)
            {
                std::cout << "Failed to create Command Pool" << std::endl;
            }
            return nullptr;
        }
    }
    vk::CommandBuffer make_command_buffer(CommandBufferInputChunk input_chunk, bool debug)
    {
        vk::CommandBufferAllocateInfo allocInfo = { };
        allocInfo.commandPool = input_chunk.command_pool;
        allocInfo.level = vk::CommandBufferLevel::ePrimary;
        allocInfo.commandBufferCount = 1;

        try
        {
            vk::CommandBuffer commandbuffer = input_chunk.device.allocateCommandBuffers(allocInfo)[0];
            if(debug)
            {
                std::cout << "Allocated main command buffer" << std::endl;
            }

            return commandbuffer;
        }
        catch(const vk::SystemError &err)
        {
            if(debug)
            {
                std::cout << "Failed to allocate main command buffer" << std::endl;
            }

            return nullptr;
        }
    }
    void make_frame_command_buffer(CommandBufferInputChunk input_chunk, bool debug)
    {
        vk::CommandBufferAllocateInfo allocInfo = { };
        allocInfo.commandPool = input_chunk.command_pool;
        allocInfo.level = vk::CommandBufferLevel::ePrimary;
        allocInfo.commandBufferCount = 1;

        for(std::vector<vkutil::SwapChainFrame>::size_type i = 0; i < input_chunk.frames.size(); i++)
        {
            try
            {
                input_chunk.frames[i].commandbuffer = input_chunk.device.allocateCommandBuffers(allocInfo)[0];
            }
            catch(const vk::SystemError &err)
            {
                if(debug)
                {
                    std::cout << "Failed to allocate command buffer for frame " << i << std::endl;
                }
            }
        }

    }
}