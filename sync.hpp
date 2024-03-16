//
// Created by daily on 27-12-23.
//
/**
 * @file sync.hpp
 * @brief Defines functions for creating Vulkan synchronization primitives.
 * @date Created by Renato on 27-12-23.
 */
#ifndef INC_3DLOADERVK_SYNC_HPP
#define INC_3DLOADERVK_SYNC_HPP
#include <vulkan/vulkan.hpp>
#include <iostream>
#include <format>

namespace vkinit
{
    vk::Semaphore make_semaphore(vk::Device device, 
                                 bool debug);
    vk::Fence make_fence(vk::Device device, 
                         bool debug);
}
#endif //INC_3DLOADERVK_SYNC_HPP
