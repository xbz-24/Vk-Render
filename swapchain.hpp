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
    struct SwapChainSupportDetails
    {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presentModes;
    };
    struct SwapChainBundle
    {
        vk::SwapchainKHR swapchain;
        std::vector<vkutil::SwapChainFrame> frames;
        vk::Format format;
        vk::Extent2D extent;
    };
    SwapChainSupportDetails query_swap_chain_support(vk::PhysicalDevice device,
                                                     vk::SurfaceKHR surface, bool debug);

    vk::SurfaceFormatKHR choose_swap_chain_surface_format(std::vector<vk::SurfaceFormatKHR> formats);

    vk::PresentModeKHR choose_swap_chain_present_mode(std::vector<vk::PresentModeKHR> presentModes);

    vk::Extent2D choose_swap_chain_extent(uint32_t width,
                                          uint32_t height,
                                          vk::SurfaceCapabilitiesKHR capabilities);

    SwapChainBundle create_swap_chain(vk::Device logicalDevice,
                                      vk::PhysicalDevice physicalDevice,
                                      vk::SurfaceKHR surface, int width, int height, bool debug);
}
#endif //INC_3DLOADERVK_SWAPCHAIN_HPP
