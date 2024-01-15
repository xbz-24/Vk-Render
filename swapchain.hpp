//
// Created by daily on 27-12-23.
//
/**
 * @file swapchain_.hpp
 * @brief Defines functionality for creating and managing Vulkan swap chains.
 * @date Created by Renato on 27-12-23.
 */
#ifndef INC_3DLOADERVK_SWAPCHAIN_HPP
#define INC_3DLOADERVK_SWAPCHAIN_HPP

#include "logging.hpp"
#include "queue_families.hpp"
#include "frame.hpp"
#include <vector>
#include <vulkan/vulkan.hpp>
#include <iostream>

namespace vkinit
{
    /**
     * @struct SwapChainSupportDetails
     * @brief Holds detailed information about the swap chain support of a physical device_.
     *
     * This structure is used to store the capabilities, surface_ formats, and present modes
     * supported by a physical device_ for swap chain creation.
     */
    struct SwapChainSupportDetails
    {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presentModes;
    };
    /**
     * @struct SwapChainBundle
     * @brief Contains the Vulkan swap chain and its associated frames, format, and extent.
     *
     * This structure encapsulates a swap chain object and its related components, including
     * the frames (images and views), image format, and the extent of the images.
     */
    struct SwapChainBundle
    {
        vk::SwapchainKHR swapchain;
        std::vector<vkutil::SwapChainFrame> frames;
        vk::Format format;
        vk::Extent2D extent;
    };

    /**
     * @brief Queries the swap chain support details for a given physical device_ and surface_.
     *
     * Gathers and returns detailed information about the supported capabilities, formats, and
     * present modes for swap chain creation on the specified physical device_ and surface_.
     *
     * @param device The Vulkan physical device_.
     * @param surface The Vulkan surface_.
     * @param debug Flag indicating whether to enable debug logging.
     * @return SwapChainSupportDetails for the given device_ and surface_.
     */
    SwapChainSupportDetails query_swapchain_support(vk::PhysicalDevice device, vk::SurfaceKHR surface, bool debug);
    /**
     * @brief Chooses the most suitable surface_ format for the swap chain.
     *
     * Selects an appropriate surface_ format from a list of available formats, preferring
     * B8G8R8A8 format with sRGB color space if available
     *
     * @param formats A vector of available surface_ formats.
     * @return The chosen vk::SurfaceFormatKHR
     */
    vk::SurfaceFormatKHR choose_swapchain_surface_format(std::vector<vk::SurfaceFormatKHR> formats);

    /**
     * @brief Chooses the best available present mode for the swap chain.
     *
     * Selects a presentation mode from the available modes, with a preference for
     * 'mailbox' mode, which supports triple buffering.
     *
     * @param presentModes A vector of available presentation modes.
     * @return The chosen vk::PresentModeKHR.
     */
    vk::PresentModeKHR choose_swapchain_present_mode(std::vector<vk::PresentModeKHR> presentModes);

    /**
     * @brief Determines the extent of the swap chain images.
     *
     * Calculates and returns the extent (resolution) of the swap chain images,
     * based on the window_ size and surface_ capabilities
     *
     * @param width The width_ of the window_.
     * @param height The height_ of the window_.
     * @param capabilities The surface_ capabilities.
     * @return The vk::Extent2D representing the swap chain image extent.
     */
    vk::Extent2D choose_swapchain_extent(uint32_t width, uint32_t height, vk::SurfaceCapabilitiesKHR capabilities);

    /**
     * @brief Creates a Vulkan swap chain and associated resources.
     *
     * Initializes the Vulkan swap chain for image presentation, including setting up
     * the image views for each frame in the swap chain.
     *
     * @param logicalDevice The Vulkan logical device_.
     * @param physicalDevice The Vulkan physical device_.
     * @param surface The Vulkan surface_.
     * @param width The width_ of the window_.
     * @param height The height_ of the window_.
     * @param debug Flag indicating whether to enable debug logging.
     * @return A SwapChainBlundle containing the swap chain and its related components.
     */
    SwapChainBundle create_swapchain(vk::Device logicalDevice, vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface, int width, int height, bool debug);
}
#endif //INC_3DLOADERVK_SWAPCHAIN_HPP
