//
// Created by daily on 02-01-24.
//

#ifndef INC_3DLOADERVK_MEMORY_HPP
#define INC_3DLOADERVK_MEMORY_HPP
#include "config.hpp"
#include <vulkan/vulkan.hpp>
#include "vma.hpp"
namespace vkutil {
    Buffer createBuffer(VmaAllocator allocator,
                        BufferInput input);
    uint32_t findMemoryTypeIndex(vk::PhysicalDevice physicalDevice,
                                 uint32_t supportedMemoryIndices, 
                                 vk::MemoryPropertyFlags requestedProperties);
    void allocateBufferMemory(Buffer &buffer, 
                              const BufferInput& input,
                              VmaAllocator allocator);
}
#endif //INC_3DLOADERVK_MEMORY_HPP
