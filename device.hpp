#ifndef INC_3DLOADERVK_DEVICE_HPP
#define INC_3DLOADERVK_DEVICE_HPP

#include <vulkan/vulkan.hpp>
#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <array>
#include "queue_families.hpp"
#include "logging.hpp"

namespace vkinit
{
    bool check_device_extension_support(const vk::PhysicalDevice& device,
                                        const std::vector<const char*>& requested_extensions,
                                        const bool& debug);

    bool is_suitable(const vk::PhysicalDevice& device, bool debug);
    vk::PhysicalDevice choose_physical_device(vk::Instance& instance, bool debug);
    vk::Device create_logical_device(vk::PhysicalDevice physical_device, vk::SurfaceKHR surface, bool debug);
    std::array<vk::Queue, 2> get_queues(vk::PhysicalDevice physical_device, vk::Device device, vk::SurfaceKHR surface, bool debug);
}
#endif //INC_3DLOADERVK_DEVICE_HPP