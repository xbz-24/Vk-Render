#ifndef INC_3DLOADERVK_FRAMEBUFFER_HPP
#define INC_3DLOADERVK_FRAMEBUFFER_HPP

#include <vulkan/vulkan.hpp>
#include <vector>
#include <iostream>
#include "frame.hpp"

namespace vkInit
{
    struct framebufferInput
    {
        vk::Device device;
        vk::RenderPass renderpass;
        vk::Extent2D swapchainExtent;
    };

    void make_framebuffers(framebufferInput inputChunk, std::vector<vkUtil::SwapChainFrame>& frames, bool debug);
}

#endif //INC_3DLOADERVK_FRAMEBUFFER_HPP
