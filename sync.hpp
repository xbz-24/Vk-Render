//
// Created by daily on 27-12-23.
//
/**
 * @file sync.hpp
 * @brief Defines functions for creating Vulkan synchronization primitives.
 * @date Created by Renato on 27-12-23.
 */
#ifndef INC_3DLOADERVK_SYNC_HPP
#define INC_3DLOADERVK_SYNC_HPP
#include "config.hpp"

namespace vkInit {
    /**
     * @brief Creates a Vulkan semaphore.
     *
     * Initializes and returns a new semaphore object. Semaphores are used to synchronize operations
     * within or across command queues.
     *
     * @param device The Vulkan logical device used to create the semaphore.
     * @param debug Flag indicating whether to enable debug logging.
     * @return A Vulkan semaphore object, or nullptr if creation fails.
     */
    vk::Semaphore make_semaphore(vk::Device device, bool debug){
        vk::SemaphoreCreateInfo semaphoreInfo = { };
        semaphoreInfo.flags = vk::SemaphoreCreateFlags();
        try{
            return device.createSemaphore(semaphoreInfo);
        }
        catch(vk::SystemError err){
            if(debug){
                std::cout << "Failed to create semaphore" << std::endl;
            }
            return nullptr;
        }
    }
    /**
     * @brief Creates a Vulkan fence
     *
     * Initializes and returns a new semaphore object. Semaphores are used to synchronize operations
     * within or across command queues.
     *
     * @param device The Vulkan logical device used to create the semaphore.
     * @param debug Flag indicating whether to enable debug logging.
     * @return A Vulkan fence object, or nullptr if creation fails.
     */
    vk::Fence make_fence(vk::Device device, bool debug){
        vk::FenceCreateInfo fenceInfo = { };
        fenceInfo.flags = vk::FenceCreateFlags() | vk::FenceCreateFlagBits::eSignaled;

        try{
            return device.createFence(fenceInfo);
        }
        catch(vk::SystemError err){
            if(debug){
                std::cout << "Failed to create fence" << std::endl;
            }
            return nullptr;
        }
    }
}


#endif //INC_3DLOADERVK_SYNC_HPP
