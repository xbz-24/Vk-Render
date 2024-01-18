/**
 * @file mesh.hpp
 * @brief Provides functions for mesh handling in Vulkan.
 * @date Created by daily on 02-01-24.
 */

#ifndef INC_3DLOADERVK_MESH_HPP
#define INC_3DLOADERVK_MESH_HPP

#include <array>
#include <vulkan/vulkan.hpp>

namespace vkmesh
{
    vk::VertexInputBindingDescription getPosColorBindingDescription();
    std::array<vk::VertexInputAttributeDescription, 2> getPosColorAttributeDescriptions();
}

#endif //INC_3DLOADERVK_MESH_HPP