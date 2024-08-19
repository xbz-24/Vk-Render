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