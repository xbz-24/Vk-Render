//
// Created by daily on 27-12-23.
//

#ifndef INC_3DLOADERVK_SYNC_H
#define INC_3DLOADERVK_SYNC_H
#include "config.hpp"

namespace vkInit {
    vk::Semaphore make_semaphore(vk::Device device, bool debug){
        vk::SemaphoreCreateInfo semaphoreInfo = { };
        semaphoreInfo.flags = vk::SemaphoreCreateFlags();
        try{
            return device.createSemaphore(semaphoreInfo);
        }
        catch(vk::SystemError err){
            if(debug){
                std::cout << "Failed to create semaphore" << std::endl;
            }
            return nullptr;
        }
    }
    vk::Fence make_fence(vk::Device device, bool debug){
        vk::FenceCreateInfo fenceInfo = { };
        fenceInfo.flags = vk::FenceCreateFlags() | vk::FenceCreateFlagBits::eSignaled;

        try{
            return device.createFence(fenceInfo);
        }
        catch(vk::SystemError err){
            if(debug){
                std::cout << "Failed to create fence" << std::endl;
            }
            return nullptr;
        }
    }
}


#endif //INC_3DLOADERVK_SYNC_H
