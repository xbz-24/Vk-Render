//
// Created by daily on 27-12-23.
//

#ifndef INC_3DLOADERVK_COMMANDS_H
#define INC_3DLOADERVK_COMMANDS_H
#include "config.hpp"
#include "queue_families.hpp"

namespace vkInit {

    struct commandBufferInputChunk{
        vk::Device device;
        vk::CommandPool commandPool;
        std::vector<vkUtil::SwapChainFrame>& frames;
    };

    vk::CommandPool make_command_pool(vk::Device device, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, bool debug){
        vkUtil::QueueFamilyIndices queueFamilyIndices = vkUtil::findQueueFamilies(physicalDevice, surface, debug);
        vk::CommandPoolCreateInfo poolInfo = {};
        poolInfo.flags = vk::CommandPoolCreateFlags() | vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

        try{
            return device.createCommandPool(poolInfo);
        }
        catch(vk::SystemError err){
            if(debug){
                std::cout << "Failed to create Command Pool" << std::endl;
            }
            return nullptr;
        }
    }

    vk::CommandBuffer make_command_buffers(commandBufferInputChunk inputChunk, bool debug){
        vk::CommandBufferAllocateInfo allocInfo = { };
        allocInfo.commandPool = inputChunk.commandPool;
        allocInfo.level = vk::CommandBufferLevel::ePrimary;
        allocInfo.commandBufferCount = 1;

        for(int i = 0; i < inputChunk.frames.size(); i++){
            try{
                inputChunk.frames[i].commandbuffer = inputChunk.device.allocateCommandBuffers(allocInfo)[0];
            }
            catch(vk::SystemError err){
                if(debug){
                    std::cout << "Failed to allocate command buffer for frame " << i << std::endl;
                }
            }
        }
        try{
            vk::CommandBuffer commandbuffer = inputChunk.device.allocateCommandBuffers(allocInfo)[0];
            if(debug){
                std::cout << "Allocated main command buffer" << std::endl;
            }

            return commandbuffer;
        }
        catch(vk::SystemError err){
            if(debug){
                std::cout << "Failed to allocate main command buffer" << std::endl;
            }

            return nullptr;
        }
    }
}



#endif //INC_3DLOADERVK_COMMANDS_H
