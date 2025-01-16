project(Vulkan)

cocktail_source_directory(VULKAN_FILES    
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Vulkan/ApiVersion.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Vulkan/Export.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Vulkan/RenderDeviceCreateInfo.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Vulkan/VersionDescriptor.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Vulkan/Vulkan.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Vulkan/VulkanServiceProvider.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/ExtensionManager.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/ExtensionManager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/RenderDevice.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/RenderDevice.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Semaphore.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Semaphore.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/SemaphoreCreateInfo.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/VersionDescriptor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Volk.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Volk.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/VolkService.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/VolkService.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Vulkan.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/VulkanServiceProvider.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/VulkanUtils.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/VulkanUtils.cpp
)

cocktail_source_directory(VULKAN_FILES    
    FOLDER "Buffer"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Buffer/Buffer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Buffer/Buffer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Buffer/BufferAllocator.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Buffer/BufferAllocator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Buffer/BufferPool.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Buffer/BufferPool.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Buffer/BufferView.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Buffer/BufferView.cpp
)

cocktail_source_directory(VULKAN_FILES    
    FOLDER "Command"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Command/CommandList.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Command/CommandList.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Command/Fence.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Command/Fence.cpp
)

cocktail_source_directory(VULKAN_FILES    
    FOLDER "Command/Allocator"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Command/Allocator/CommandListPool.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Command/Allocator/CommandListPool.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Command/Allocator/CommandPool.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Command/Allocator/CommandPool.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Command/Allocator/CommandPoolCreateInfo.hpp
)

cocktail_source_directory(VULKAN_FILES    
    FOLDER "Command/Staging"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Command/Staging/StagingAllocator.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Command/Staging/StagingAllocator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Command/Staging/StagingBuffer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Command/Staging/StagingBuffer.cpp
)

cocktail_source_directory(VULKAN_FILES    
    FOLDER "Context"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Context/FrameContext.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Context/FrameContext.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Context/PresentationContext.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Context/PresentationContext.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Context/RenderContext.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Context/RenderContext.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Context/RenderSurface.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Context/RenderSurface.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Context/Swapchain.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Context/Swapchain.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Context/SwapchainCreateInfo.hpp
)

cocktail_source_directory(VULKAN_FILES
    FOLDER "Debug"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Debug/DebugMessenger.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Debug/DebugMessenger.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Debug/DebugMessengerCreateInfo.hpp
)

cocktail_source_directory(VULKAN_FILES
    FOLDER "DescriptorSet"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/DescriptorSet/DescriptorSet.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/DescriptorSet/DescriptorSet.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/DescriptorSet/DescriptorSetCreateInfo.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/DescriptorSet/DescriptorUpdateTemplate.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/DescriptorSet/DescriptorUpdateTemplate.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/DescriptorSet/DescriptorUpdateTemplateCreateInfo.hpp
)

cocktail_source_directory(VULKAN_FILES
    FOLDER "DescriptorSet/Allocator"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/DescriptorSet/Allocator/DescriptorPool.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/DescriptorSet/Allocator/DescriptorPool.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/DescriptorSet/Allocator/DescriptorPoolCreateInfo.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/DescriptorSet/Allocator/DescriptorSetAllocator.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/DescriptorSet/Allocator/DescriptorSetAllocator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/DescriptorSet/Allocator/DescriptorSetAllocatorCreateInfo.hpp
)

cocktail_source_directory(VULKAN_FILES
    FOLDER "DescriptorSet/Layout"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/DescriptorSet/Layout/DescriptorSetLayout.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/DescriptorSet/Layout/DescriptorSetLayout.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/DescriptorSet/Layout/DescriptorSetLayoutBinding.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/DescriptorSet/Layout/DescriptorSetLayoutCreateInfo.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/DescriptorSet/Layout/DescriptorSetLayoutSignature.hpp
)

cocktail_source_directory(VULKAN_FILES
    FOLDER "DescriptorSet/State"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/DescriptorSet/State/DescriptorSetStateManager.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/DescriptorSet/State/DescriptorSetStateManager.cpp
)

cocktail_source_directory(VULKAN_FILES    
    FOLDER "Framebuffer"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Framebuffer/AttachmentBuffer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Framebuffer/AttachmentBuffer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Framebuffer/DepthResolver.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Framebuffer/DepthResolver.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Framebuffer/Framebuffer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Framebuffer/Framebuffer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Framebuffer/RenderBuffer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Framebuffer/RenderBuffer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Framebuffer/RenderBufferCreateInfo.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Framebuffer/RenderPass.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Framebuffer/RenderPass.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Framebuffer/RenderPassBuilder.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Framebuffer/RenderPassBuilder.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Framebuffer/RenderPassCreateInfo.hpp
)

cocktail_source_directory(VULKAN_FILES    
    FOLDER "Memory"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Memory/DeviceMemory.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Memory/DeviceMemory.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Memory/DeviceMemoryCreateInfo.hpp
)

cocktail_source_directory(VULKAN_FILES    
    FOLDER "Memory/Allocator"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Memory/Allocator/DeviceMemoryAllocator.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Memory/Allocator/DeviceMemoryAllocator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Memory/Allocator/DeviceMemoryBlock.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Memory/Allocator/DeviceMemoryBlock.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Memory/Allocator/DeviceMemoryChunk.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Memory/Allocator/DeviceMemoryChunk.cpp
)

