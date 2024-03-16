#ifndef INC_3DLOADERVK_INSTANCE_HPP
#define INC_3DLOADERVK_INSTANCE_HPP
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>
#include <vector>
#include <span>
#include <iostream>
#include <format>
namespace vkinit{
    /**
        Check whether the requested extensions and layers are supported.

        \param extensions a list of extension names being requested.
        \param layers a list of layer names being requested.
        \param debug whether to log error messages.
        \returns whether all of the extensions and layers are supported.
    */
    bool supported(std::span<const char* const> extensions,
                   std::span<const char* const> layers,
                   bool debug);

    /**
        Create a Vulkan instance_.

        \param debug whether the system is being run in debug mode.
        \param applicationName the name of the application.
        \returns the instance_ created.
    */
    vk::Instance make_instance(bool debug, 
                               const char* applicationName);
}
#endif //INC_3DLOADERVK_INSTANCE_HPP
