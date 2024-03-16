//
//  vulkan_result.hpp
//  3DLoaderVk
//
//  Created by xbz on 15/03/2024.
//
#ifndef vulkan_result_hpp
#define vulkan_result_hpp

#include <string>
#include <optional>
template<typename T>
struct VulkanResult {
    std::optional<T> value;
    std::string error;
    VulkanResult() = default;
    VulkanResult(const T &val) : value(val) { }
    VulkanResult(const std::string& err) : error(err) { }
    bool isSuccess() const {
        return value.has_value();
    }
};

#endif /* vulkan_result_hpp */
