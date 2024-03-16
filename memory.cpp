/**
 * @file memory.cpp
 * @brief
 * @date Created on 02-01-24.
 * @author Renato German Chavez Chicoma
 */
#include "memory.hpp"
Buffer vkutil::createBuffer(VmaAllocator allocator,
                            BufferInput input){
//    vk::BufferCreateInfo bufferInfo;
//    bufferInfo.flags = vk::BufferCreateFlags();
//    bufferInfo.size = input.size;
//    bufferInfo.usage = static_cast<VkBufferUsageFlags>(input.usage);
//    bufferInfo.sharingMode = vk::SharingMode::eExclusive;
//    Buffer buffer;
//    buffer.buffer = input.logical_device.createBuffer(bufferInfo);
//    allocateBufferMemory(buffer, input);
//    return buffer;
    VkBufferCreateInfo bufferCreateInfo = {};
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferCreateInfo.size = input.size;
    bufferCreateInfo.usage = static_cast<VkBufferUsageFlags>(input.usage);
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo allocCreateInfo = {};
    allocCreateInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU;

    VkBuffer buffer;
    VmaAllocation allocation;
    VmaAllocationInfo allocationInfo;

    if (vmaCreateBuffer(allocator, &bufferCreateInfo, &allocCreateInfo, &buffer, &allocation, &allocationInfo) != VK_SUCCESS) {
        throw std::runtime_error("Failed to create buffer and allocate memory.");
    }

    return Buffer{buffer, allocation};

}
uint32_t vkutil::findMemoryTypeIndex(vk::PhysicalDevice physicalDevice, 
                                     uint32_t supportedMemoryIndices,
                                     vk::MemoryPropertyFlags requestedProperties){
    vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();
    for(uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++){
        bool supported { static_cast<bool>(supportedMemoryIndices & (1 << i)) };
        bool sufficient { (memoryProperties.memoryTypes[i].propertyFlags & requestedProperties) == requestedProperties };
        if(supported && sufficient){
            return i;
        }
    }
    throw std::runtime_error("Failed to find suitable memory type.");
}
void vkutil::allocateBufferMemory(Buffer &buffer, 
                                  const BufferInput& input,
                                  VmaAllocator allocator){
    
    //    vk::MemoryRequirements memoryRequirements = input.logical_device.getBufferMemoryRequirements(buffer.buffer);
    //    vk::MemoryAllocateInfo allocInfo;
    //    allocInfo.allocationSize = memoryRequirements.size;
    //    allocInfo.memoryTypeIndex = findMemoryTypeIndex(
    //            input.physical_device, memoryRequirements.memoryTypeBits,
    //            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
    //    );
    //    buffer.buffer_memory = input.logical_device.allocateMemory(allocInfo);
    //
    //    input.logical_device.bindBufferMemory(buffer.buffer, buffer.buffer_memory, 0);
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = input.size;
    bufferInfo.usage = static_cast<VkBufferUsageFlags>(input.usage); // Cast to VkBufferUsageFlags
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VmaAllocationCreateInfo allocInfo = {};
    allocInfo.usage = VMA_MEMORY_USAGE_CPU_TO_GPU; // Example usage, adjust based on your needs

    // Create buffer and allocate memory
    VkResult result = vmaCreateBuffer(allocator, &bufferInfo, &allocInfo, &buffer.buffer, &buffer.allocation, nullptr);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate buffer memory with VMA.");
    }
}
