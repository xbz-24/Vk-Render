#include "queue_families.hpp"

namespace vkutil
{
    bool QueueFamilyIndices::is_complete() const
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }

    QueueFamilyIndices find_queue_families(vk::PhysicalDevice device, vk::SurfaceKHR surface, bool debug)
    {
        QueueFamilyIndices indices;
        std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();
        if (debug)
        {
            std::cout << "System can support " << queueFamilies.size() << " queue families.\n";
        }
        int i = 0;
        for (vk::QueueFamilyProperties queueFamily: queueFamilies)
        {
            if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
            {
                indices.graphicsFamily = i;
                indices.presentFamily = i;

                if (debug)
                {
                    std::cout << "Queue Family " << i << " is suitable for graphics.\n";
                }
            }
            if (device.getSurfaceSupportKHR(static_cast<uint32_t>(i), surface))
            {
                indices.presentFamily = i;
                if (debug)
                {
                    std::cout << "Queue Family " << i << " is suitable for presenting.\n";
                }
            }
            if (indices.is_complete())
            {
                break;
            }
            i++;
        }
        return indices;
    }
}
