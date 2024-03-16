/**
 * @file device_.hpp
 * @brief Defines functionalities for Vulkan physical and logical device_ management.
 * @date Created by Renato on 27-12-23.
 */
#ifndef INC_3DLOADERVK_DEVICE_HPP
#define INC_3DLOADERVK_DEVICE_HPP
#include <vulkan/vulkan.hpp>
#include <iostream>
#include <vector>
#include <format>
#include <set>
#include <string>
#include <array>
#include <optional>
#include <ranges>
#include "queue_families.hpp"
#include "logging.hpp"
#include <functional>
/**
 * Vulkan represents the concept of physic  al and logical devices.
 *
 * A physical device_ usually represents a single complete implementation of Vulkan
 * (excluding instance_-level functionality) available to the host,
 * of which there are a finite number.
 *
 * A logical device_ represents an instance_ of that implementation
 * with its own state and resources independent of other logical devices.
 */
 /**
  * @namespace vkinit
  * @brief Checks if a Vulkan physical device_ support the required extensions.
  *
  * This namespace includes functions for checking device_ extension support,
  * determining if a physical device_ is suitable, choosing a physical device_,
  * creating a logical device_ and retrieving device_ queues.
  */
namespace vkinit{
    bool CheckDeviceExtensionSupport(const vk::PhysicalDevice& device,
                                     const std::vector<const char*>& requested_extensions,
                                     const bool& debug);
    bool IsSuitable(const vk::PhysicalDevice& device, 
                    bool debug);
    std::optional<vk::PhysicalDevice> ChoosePhysicalDevice(vk::Instance& instance, 
                                            bool debug);
    std::optional<vk::Device> CreateLogicalDevice(vk::PhysicalDevice physical_device,
                                   vk::SurfaceKHR surface,
                                   bool debug);
    std::array<vk::Queue, 2> GetQueues(vk::PhysicalDevice physical_device, 
                                       vk::Device device,
                                       vk::SurfaceKHR surface,
                                       bool debug);

}
#endif //INC_3DLOADERVK_DEVICE_HPP
