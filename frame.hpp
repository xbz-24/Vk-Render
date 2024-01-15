//
// Created by daily on 27-12-23.
//
/**
 * @file frame.hpp
 * @brief Defines the SwapChainFrame structure for managing Vulkan swap chain frames.
 * @date Created by Renato on 27-12-23.
 */
#ifndef INC_3DLOADERVK_FRAME_HPP
#define INC_3DLOADERVK_FRAME_HPP
#include <vulkan/vulkan.hpp>
/**
 * @namespace vkUtil
 * @brief Contains utility structures and functions for Vulkan.
 *
 * This namespace provides additional support for managing Vulkan objects and operations,
 * supplementing the core Vulkan functionalities.
 */
namespace vkUtil
{
    /**
     * @struct SwapChainFrame
     * @brief Holds the components necessary for a single frame in a Vulkan swap chain.
     *
     * This structure includes an image, an image view, a framebuffer, command buffer,
     * and synchronization objects for a frame. These components are essential for rendering
     * and presenting each frame in a Vulkan application.
     */
    struct SwapChainFrame
    {
        vk::Image image;
        vk::ImageView imageView;
        vk::Framebuffer framebuffer;
        vk::CommandBuffer commandbuffer;
        vk::Semaphore imageAvailable;
        vk::Semaphore renderFinished;
        vk::Fence inFlight;
    };
}


#endif //INC_3DLOADERVK_FRAME_HPP
