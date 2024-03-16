/**
 * @file sync.cpp
 * @brief
 * @date Created on 27-12-23.
 * @author Renato German Chavez Chicoma
 */
#include "sync.hpp"
namespace vkinit {
    /**
     * @brief Creates a Vulkan semaphore.
     *
     * Initializes and returns a new semaphore object. Semaphores are used to synchronize operations
     * within or across command queues.
     *
     * @param device The Vulkan logical device_ used to create the semaphore.
     * @param debug Flag indicating whether to enable debug logging.
     * @return A Vulkan semaphore object, or nullptr if creation fails.
     */
    vk::Semaphore make_semaphore(vk::Device device, 
                                 bool debug) {
        vk::SemaphoreCreateInfo semaphoreInfo = {
        };
        semaphoreInfo.flags = vk::SemaphoreCreateFlags();
        try {
            return device.createSemaphore(semaphoreInfo);
        }
        catch(vk::SystemError &err) {
            if(debug){
                std::cout << std::format("Failed to create semaphore\n");
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
     * @param device The Vulkan logical device_ used to create the semaphore.
     * @param debug Flag indicating whether to enable debug logging.
     * @return A Vulkan fence object, or nullptr if creation fails.
     */
    vk::Fence make_fence(vk::Device device, bool debug){
        vk::FenceCreateInfo fenceInfo = {
        };
        fenceInfo.flags = vk::FenceCreateFlags() | vk::FenceCreateFlagBits::eSignaled;
        try {
            return device.createFence(fenceInfo);
        }
        catch (vk::SystemError &err) {
            if(debug) {
                std::cout << std::format("Failed to create fence\n");
            }
            return nullptr;
        }
    }
}
