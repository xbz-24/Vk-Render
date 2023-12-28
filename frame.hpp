//
// Created by daily on 27-12-23.
//

#ifndef INC_3DLOADERVK_FRAME_HPP
#define INC_3DLOADERVK_FRAME_HPP
#include "config.hpp"

namespace vkUtil{
    struct SwapChainFrame{
        vk::Image image;
        vk::ImageView imageView;
        vk::Framebuffer framebuffer;
        vk::CommandBuffer commandbuffer;
    };
}


#endif //INC_3DLOADERVK_FRAME_HPP
