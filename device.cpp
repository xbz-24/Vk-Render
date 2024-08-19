#include "device.hpp"

bool vkinit::check_device_extension_support(const vk::PhysicalDevice& device,
                                            const std::vector<const char*>& requested_extensions,
                                            const bool& debug)
{
    auto start = requested_extensions.begin();
    auto end = requested_extensions.end();

    std::set<std::string> requiredExtensions(start ,end);

    if(debug)
    {
        std::cout << "Device can support extensions:\n";
    }

    for(const vk::ExtensionProperties& extension : device.enumerateDeviceExtensionProperties())
    {
        if(debug)
        {
            std::cout << "\t\""<<extension.extensionName << "\"\n";
        }
        requiredExtensions.erase(extension.extensionName);
    }
    return requiredExtensions.empty();
}
bool vkinit::is_suitable(const vk::PhysicalDevice& device, const bool debug)
{
    if(debug)
    {
        std::cout << "Checking if device_ is suitable\n";
    }

    const std::vector<const char*> requestedExtensions =
    {
       VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };

    if(debug)
    {
        std::cout << "We are requesting device_ extensions:\n";
        for(const char* extension : requestedExtensions)
        {
            std::cout << "\t\"" << extension << "\"\n";
        }
    }

    bool extensionSupported = check_device_extension_support(device,
                                                             requestedExtensions, debug);

    if(extensionSupported)
    {
        if(debug)
        {
            std::cout << "Device can support the requested extensions!\n";
        }
    }
    else
    {
        if(debug)
        {
            std::cout << "Device can't support the requested extensions!\n";
        }
        return false;
    }
    return true;
}
vk::PhysicalDevice vkinit::choose_physical_device(vk::Instance& instance, bool debug)
{
    if(debug)
    {
        std::cout << "Choosing physical device_...\n";
    }

    std::vector<vk::PhysicalDevice> availableDevices = instance.enumeratePhysicalDevices();

    if(debug)
    {
        std::cout << "There are "
                  << availableDevices.size()
                  << " physical devices available on this system\n";
    }

    for(vk::PhysicalDevice device : availableDevices)
    {
        if(debug)
        {
            log_device_properties(device);
        }
        if(is_suitable(device, debug))
        {
            return device;
        }
    }
    // TODO: add more logging and validation (just giving random nullptr is not safe)
    // ideally create a function that returns true false do not use exceptions
    return nullptr;
}
vk::Device vkinit::create_logical_device(vk::PhysicalDevice physical_device,
                                         vk::SurfaceKHR surface, bool debug)
{
    vkutil::QueueFamilyIndices indices = vkutil::find_queue_families(physical_device,
                                                                     surface, debug);

    std::vector<uint32_t> uniqueIndices;
    uniqueIndices.push_back(indices.graphicsFamily.value());

    if(indices.graphicsFamily.value() != indices.presentFamily.value())
    {
        uniqueIndices.push_back(indices.presentFamily.value());
    }

    float queuePriority = 1.0F;
    std::vector<vk::DeviceQueueCreateInfo> queueCreateInfo;
    for([[maybe_unused]] uint32_t queueFamilyIndex : uniqueIndices)
    {
        queueCreateInfo.emplace_back
        (
            vk::DeviceQueueCreateFlags(),
            indices.graphicsFamily.value(),
            1,
            &queuePriority
        );
    }

    std::vector<const char*> deviceExtensions =
    {
       VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };


    vk::PhysicalDeviceFeatures deviceFeatures = vk::PhysicalDeviceFeatures();

    std::vector<const char*> enabledLayers;

    if(debug)
    {
        enabledLayers.push_back("VK_LAYER_KHRONOS_validation");
    }

    vk::DeviceCreateInfo deviceInfo = vk::DeviceCreateInfo(
        vk::DeviceCreateFlags(),
        static_cast<uint32_t>(queueCreateInfo.size()),
        queueCreateInfo.data(),
        static_cast<uint32_t>(enabledLayers.size()),
        enabledLayers.data(),
        static_cast<uint32_t>(deviceExtensions.size()),
        deviceExtensions.data(),
        &deviceFeatures
    );

    try
    {
        vk::Device device = physical_device.createDevice(deviceInfo);
        if(debug)
        {
            std::cout << "GPU has been successfully abstracted!\n";
        }
        return device;
    }
    catch(const vk::SystemError &err)
    {
        if(debug)
        {
            std::cout << "Device creation failed!\n";
            // TODO: add more messaging and error handling without using exceptions
            return nullptr;
        }
    }

    return nullptr;
}
std::array<vk::Queue, 2> vkinit::get_queues(vk::PhysicalDevice physical_device, vk::Device device, vk::SurfaceKHR surface, bool debug)
{
    vkutil::QueueFamilyIndices indices = vkutil::find_queue_families(physical_device, surface, debug);
    return
    {
        {
            device.getQueue(indices.graphicsFamily.value(), 0),
            device.getQueue(indices.presentFamily.value(), 0)
        }
    };
}