project(Renderer)

cocktail_source_directory(RENDERER_FILES    
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/ColorSpace.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Export.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Format.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/GraphicApi.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/RasterizationSamples.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/RenderDevice.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/RenderDeviceExtension.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/RenderDeviceObject.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Renderer.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Renderer/Format.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Renderer/New.cpp
)

cocktail_source_directory(RENDERER_FILES    
    FOLDER "Buffer"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Buffer/Buffer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Buffer/BufferAllocator.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Buffer/BufferArea.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Buffer/BufferCreateInfo.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Buffer/BufferUsage.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Buffer/BufferView.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Buffer/BufferViewCreateInfo.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Buffer/IndexType.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Renderer/Buffer/IndexType.cpp
)

cocktail_source_directory(RENDERER_FILES    
    FOLDER "Command"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Command/BufferUploadInfo.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Command/CommandList.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Command/CommandListCreateInfo.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Command/CommandListDynamicState.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Command/CommandListUsage.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Command/CommandListState.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Command/CommandQueueType.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Command/GpuBarrier.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Command/RenderPassBeginInfo.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Command/TextureUploadInfo.hpp
	SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Renderer/Command/CommandQueueType.cpp
)

cocktail_source_directory(RENDERER_FILES    
    FOLDER "Command/Synchronization"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Command/Synchronization/Fence.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Command/Synchronization/FenceCreateInfo.hpp
)

cocktail_source_directory(RENDERER_FILES    
    FOLDER "Context"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Context/RenderContext.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Context/RenderContextCreateInfo.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Context/RenderSurface.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Context/RenderSurfaceCreateInfo.hpp
)

cocktail_source_directory(RENDERER_FILES    
    FOLDER "Framebuffer"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Framebuffer/Framebuffer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Framebuffer/FramebufferCreateInfo.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Framebuffer/FramebufferLayout.hpp
	SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Renderer/Framebuffer/FramebufferLayout.cpp
)

cocktail_source_directory(RENDERER_FILES    
    FOLDER "Memory"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Memory/MemoryResource.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Memory/MemoryType.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Memory/ResourceState.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Memory/ResourceType.hpp
)

cocktail_source_directory(RENDERER_FILES    
    FOLDER "Shader"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Shader/DescriptorType.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Shader/Shader.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Shader/ShaderCreateInfo.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Shader/ShaderProgram.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Shader/ShaderProgramCreateInfo.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Shader/ShaderProgramType.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Shader/ShaderType.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Shader/UniformSlot.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Shader/VertexAttributeLocation.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Renderer/Shader/ShaderProgramType.cpp
)

cocktail_source_directory(RENDERER_FILES    
    FOLDER "Texture"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Texture/Sampler.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Texture/SamplerCreateInfo.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Texture/StaticSampler.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Texture/Texture.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Texture/TextureCreateInfo.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Texture/TextureSubResource.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Texture/TextureView.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Renderer/Texture/TextureViewCreateInfo.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Renderer/Texture/Texture.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Renderer/Texture/TextureSubResource.cpp
)

cocktail_add_runtime_library(
    NAME Renderer
    DEPENDS Core
    SOURCES ${RENDERER_FILES}
)
