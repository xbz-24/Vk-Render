#ifndef INC_3DLOADERVK_SYNC_HPP
#define INC_3DLOADERVK_SYNC_HPP

#include <vulkan/vulkan.hpp>
#include <iostream>

namespace vkinit
{
    vk::Semaphore make_semaphore(vk::Device device, bool debug);
    vk::Fence make_fence(vk::Device device, bool debug);
}
#endif //INC_3DLOADERVK_SYNC_HPP