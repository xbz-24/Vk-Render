/**
 * @file swapchain.cpp
 * @brief
 * @date Created on 27-12-23.
 * @author Renato German Chavez Chicoma
 */
#include "swapchain.hpp"
namespace vkinit {
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
    SwapChainSupportDetails query_swapchain_support(vk::PhysicalDevice device, 
                                                    vk::SurfaceKHR surface,
                                                    bool debug) {
        SwapChainSupportDetails support;
        support.capabilities = device.getSurfaceCapabilitiesKHR(surface);

        if (debug) {
            std::cout << "Swapchain can support the following surface_ cappabilities\n";
            std::cout << std::format("\tminimum image count: {}\n", support.capabilities.minImageCount);
            std::cout << std::format("\tmaximum image count: {}\n", support.capabilities.maxImageCount);
            std::cout << std::format("\tcurrent extent: \n");
            //vk::Extent2D extent(230, 234);
            std::cout << std::format("\t\twidth: {}\n", support.capabilities.currentExtent.width);
            std::cout << std::format("\t\theight: {}\n", support.capabilities.currentExtent.height);

            std::cout << std::format("\t\tminimum supported extent: \n");
            std::cout << std::format("\t\twidth: {}\n", support.capabilities.minImageExtent.width);
            std::cout << std::format("\t\theight: {}\n", support.capabilities.minImageExtent.height);

            std::cout << std::format("\t\tmaximum supported extent: \n");
            std::cout << std::format("\t\twidth: {}\n", support.capabilities.maxImageExtent.width);
            std::cout << std::format("\t\theight: {}\n", support.capabilities.maxImageExtent.height);

            std::cout << std::format("\tmaxximum image array layers: {}\n", support.capabilities.maxImageArrayLayers);

            std::cout << "\tSupported transforms:\n";
            std::vector<std::string> stringList = log_transform_bits(support.capabilities.supportedTransforms);
            for (std::string &line : stringList | std::views::all) {
                std::cout << "\t\t" << line << std::endl;
            }

            std::cout << "\tcurrent transform:\n";
            stringList = log_transform_bits(support.capabilities.currentTransform);
            for (std::string &line : stringList | std::views::all) {
                std::cout << "\t\t" << line << std::endl;
            }

            std::cout << "\tsupported alpha operations:\n";
            stringList = log_alpha_composite_bits(support.capabilities.supportedCompositeAlpha);
            for (std::string &line : stringList | std::views::all) {
                std::cout << "\t\t" << line << std::endl;
            }

            std::cout << "\tsupported image usage:\n";
            stringList = log_image_usage_bits(support.capabilities.supportedUsageFlags);
            for (std::string &line : stringList | std::views::all) {
                std::cout << "\t\t" << line << std::endl;
            }
        }

        support.formats = device.getSurfaceFormatsKHR(surface);
        if (debug){
            std::ranges::for_each(support.formats, [](const vk::SurfaceFormatKHR &supportedFormat) {
                std::cout << "supported pixel format: " << vk::to_string(supportedFormat.format) << '\n';
                std::cout << "supported color space: " << vk::to_string(supportedFormat.colorSpace) << '\n';
            });
//            for (const vk::SurfaceFormatKHR &supportedFormat: support.formats){
//                std::cout << "supported pixel format: " << vk::to_string(supportedFormat.format) << '\n';
//                std::cout << "supported color space: " << vk::to_string(supportedFormat.colorSpace) << '\n';
//            }
        }
        support.presentModes = device.getSurfacePresentModesKHR(surface);
        for (vk::PresentModeKHR presentMode: support.presentModes){
            std::cout << '\t' << log_present_mode(presentMode) << '\n';
        }
        return support;
    }

    /**
     * @brief Chooses the most suitable surface_ format for the swap chain.
     *
     * Selects an appropriate surface_ format from a list of available formats, preferring
     * B8G8R8A8 format with sRGB color space if available
     *
     * @param formats A vector of available surface_ formats.
     * @return The chosen vk::SurfaceFormatKHR
     */
    vk::SurfaceFormatKHR choose_swapchain_surface_format(std::vector<vk::SurfaceFormatKHR> formats){
        for (vk::SurfaceFormatKHR format: formats){
            if (format.format == vk::Format::eB8G8R8A8Unorm && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear){
                return format;
            }
        }
        return formats[0];
    }

