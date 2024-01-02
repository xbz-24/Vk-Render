//
// Created by daily on 27-12-23.
//
/**
 * @file device.hpp
 * @brief Defines functionalities for Vulkan physical and logical device management.
 * @date Created by Renato on 27-12-23.
 */
#ifndef INC_3DLOADERVK_DEVICE_HPP
#define INC_3DLOADERVK_DEVICE_HPP

#include "config.hpp"
#include "logging.hpp"
#include "queue_families.hpp"
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
namespace vkInit {
    /**
     * @brief Checks if a Vulkan physical device supports the required extensions.
     *
     * @param device The Vulkan physical device to check.
     * @param requestedExtensions A vector of extension names to be checked.
     * @param debug Flag indicating whether to enable debug logging.
     * @return true if the device supports all requested extensions, false otherwise
     */
    bool checkDeviceExtensionSupport(
            const vk::PhysicalDevice& device,
            const std::vector<const char*>& requestedExtensions,
            const bool& debug
            ){
        std::set<std::string> requiredExtensions(requestedExtensions.begin(), requestedExtensions.end());
        if(debug){
            std::cout << "Device can support extensions:\n";
        }
        for(vk::ExtensionProperties& extension : device.enumerateDeviceExtensionProperties()){
            if(debug){
                std::cout << "\t\""<<extension.extensionName << "\"\n";
            }
            requiredExtensions.erase(extension.extensionName);
        }
        return requiredExtensions.empty();
    }
    /**
     * @brief Determines if a Vulkan physical device is suitable for the application's needs.
     *
     * @param device The vulkan physical device to evaluate.
     * @param debug Flag indicating whether to enable debug logging.
     * @return true if the device is suitable, false otherwise.
     */
    bool isSuitable(const vk::PhysicalDevice& device, const bool debug){
        if(debug){
            std::cout << "Checking if device is suitable\n";
        }
        const std::vector<const char*> requestedExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
        if(debug){
            std::cout << "We are requesting device extensions:\n";
            for(const char* extension : requestedExtensions){
                std::cout << "\t\"" << extension << "\"\n";
            }
        }
        if(bool extensionSupported = checkDeviceExtensionSupport(device, requestedExtensions, debug)){
            if(debug){
                std::cout << "Device can support the requested extensions!\n";
            }
        }
        else{
            if(debug){
                std::cout << "Device can't support the requested extensions!\n";
            }
            return false;
        }
        return true;
    }
    /**
     * @brief Chooses a suitable Vulkan physical device from available devices.
     *
     * @param instance The Vulkan instance.
     * @param debug Flag indicating whether to enable debug logging.
     * @return The chosen Vulkan physical device.
     */
    vk::PhysicalDevice choose_physical_device(vk::Instance& instance, bool debug){
        /**
         * Choose a suitable physical device from a list of candidates.
         * Note: Physical device are neither created nor destroyed, they exist
         * independently to the program.
         */
        if(debug){
            std::cout << "Choosing physical device...\n";
        }
        /**
         * ResultValueType<std::vector<PhysicalDevice, PhysicalDeviceAllocator>>::type
         * Instance::enumeratePhysicalDevices( Dispatch const & d )
         * std::vector<vk::PhysicalDevice> instance.enumeratePhysicalDevices( Dispatch const & d = static/default )
         */
        std::vector<vk::PhysicalDevice>availableDevices = instance.enumeratePhysicalDevices();
        if(debug){
            std::cout << "There are " << availableDevices.size() << " physical devices available on this system\n";
        }
        for(vk::PhysicalDevice device : availableDevices){
            if(debug){
                log_device_properties(device);
            }
            if(isSuitable(device, debug)){
                return device;
            }
        }

        return nullptr;
    }
    /**
     * @brief Creates a Vulkan logical device from a physical device.
     *
     * @param physicalDevice The Vulkan physical device.
     * @param surface The Vulkan surface.
     * @param debug The Vulkan surface.
     * @return The created Vulkan logical device.
     */
    vk::Device create_logical_device(vk::PhysicalDevice physicalDevice,vk::SurfaceKHR surface, bool debug){
        vkUtil::QueueFamilyIndices indices = vkUtil::findQueueFamilies(physicalDevice,surface, debug);

        std::vector<uint32_t>uniqueIndices;
        uniqueIndices.push_back(indices.graphicsFamily.value());
        if(indices.graphicsFamily.value() != indices.presentFamily.value()){
            uniqueIndices.push_back(indices.presentFamily.value());
        }

        float queuePriority = 1.0f;
        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfo;
        for(uint32_t queueFamilyIndex : uniqueIndices){
            queueCreateInfo.push_back(
            vk::DeviceQueueCreateInfo(
                    vk::DeviceQueueCreateFlags(), indices.graphicsFamily.value(),
                    1, &queuePriority
            ));
        }

        std::vector<const char*> deviceExtensions = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };


        vk::PhysicalDeviceFeatures deviceFeatures = vk::PhysicalDeviceFeatures();
        std::vector<const char*> enabledLayers;
        if(debug){
            enabledLayers.push_back("VK_LAYER_KHRONOS_validation");
        }
        vk::DeviceCreateInfo deviceInfo = vk::DeviceCreateInfo(
            vk::DeviceCreateFlags(),
            queueCreateInfo.size(),
            queueCreateInfo.data(),
            enabledLayers.size(), enabledLayers.data(),
            deviceExtensions.size(),
            deviceExtensions.data(),
            &deviceFeatures
        );
        try{
            vk::Device device = physicalDevice.createDevice(deviceInfo);
            if(debug){
                std::cout << "GPU has been successfully abstracted!\n";
            }
            return device;
        }catch(vk::SystemError err){
            if(debug){
                std::cout << "Device creation failed!\n";
                return nullptr;
            }
        }
        return nullptr;
    }
    /**
     * @brief Retrieves graphics and presentation queues from a Vulkan device.
     *
     * @param physicalDevice The Vulkan physical device.
     * @param device The Vulkan logical device.
     * @param surface The Vulkan surface.
     * @param debug Flag indicating whether to enable debug logging.
     * @return An array containing the graphics and presentation queues.
     */
    std::array<vk::Queue, 2> get_queues(vk::PhysicalDevice physicalDevice, vk::Device device,vk::SurfaceKHR surface, bool debug){
        vkUtil::QueueFamilyIndices indices = vkUtil::findQueueFamilies(physicalDevice, surface, debug);

        return {{
            device.getQueue(indices.graphicsFamily.value(), 0),
            device.getQueue(indices.presentFamily.value(), 0)
        }};
    }
}
#endif //INC_3DLOADERVK_DEVICE_HPP