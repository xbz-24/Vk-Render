#ifndef INC_3DLOADERVK_QUEUE_FAMILIES_HPP
#define INC_3DLOADERVK_QUEUE_FAMILIES_HPP

#include <vulkan/vulkan.hpp>
#include <optional>
#include <iostream>

namespace vkutil
{
    struct QueueFamilyIndices
    {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        [[nodiscard]] bool is_complete() const;
    };

    QueueFamilyIndices find_queue_families(vk::PhysicalDevice device, vk::SurfaceKHR surface, bool debug);
}

#endif //INC_3DLOADERVK_QUEUE_FAMILIES_HPP
