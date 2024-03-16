/**
 * @file mesh.cpp
 * @brief Implements functions for mesh handling in Vulkan.
 * @date Created on 02-01-24.
 * @author Renato German Chavez Chicoma
 */
#include "mesh.hpp"

namespace vkmesh{
    vk::VertexInputBindingDescription getPosColorBindingDescription(){
        vk::VertexInputBindingDescription bindingDescription{
        };
        bindingDescription.binding = 0;
        bindingDescription.stride = 5 * sizeof(float);
        bindingDescription.inputRate = vk::VertexInputRate::eVertex;
        return bindingDescription;
    }
    std::array<vk::VertexInputAttributeDescription,2> getPosColorAttributeDescriptions(){
        std::array<vk::VertexInputAttributeDescription,2> attributes {
        };
        // Position
        attributes[0].binding = 0;
        attributes[0].location = 0;
        attributes[0].format = vk::Format::eR32G32B32A32Sfloat;
        attributes[0].offset = 0;
        // Color
        attributes[1].binding = 0;
        attributes[1].location = 1;
        attributes[1].format = vk::Format::eR32G32B32A32Sfloat;
        attributes[1].offset = 2 * sizeof(float);
        return attributes;
    }
}
