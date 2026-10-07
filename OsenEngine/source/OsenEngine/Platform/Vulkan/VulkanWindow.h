#pragma once

#include <vulkan/vulkan_raii.hpp>
#include <vector>

class VulkanWindow
{
public:
    virtual ~VulkanWindow() = default;

    virtual std::vector<const char*> requiredVulkanExtensions() const = 0;
    virtual vk::raii::SurfaceKHR createSurface(const vk::raii::Instance& instance) const = 0;
};
