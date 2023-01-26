#pragma once

#include "zz/zz.h"

#if defined(ZZ_BACKEND_VULKAN)

#include <vulkan/vulkan.h>
#if defined(ZZ_PLATFORM_WINDOWS)
#include <windows.h>
#include <windowsx.h>
#include <vulkan/vulkan_win32.h>
#elif defined(ZZ_PLATFORM_LINUX)
#endif

#include "zz/assert.h"
#include "platform/windows/core/application/application.h"

#define ZZ_BACKEND_VULKAN_ASSERT(expression) ZZ_ASSERT(expression == VK_SUCCESS);

struct backend_vulkan_physical_device_requirements
{
    b8 graphics;
    b8 present;
    b8 compute;
    b8 transfer;

    b8 sampler_anisotropy;
    b8 discrete_gpu;

    const char** extension_names;
};

struct backend_vulkan_physical_device_swapchain_support_info
{
    VkSurfaceCapabilitiesKHR capabilities;
    VkSurfaceFormatKHR* surfaceFormats;
    u32 surface_format_count;
    VkPresentModeKHR* presentModes;
    u32 present_mode_count;
};

struct backend_vulkan_physical_device_queue_family_info
{
    u32 graphics_index;
    u32 present_index;
    u32 compute_index;
    u32 transfer_index;
};

struct backend_vulkan_device
{
    VkPhysicalDevice physicalDevice;
    VkPhysicalDeviceProperties physicalDeviceProperties;
    VkPhysicalDeviceFeatures physicalDeviceFeatures;
    VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties;

    VkDevice logicalDevice;

    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkQueue computeQueue;
    VkQueue transferQueue;

    VkCommandPool graphicsCommandPool;

    struct backend_vulkan_physical_device_queue_family_info queue_family_info;
    struct backend_vulkan_physical_device_swapchain_support_info swapchain_support_info;

    VkFormat depthFormat;
};

struct backend_vulkan_image
{
    VkImage handle;
    VkDeviceMemory memory;
    VkImageView view;
    u32 width, height;
};

enum backend_vulkan_render_pass_state
{
    ZZ_BACKEND_VULKAN_RENDER_PASS_STATE_READY,
    ZZ_BACKEND_VULKAN_RENDER_PASS_STATE_RECORDING,
    ZZ_BACKEND_VULKAN_RENDER_PASS_STATE_IN_RENDER_PASS,
    ZZ_BACKEND_VULKAN_RENDER_PASS_STATE_FINISHED,
    ZZ_BACKEND_VULKAN_RENDER_PASS_STATE_SUMBITTED,
    ZZ_BACKEND_VULKAN_RENDER_PASS_STATE_UNALLOCATED
};

struct backend_vulkan_render_pass
{
    VkRenderPass handle;

    f32 x, y, w, h;
    f32 r, g, b, a;

    f32 depth;
    u32 stencil;

    enum backend_vulkan_render_pass_state state;
};

struct backend_vulkan_framebuffer
{
    VkFramebuffer handle;
    u32 attachment_image_view_count;
    VkImageView* attachmentImageViews;
    struct backend_vulkan_render_pass* render_pass;
};

struct backend_vulkan_swapchain
{
    VkSwapchainKHR handle;
    VkSurfaceFormatKHR imageSurfaceFormat;
    u8 max_frames_in_flight;
    u32 image_count;
    VkImage* images;
    VkImageView* imageViews;

    struct backend_vulkan_image depth_image;

    struct backend_vulkan_framebuffer* framebuffers;
};

enum backend_vulkan_command_buffer_state
{
    ZZ_BACKEND_VULKAN_COMMAND_BUFFER_STATE_READY,
    ZZ_BACKEND_VULKAN_COMMAND_BUFFER_STATE_RECORDING,
    ZZ_BACKEND_VULKAN_COMMAND_BUFFER_STATE_IN_RENDER_PASS,
    ZZ_BACKEND_VULKAN_COMMAND_BUFFER_STATE_FINISHED,
    ZZ_BACKEND_VULKAN_COMMAND_BUFFER_STATE_SUBMITTED,
    ZZ_BACKEND_VULKAN_COMMAND_BUFFER_STATE_UNALLOCATED
};

struct backend_vulkan_command_buffer
{
    VkCommandBuffer handle;
    enum backend_vulkan_command_buffer_state state;
};

struct backend_vulkan_fence
{
    VkFence handle;
    b8 signaled;
};

struct backend_vulkan_context
{
    VkInstance instance;
    VkAllocationCallbacks* allocator;

    VkSurfaceKHR surface;

    struct backend_vulkan_device device;

    struct backend_vulkan_swapchain swapchain;

    struct backend_vulkan_render_pass main_render_pass;

    struct backend_vulkan_command_buffer* graphics_command_buffers;

    VkSemaphore* imageAvailableSemaphores;
    VkSemaphore* queueCompleteSemaphores;

    u32 in_flight_fence_count;
    struct backend_vulkan_fence* in_flight_fences;

    struct backend_vulkan_fence** in_flight_image_fences;

    u32 framebuffer_width, framebuffer_height;
    u64 framebuffer_size_generation;
    u64 framebuffer_size_last_generation;
    u32 image_index;
    u32 current_frame;
    b8 recreating_swapchain;

#if defined(ZZ_DEBUG)
    VkDebugUtilsMessengerEXT debugUtilsMessenger;
#elif defined(ZZ_RELEASE)
#endif
};

b8 backend_vulkan_context_create_surface(struct platform_application* platform_application, struct backend_vulkan_context* context);

