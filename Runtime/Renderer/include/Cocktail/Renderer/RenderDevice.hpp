#ifndef COCKTAIL_RENDERER_RENDERDEVICE_HPP
#define COCKTAIL_RENDERER_RENDERDEVICE_HPP

#include <Cocktail/Core/String.hpp>
#include <Cocktail/Core/Log/LogLevel.hpp>
#include <Cocktail/Core/Signal/Signal.hpp>

#include <Cocktail/Renderer/RenderDeviceExtension.hpp>
#include <Cocktail/Renderer/Renderer.hpp>
#include <Cocktail/Renderer/Texture/TextureCreateInfo.hpp>

namespace Ck::Renderer
{
    class Buffer;
    struct BufferCreateInfo;
    class BufferView;
    struct BufferViewCreateInfo;
    class Fence;
    struct FenceCreateInfo;
    class Framebuffer;
    struct FramebufferCreateInfo;
    class RenderContext;
    struct RenderContextCreateInfo;
    class RenderSurface;
    struct RenderSurfaceCreateInfo;
    class Sampler;
    struct SamplerCreateInfo;
    class Shader;
    struct ShaderCreateInfo;
    class ShaderProgram;
    struct ShaderProgramCreateInfo;
    class Texture;
    struct TextureCreateInfo;
    class TextureView;
    struct TextureViewCreateInfo;

    /**
     * \brief Type of debug message reported by the graphics driver
     */
    enum class MessageType
    {
        /**
         * \brief General informational or diagnostic message
         *
         * This category includes messages that do not fall into validation
         * or performance-specific categories
         */
        General,

        /**
         * \brief Validation-related message
         *
         * Indicates incorrect API usage, invalid parameters, or violations
         * of the rendering API specification
         */
        Validation,

        /**
         * \brief Performance-related message
         *
         * Provides hints or warnings about potential performance issues
         * or inefficient API usage
         */
        Performance,
    };

    /**
     * \brief Interface representing a GPU device
     *
     * The RenderDevice is responsible for creating GPU resources and objects
     * that can be consumed by CommandList instances to perform rendering
     * operations.
     *
     * This class is thread-safe and may be called from any thread
     */
    class RenderDevice
    {
    public:

        /**
         * \brief Destructor
         */
        virtual ~RenderDevice() = default;

        /**
         * \brief Creates a new Buffer resource
         *
         * \param createInfo Parameters used to create the Buffer
         *
         * \return The newly created Buffer
         */
        virtual std::shared_ptr<Buffer> CreateBuffer(const BufferCreateInfo& createInfo) = 0;

        /**
         * \brief Creates a new BufferView
         *
         * \param createInfo Parameters used to create the BufferView
         *
         * \return The newly created BufferView
         */
        virtual std::shared_ptr<BufferView> CreateBufferView(const BufferViewCreateInfo& createInfo) = 0;

        /**
         * \brief Creates a new Fence synchronization object
         *
         * \param createInfo Parameters used to create the Fence
         *
         * \return The newly created Fence
         */
        virtual std::shared_ptr<Fence> CreateFence(const FenceCreateInfo& createInfo) = 0;

        /**
         * \brief Creates a new Framebuffer
         *
         * \param createInfo Parameters used to create the Framebuffer
         *
         * \return The newly created Framebuffer
         */
        virtual std::shared_ptr<Framebuffer> CreateFramebuffer(const FramebufferCreateInfo& createInfo) = 0;

        /**
         * \brief Creates a new RenderContext
         *
         * A RenderContext typically represents a queue or execution context
         * used to submit command lists to the GPU.
         *
         * \param createInfo Parameters used to create the RenderContext
         *
         * \return The newly created RenderContext
         */
        virtual std::shared_ptr<RenderContext> CreateRenderContext(const RenderContextCreateInfo& createInfo) = 0;

        /**
         * \brief Creates a new RenderSurface
         *
         * A RenderSurface represents a presentation target, such as a window
         * or swapchain surface.
         *
         * \param createInfo Parameters used to create the RenderSurface
         *
         * \return The newly created RenderSurface
         */
        virtual std::shared_ptr<RenderSurface> CreateRenderSurface(const RenderSurfaceCreateInfo& createInfo) = 0;

        /**
         * \brief Creates a new Sampler object
         *
         * \param createInfo Parameters used to create the Sampler
         *
         * \return The newly created Sampler
         */
        virtual std::shared_ptr<Sampler> CreateSampler(const SamplerCreateInfo& createInfo) = 0;

        /**
         * \brief Creates a new Shader
         *
         * \param createInfo Parameters used to create the Shader
         *
         * \return The newly created Shader
         */
        virtual std::shared_ptr<Shader> CreateShader(const ShaderCreateInfo& createInfo) = 0;

        /**
         * \brief Creates a new ShaderProgram
         *
         * A ShaderProgram links multiple shader stages into a single
         * executable program.
         *
         * \param createInfo Parameters used to create the ShaderProgram
         *
         * \return The newly created ShaderProgram
         */
        virtual std::shared_ptr<ShaderProgram> CreateShaderProgram(const ShaderProgramCreateInfo& createInfo) = 0;

        /**
         * \brief Creates a new Texture resource
         *
         * \param createInfo Parameters used to create the Texture
         *
         * \return The newly created Texture
         */
        virtual std::shared_ptr<Texture> CreateTexture(const TextureCreateInfo& createInfo) = 0;

        /**
         * \brief Creates a new TextureView
         *
         * \param createInfo Parameters used to create the TextureView
         *
         * \return The newly created TextureView
         */
        virtual std::shared_ptr<TextureView> CreateTextureView(const TextureViewCreateInfo& createInfo) = 0;

        /**
         * \brief Checks whether a specific RenderDevice extension is supported
         *
         * Extension support depends heavily on the underlying graphics API
         * and driver capabilities.
         *
         * \param extension The extension to query
         *
         * \return True if the extension is supported, false otherwise
         */
        virtual bool IsExtensionSupported(RenderDeviceExtension extension) const = 0;

        /**
         * \brief Retrieves supported texture usage flags for a given format and memory type
         *
         * \param format The pixel format to query
         * \param memoryType The memory type to query
         *
         * \return A bitmask describing the supported texture usages
         */
        virtual TextureUsageFlags GetTextureFormatSupport(const PixelFormat& format, MemoryType memoryType) const = 0;

        /**
         * \brief Checks whether a specific variable shading rate configuration is supported
         *
         * \param fragmentSize The fragment size used for shading
         * \param samples The rasterization sample count
         *
         * \return True if the shading rate configuration is supported, false otherwise
         */
        virtual bool IsShadingRateSupported(Extent2D<unsigned int> fragmentSize, RasterizationSamples samples) = 0;

        /**
         * \brief Signal emitted when the underlying graphics API reports a debug message
         *
         * \return A signal carrying the log level, message type, and message text
         */
        virtual Signal<LogLevel, MessageType, AsciiStringView>& OnDebugMessage() = 0;
    };
}

#endif // COCKTAIL_RENDERER_RENDERDEVICE_HPP
