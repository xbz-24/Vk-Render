//
// Created by daily on 27-12-23.
//

#ifndef INC_3DLOADERVK_LOGGING_HPP
#define INC_3DLOADERVK_LOGGING_HPP
#include <vulkan/vulkan.hpp>
#include <vector>
#include <string>
#include <iostream>

namespace vkinit{
    VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                 VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                 const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                 void* pUserData);
    vk::DebugUtilsMessengerEXT make_debug_messenger(vk::Instance& instance, 
                                                    vk::DispatchLoaderDynamic& dldi);
    std::vector<std::string> log_transform_bits(vk::SurfaceTransformFlagsKHR bits);
    std::vector<std::string> log_alpha_composite_bits(vk::CompositeAlphaFlagsKHR bits);
    std::vector<std::string> log_image_usage_bits(vk::ImageUsageFlags bits);
    std::string log_present_mode(vk::PresentModeKHR presentMode);
    void log_device_properties(const vk::PhysicalDevice& device);
}

#endif //INC_3DLOADERVK_LOGGING_HPP