void backend_vulkan_physical_device_query_swapchain_support(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, struct backend_vulkan_physical_device_swapchain_support_info* swapchain_support_info);
b8 backend_vulkan_physical_device_meets_requirements(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const VkPhysicalDeviceProperties* properties, const VkPhysicalDeviceFeatures* features, const struct backend_vulkan_physical_device_requirements* requirements, struct backend_vulkan_physical_device_queue_family_info* queue_family_info, struct backend_vulkan_physical_device_swapchain_support_info* swapchain_support_info);
b8 backend_vulkan_context_select_physical_device(struct backend_vulkan_context* context);

b8 backend_vulkan_context_create_device(struct backend_vulkan_context* context);
void backend_vulkan_context_destroy_device(struct backend_vulkan_context* context);

b8 backend_vulkan_device_detect_depth_format(struct backend_vulkan_device* device);

void backend_vulkan_image_create(struct backend_vulkan_context* context, VkImageType imageType, u32 width, u32 height, VkFormat format, VkImageTiling imageTiling, VkImageUsageFlags imageUsageFlags, VkMemoryPropertyFlags memoryPorpertyFlags, b32 create_view, VkImageAspectFlags imageAspectFlags, struct backend_vulkan_image* image);
void backend_vulkan_image_view_create(struct backend_vulkan_context* context, VkFormat format, struct backend_vulkan_image* image, VkImageAspectFlags imageAspectFlags);
void backend_vulkan_image_destroy(struct backend_vulkan_context* context, struct backend_vulkan_image* image);

b8 backend_vulkan_render_pass_create(struct backend_vulkan_context* context, struct backend_vulkan_render_pass* render_pass, f32 x, f32 y, f32 w, f32 h, f32 r, f32 g, f32 b, f32 a, f32 depth, u32 stencil);
void backend_vulkan_render_pass_destroy(struct backend_vulkan_context* context, struct backend_vulkan_render_pass* render_pass);
void backend_vulkan_render_pass_begin(struct backend_vulkan_render_pass* render_pass, struct backend_vulkan_command_buffer* command_buffer, VkFramebuffer framebuffer);
void backend_vulkan_render_pass_end(struct backend_vulkan_render_pass* render_pass, struct backend_vulkan_command_buffer* command_buffer);

b8 backend_vulkan_framebuffer_create(struct backend_vulkan_context* context, struct backend_vulkan_framebuffer* framebuffer, struct backend_vulkan_render_pass* render_pass, u32 width, u32 height, u32 attachment_image_view_count, VkImageView* attachmentImageViews);
void backend_vulkan_framebuffer_destroy(struct backend_vulkan_context* context, struct backend_vulkan_framebuffer* framebuffer);

b8 backend_vulkan_swapchain_create(struct backend_vulkan_context* context, u32 width, u32 height, struct backend_vulkan_swapchain* swapchain);
b8 backend_vulkan_swapchain_recreate(struct backend_vulkan_context* context, u32 width, u32 height, struct backend_vulkan_swapchain* swapchain);
void backend_vulkan_swapchain_destroy(struct backend_vulkan_context* context, struct backend_vulkan_swapchain* swapchain);
b8 backend_vulkan_swapchain_acquire_next_image_index(struct backend_vulkan_context* context, struct backend_vulkan_swapchain* swapchain, u64 timeout_nanoseconds, VkSemaphore imageAvailableSemaphore, VkFence fence, u32* image_index);
void backend_vulkan_swapchain_present(struct backend_vulkan_context* context, struct backend_vulkan_swapchain* swapchain, VkQueue graphicsQueue, VkQueue presentQueue, VkSemaphore renderCompleteSemaphore, u32 present_image_index);

b8 backend_vulkan_command_buffer_allocate(struct backend_vulkan_context* context, struct backend_vulkan_command_buffer* command_buffer, VkCommandPool commandPool, b8 primary);
void backend_vulkan_command_buffer_deallocate(struct backend_vulkan_context* context, struct backend_vulkan_command_buffer* command_buffer, VkCommandPool commandPool);
void backend_vulkan_command_buffer_begin(struct backend_vulkan_command_buffer* command_buffer, b8 single_use, b8 render_pass_continue, b8 simultaneous_use);
void backend_vulkan_command_buffer_end(struct backend_vulkan_command_buffer* command_buffer);
void backend_vulkan_command_buffer_reset(struct backend_vulkan_command_buffer* command_buffer);
void backend_vulkan_command_buffer_allocate_and_begin_for_single_use(struct backend_vulkan_context* context, struct backend_vulkan_command_buffer* command_buffer, VkCommandPool commandPool);
void backend_vulkan_command_buffer_end_and_submit_for_single_use(struct backend_vulkan_context* context, struct backend_vulkan_command_buffer* command_buffer, VkCommandPool commandPool, VkQueue queue);

b8 backend_vulkan_fence_create(struct backend_vulkan_context* context, struct backend_vulkan_fence* fence, b8 signaled);
void backend_vulkan_fence_destroy(struct backend_vulkan_context* context, struct backend_vulkan_fence* fence);
b8 backend_vulkan_fence_wait(struct backend_vulkan_context* context, struct backend_vulkan_fence* fence, u64 timeout_nanoseconds);
void backend_vulkan_fence_reset(struct backend_vulkan_context* context, struct backend_vulkan_fence* fence);

void backend_vulkan_context_generate_command_buffers(struct backend_vulkan_context* context);
void backend_vulkan_context_generate_framebuffers(struct backend_vulkan_context* context);

i32 backend_vulkan_context_get_memory_index(struct backend_vulkan_context* context, u32 type_filter, u32 property_flags);

b8 backend_vulkan_result_is_error(VkResult result);

#if defined(ZZ_DEBUG)
VKAPI_ATTR VkBool32 VKAPI_CALL backend_vulkan_debug_callback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* callbackData, void* userData);
#elif defined(ZZ_RELEASE)
#endif

#endif
