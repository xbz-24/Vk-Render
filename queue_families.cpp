//
// Created by daily on 27-12-23.
//

#include "queue_families.hpp"
namespace vkUtil {
    bool QueueFamilyIndices::isComplete()
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }

    /**
     * @brief Finds queue families that support specific capabilities on a physical device.
     *
     * Identifies and returns the indices of queue families that support graphics and presentation
     * capabilities on the specified physical device and surface.
     *
     * @param device The Vulkan physical device.
     * @param surface The Vulkan surface.
     * @param debug Flag indicating whether to enable debug logging.
     * @return QueueFamilyIndices with the indices of the suitable queue families.
     */
    QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device, vk::SurfaceKHR surface, bool debug) {
        QueueFamilyIndices indices;
        std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();
        if (debug) {
            std::cout << "System can support " << queueFamilies.size() << " queue families.\n";
        }
        int i = 0;
        for (vk::QueueFamilyProperties queueFamily: queueFamilies) {
            if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
                indices.graphicsFamily = i;
                indices.presentFamily = i;

                if (debug) {
                    std::cout << "Queue Family " << i << " is suitable for graphics.\n";
                }
            }
            if (device.getSurfaceSupportKHR(i, surface)) {
                indices.presentFamily = i;
                if (debug) {
                    std::cout << "Queue Family " << i << " is suitable for presenting.\n";
                }
            }
            if (indices.isComplete()) {
                break;
            }
            i++;
        }
        return indices;
    }
}
