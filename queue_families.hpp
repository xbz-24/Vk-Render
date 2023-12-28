//
// Created by daily on 27-12-23.
//

#ifndef INC_3DLOADERVK_QUEUE_FAMILIES_HPP
#define INC_3DLOADERVK_QUEUE_FAMILIES_HPP
#include "config.hpp"

namespace vkUtil{
    struct QueueFamilyIndices{
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete(){
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    QueueFamilyIndices findQueueFamilies(vk::PhysicalDevice device, vk::SurfaceKHR surface, bool debug){
        QueueFamilyIndices indices;
        std::vector<vk::QueueFamilyProperties> queueFamilies = device.getQueueFamilyProperties();
        if(debug){
            std::cout << "System can support " << queueFamilies.size() << " queue families.\n";
        }

        int i = 0;
        for(vk::QueueFamilyProperties queueFamily : queueFamilies){
            if(queueFamily.queueFlags & vk::QueueFlagBits::eGraphics){
                indices.graphicsFamily = i;
                indices.presentFamily = i;

                if(debug){
                    std::cout << "Queue Family " << i <<" is suitable for graphics.\n";
                }
            }
            if(device.getSurfaceSupportKHR(i, surface)){
                indices.presentFamily = i;
                if(debug){
                    std::cout << "Queue Family " << i << " is suitable for presenting.\n";
                }
            }

            if(indices.isComplete()){
                break;
            }
            i++;
        }
        return indices;
    }

}
#endif //INC_3DLOADERVK_QUEUE_FAMILIES_HPP
