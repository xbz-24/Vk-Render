//
// Created by daily on 02-01-24.
//

#ifndef INC_3DLOADERVK_MEMORY_H
#define INC_3DLOADERVK_MEMORY_H
#include "config.hpp"

namespace vkUtil{

    Buffer createBuffer(BufferInput input);
    uint32_t findMemoryTypeIndex(vk::PhysicalDevice physicalDevice, uint32_t supportedMemoryIndices, vk::MemoryPropertyFlags requestedProperties);
    void allocateBufferMemory(Buffer &buffer, const BufferInput& input);
}


#endif //INC_3DLOADERVK_MEMORY_H