    /**
     * @brief Chooses the best available present mode for the swap chain.
     *
     * Selects a presentation mode from the available modes, with a preference for
     * 'mailbox' mode, which supports triple buffering.
     *
     * @param presentModes A vector of available presentation modes.
     * @return The chosen vk::PresentModeKHR.
     */
    vk::PresentModeKHR choose_swapchain_present_mode(std::vector<vk::PresentModeKHR> presentModes){
        for (vk::PresentModeKHR presentMode: presentModes){
            if (presentMode == vk::PresentModeKHR::eMailbox){
                return presentMode;
            }
        }
        return vk::PresentModeKHR::eFifo;
    }

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
    vk::Extent2D choose_swapchain_extent(uint32_t width, uint32_t height, vk::SurfaceCapabilitiesKHR capabilities){
        if (capabilities.currentExtent.width != UINT32_MAX){
            return capabilities.currentExtent;
        }
        else{
            vk::Extent2D extent = {width, height};
            extent.width = std::min
                    (
                            capabilities.maxImageExtent.width,
                            std::max(capabilities.minImageExtent.width, width)
                    );
            extent.height = std::min
                    (
                            capabilities.maxImageExtent.height,
                            std::max(capabilities.minImageExtent.height,height)
                    );
            return extent;
        }
    }

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
    SwapChainBundle create_swapchain(vk::Device logicalDevice, 
                                     vk::PhysicalDevice physicalDevice,
                                     vk::SurfaceKHR surface,
                                     int width,
                                     int height,
                                     bool debug){
        SwapChainSupportDetails support = query_swapchain_support(physicalDevice,
                                                                  surface,
                                                                  debug);
        vk::SurfaceFormatKHR format = choose_swapchain_surface_format(support.formats);
        vk::PresentModeKHR presentMode = choose_swapchain_present_mode(support.presentModes);
        vk::Extent2D extent = choose_swapchain_extent(static_cast<uint32_t>(width), 
                                                      static_cast<uint32_t>(height), support.capabilities);
        uint32_t imageCount = std::min(support.capabilities.maxImageCount,
                                       support.capabilities.minImageCount + 1);

        vk::SwapchainCreateInfoKHR createInfo = vk::SwapchainCreateInfoKHR(vk::SwapchainCreateFlagsKHR(),
                                                                           surface,
                                                                           imageCount,
                                                                           format.format,
                                                                           format.colorSpace,
                                                                           extent,
                                                                           1,
                                                                           vk::ImageUsageFlagBits::eColorAttachment);
        vkutil::QueueFamilyIndices indices = vkutil::findQueueFamilies(physicalDevice,
                                                                       surface,
                                                                       debug);
        uint32_t queueFamilyIndices[] = {
            indices.graphicsFamily.value(),
            indices.presentFamily.value()
        };

        if(indices.graphicsFamily.value() != indices.presentFamily.value()) {
            createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else{
            createInfo.imageSharingMode = vk::SharingMode::eExclusive;
        }

        createInfo.preTransform = support.capabilities.currentTransform;
        createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        createInfo.oldSwapchain = vk::SwapchainKHR(nullptr);

        SwapChainBundle bundle{ };
        try {
            bundle.swapchain = logicalDevice.createSwapchainKHR(createInfo);
        }
        catch (vk::SystemError &err) {
            throw std::runtime_error("failed to create swapchain_!");
        }
        std::vector<vk::Image> images = logicalDevice.getSwapchainImagesKHR(bundle.swapchain);
        bundle.frames.resize(images.size());

        for (size_t i = 0; i < images.size(); i++) {
            vk::ImageViewCreateInfo imageViewCreateInfo = {
            };
            imageViewCreateInfo.image = images[i];
            imageViewCreateInfo.viewType = vk::ImageViewType::e2D;
            imageViewCreateInfo.format = format.format;
            imageViewCreateInfo.components.r = vk::ComponentSwizzle::eIdentity;
            imageViewCreateInfo.components.g = vk::ComponentSwizzle::eIdentity;
            imageViewCreateInfo.components.b = vk::ComponentSwizzle::eIdentity;
            imageViewCreateInfo.components.a = vk::ComponentSwizzle::eIdentity;
            imageViewCreateInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
            imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
            imageViewCreateInfo.subresourceRange.levelCount = 1;
            imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
            imageViewCreateInfo.subresourceRange.layerCount = 1;

            bundle.frames[i].image = images[i];
            bundle.frames[i].imageView = logicalDevice.createImageView(imageViewCreateInfo);
        }
        bundle.format = format.format;
        bundle.extent = extent;
        return bundle;
    }
};
