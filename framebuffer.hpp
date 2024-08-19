#ifndef INC_3DLOADERVK_FRAMEBUFFER_HPP
#define INC_3DLOADERVK_FRAMEBUFFER_HPP

#include <vulkan/vulkan.hpp>
#include <vector>
#include <iostream>
#include "frame.hpp"

namespace vkinit
{
    struct FramebufferInput
    {
        vk::Device device;
        vk::RenderPass renderpass;
        vk::Extent2D swapchainExtent;
    };

    void make_frame_buffers(FramebufferInput inputChunk, std::vector<vkutil::SwapChainFrame>& frames, bool debug);
}

#endif //INC_3DLOADERVK_FRAMEBUFFER_HPP