cocktail_source_directory(VULKAN_FILES    
    FOLDER "Pipeline"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Pipeline/ComputePipeline.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Pipeline/ComputePipeline.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Pipeline/ComputePipelineCreateInfo.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Pipeline/GraphicPipeline.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Pipeline/GraphicPipeline.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Pipeline/GraphicPipelineCreateInfo.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Pipeline/Pipeline.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Pipeline/Pipeline.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Pipeline/PipelineCache.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Pipeline/PipelineCache.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Pipeline/PipelineCacheCreateInfo.hpp
)

cocktail_source_directory(VULKAN_FILES    
    FOLDER "Pipeline/Layout"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Pipeline/Layout/PipelineLayout.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Pipeline/Layout/PipelineLayout.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Pipeline/Layout/PipelineLayoutCreateInfo.hpp
)

cocktail_source_directory(VULKAN_FILES    
    FOLDER "Pipeline/State"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Pipeline/State/ComputeState.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Pipeline/State/ComputeStateManager.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Pipeline/State/ComputeStateManager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Pipeline/State/GraphicState.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Pipeline/State/GraphicStateManager.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Pipeline/State/GraphicStateManager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Pipeline/State/ShaderStageState.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Pipeline/State/StateManager.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Pipeline/State/StateManager.cpp
)

cocktail_source_directory(VULKAN_FILES    
    FOLDER "Queue"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Queue/QueueFamily.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Queue/QueueFamily.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Queue/QueueFamilyContext.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Queue/QueueFamilyContext.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Queue/QueueSubmit.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Queue/QueueSubmit.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Queue/QueueSubmitBatch.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Queue/QueueSubmitBatch.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Queue/QueueSubmitDependency.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Queue/QueueSubmitter.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Queue/QueueSubmitter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Queue/SubmitScheduler.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Queue/SubmitScheduler.cpp
)

cocktail_source_directory(VULKAN_FILES    
    FOLDER "Shader"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Shader/Shader.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Shader/Shader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Shader/ShaderProgram.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Shader/ShaderProgram.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Shader/ValidationCache.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Shader/ValidationCache.cpp
)

cocktail_source_directory(VULKAN_FILES    
    FOLDER "Shader/Location"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Shader/Location/VertexAttributeLocation.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Shader/Location/VertexAttributeLocation.cpp
)

cocktail_source_directory(VULKAN_FILES    
    FOLDER "Shader/Reflection"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Shader/Reflection/BlockMember.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Shader/Reflection/DescriptorSetInfo.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Shader/Reflection/DescriptorSetBindingInfo.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Shader/Reflection/PushConstantBlock.hpp
)

cocktail_source_directory(VULKAN_FILES    
    FOLDER "Shader/Slot"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Shader/Slot/DescriptorSetSlot.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Shader/Slot/DescriptorSetSlot.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Shader/Slot/PipelineConstantSlot.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Shader/Slot/PipelineConstantSlot.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Shader/Slot/UniformSlot.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Shader/Slot/UniformSlot.cpp
)

cocktail_source_directory(VULKAN_FILES    
    FOLDER "Texture"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Texture/AbstractTexture.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Texture/AbstractTexture.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Texture/Sampler.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Texture/Sampler.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Texture/SwapchainTexture.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Texture/SwapchainTexture.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Texture/Texture.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Texture/Texture.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Texture/TextureView.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/Texture/TextureView.cpp
)

cocktail_source_directory(VULKAN_FILES    
    FOLDER "WSI"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/WSI/WSI.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/WSI/WSI.cpp
)

if (WIN32)
    cocktail_source_directory(VULKAN_FILES    
        FOLDER "WSI/Win32"
        SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/WSI/Win32/WSI.cpp
    )
elseif (LINUX)
    cocktail_source_directory(VULKAN_FILES    
        FOLDER "WSI/Xlib"
        SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Vulkan/WSI/Xlib/WSI.cpp
    )
endif ()

cocktail_add_shader(VULKAN_FILES
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/resources/depth_resolve.vert.spv
    TYPE vert
    FILE ${CMAKE_CURRENT_SOURCE_DIR}/resources/depth_resolve.vert
)

cocktail_add_shader(VULKAN_FILES
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/resources/depth_resolve.frag.spv
    TYPE frag
    FILE ${CMAKE_CURRENT_SOURCE_DIR}/resources/depth_resolve.frag
)

cocktail_resource_directory(VULKAN_FILES
    RESOURCES 
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/depth_resolve.vert
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/depth_resolve.frag
)

set (VULKAN_LINK_LIBRARIES Vulkan::Headers volk::volk_headers spirv-reflect-static)
if (LINUX)        
    find_package(X11 REQUIRED COMPONENTS Xrandr Xcursor)
    list(APPEND VULKAN_LINK_LIBRARIES X11::X11 X11::Xrandr X11::Xcursor)
endif ()

cocktail_add_runtime_library(
    NAME 
        Vulkan
    DEPENDS 
        Core 
        Renderer
    LINK
        ${VULKAN_LINK_LIBRARIES}        
    DEFINES 
        VK_NO_PROTOTYPES
        $<$<BOOL:${WIN32}>:VK_USE_PLATFORM_WIN32_KHR>
        $<$<BOOL:${LINUX}>:VK_USE_PLATFORM_XLIB_KHR>
    SOURCES 
        ${VULKAN_FILES}
    RESOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/depth_resolve.vert.spv
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/depth_resolve.frag.spv
)
