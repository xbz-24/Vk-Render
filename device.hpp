/**
 * @file device.hpp
 * @brief Defines functionalities for Vulkan physical and logical device management.
 * @date Created by Renato on 27-12-23.
 */
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
/**
 * Vulkan represents the concept of physic  al and logical devices.
 *
 * A physical device usually represents a single complete implementation of Vulkan
 * (excluding instance-level functionality) available to the host,
 * of which there are a finite number.
 *
 * A logical device represents an instance of that implementation
 * with its own state and resources independent of other logical devices.
 */
 /**
  * @namespace vkInit
  * @brief Checks if a Vulkan physical device support the required extensions.
  *
  * This namespace includes functions for checking device extension support,
  * determining if a physical device is suitable, choosing a physical device,
  * creating a logical device and retrieving device queues.
  */
namespace vkInit
{
    bool checkDeviceExtensionSupport
    (
        const vk::PhysicalDevice& device,
        const std::vector<const char*>& requestedExtensions,
        const bool& debug
    );
    bool isSuitable(const vk::PhysicalDevice& device, const bool debug);
    vk::PhysicalDevice choose_physical_device(vk::Instance& instance, bool debug);
    vk::Device create_logical_device(vk::PhysicalDevice physicalDevice,vk::SurfaceKHR surface, bool debug);
    std::array<vk::Queue, 2> get_queues(vk::PhysicalDevice physicalDevice, vk::Device device,vk::SurfaceKHR surface, bool debug);

}
#endif //INC_3DLOADERVK_DEVICE_HPP