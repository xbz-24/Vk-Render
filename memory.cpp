#include "memory.hpp"

Buffer vkutil::create_buffer(BufferInput input)
{
    vk::BufferCreateInfo bufferInfo;

    bufferInfo.flags = vk::BufferCreateFlags();
    bufferInfo.size = input.size;
    bufferInfo.usage = input.usage;
    bufferInfo.sharingMode = vk::SharingMode::eExclusive;

    Buffer buffer;
    buffer.buffer = input.logical_device.createBuffer(bufferInfo);

    allocate_buffer_memory(buffer, input);

    return buffer;
}
uint32_t vkutil::find_memory_type_index(vk::PhysicalDevice physicalDevice, uint32_t supportedMemoryIndices, vk::MemoryPropertyFlags requestedProperties)
{
    vk::PhysicalDeviceMemoryProperties memoryProperties = physicalDevice.getMemoryProperties();

    for(uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
    {
        bool supported {
            static_cast<bool>(supportedMemoryIndices & (1 << i))
        };

        bool sufficient {
            (memoryProperties.memoryTypes[i].propertyFlags & requestedProperties) == requestedProperties
        };

        if(supported && sufficient)
        {
            return i;
        }
    }
    throw std::runtime_error("Failed to find suitable memory type.");
}
void vkutil::allocate_buffer_memory(Buffer &buffer, const BufferInput& input)
{
    vk::MemoryRequirements memoryRequirements = input.logical_device.getBufferMemoryRequirements(buffer.buffer);
    vk::MemoryAllocateInfo allocInfo;

    allocInfo.allocationSize = memoryRequirements.size;
    allocInfo.memoryTypeIndex = find_memory_type_index
    (
       input.physical_device, memoryRequirements.memoryTypeBits,
       vk::MemoryPropertyFlagBits::eHostVisible
       | vk::MemoryPropertyFlagBits::eHostCoherent
    );
    buffer.buffer_memory = input.logical_device.allocateMemory(allocInfo);

    input.logical_device.bindBufferMemory(buffer.buffer, buffer.buffer_memory, 0);
}