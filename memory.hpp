#ifndef INC_3DLOADERVK_MEMORY_HPP
#define INC_3DLOADERVK_MEMORY_HPP

#include "config.hpp"
#include <vulkan/vulkan.hpp>

namespace vkutil
{
    Buffer create_buffer(BufferInput input);
    uint32_t find_memory_type_index(vk::PhysicalDevice physicalDevice, uint32_t supportedMemoryIndices, vk::MemoryPropertyFlags requestedProperties);
    void allocate_buffer_memory(Buffer &buffer, const BufferInput& input);
}
#endif //INC_3DLOADERVK_MEMORY_HPP
