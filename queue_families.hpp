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

        [[nodiscard]] bool isComplete() const;
    };

    QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device, vk::SurfaceKHR surface, bool debug);
}

#endif //INC_3DLOADERVK_QUEUE_FAMILIES_HPP
