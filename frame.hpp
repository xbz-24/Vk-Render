#ifndef INC_3DLOADERVK_FRAME_HPP
#define INC_3DLOADERVK_FRAME_HPP
#include <vulkan/vulkan.hpp>
namespace vkutil
{
    struct SwapChainFrame
    {
        vk::Image image;
        vk::ImageView imageView;
        vk::Framebuffer framebuffer;
        vk::CommandBuffer commandbuffer;
        vk::Semaphore imageAvailable;
        vk::Semaphore renderFinished;
        vk::Fence inFlight;
    };
}
#endif //INC_3DLOADERVK_FRAME_HPP