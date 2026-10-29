#ifndef COCKTAILENGINE_VULKAN_VULKANUTILS_HPP
#define COCKTAILENGINE_VULKAN_VULKANUTILS_HPP

#include <CocktailEngine/Renderer/Buffer/BufferUsage.hpp>
#include <CocktailEngine/Renderer/Buffer/IndexType.hpp>
#include <CocktailEngine/Renderer/ColorSpace.hpp>
#include <CocktailEngine/Renderer/Framebuffer/FramebufferCreateInfo.hpp>
#include <CocktailEngine/Renderer/RasterizationSamples.hpp>
#include <CocktailEngine/Renderer/Renderer.hpp>
#include <CocktailEngine/Renderer/Shader/DescriptorType.hpp>
#include <CocktailEngine/Renderer/Shader/ShaderType.hpp>
#include <CocktailEngine/Renderer/Texture/SamplerCreateInfo.hpp>
#include <CocktailEngine/Renderer/Texture/TextureCreateInfo.hpp>
#include <CocktailEngine/Renderer/Texture/TextureSwizzle.hpp>

#include <CocktailEngine/Vulkan/Volk.hpp>

#include "CocktailEngine/Core/GenericException.hpp"

/**
 * \brief Executes a Vulkan call and throws on a non-success result.
 *
 * \param call Vulkan expression returning a \c VkResult.
 *
 * \throws Ck::Vulkan::ApiException When \p call does not return \c VK_SUCCESS.
 */
#ifndef NDEBUG
#define COCKTAIL_VK_CHECK(call)                                                                                                                                                    \
    do                                                                                                                                                                   \
    {                                                                                                                                                                    \
        VkResult result_ = call;                                                                                                                                         \
        if (result_ != VK_SUCCESS)\
            throw Ck::Vulkan::ApiException(result_);\
    } while (0)
#else
    #define COCKTAIL_VK_CHECK(call) call
#endif

namespace Ck::Vulkan
{
    /**
     * \brief Translates a Vulkan result code into its diagnostic identifier.
     */
    class ApiErrorCodeTranslator : public ErrorCodeTranslator<VkResult>
    {
    public:

        /**
         * \brief Converts a Vulkan result into an engine-text diagnostic message.
         *
         * \param errorCode Vulkan result to convert.
         *
         * \return The Vulkan identifier associated with \p errorCode, or a
         *         fallback message for an unknown result.
         */
        String TranslateErrorCode(VkResult errorCode) const override
        {
            switch (errorCode)
            {
                case VK_SUCCESS: return CK_TEXT("VK_SUCCESS");
                case VK_NOT_READY: return CK_TEXT("VK_NOT_READY");
                case VK_TIMEOUT: return CK_TEXT("VK_TIMEOUT");
                case VK_EVENT_SET: return CK_TEXT("VK_EVENT_SET");
                case VK_EVENT_RESET: return CK_TEXT("VK_EVENT_RESET");
                case VK_INCOMPLETE: return CK_TEXT("VK_INCOMPLETE");
                case VK_ERROR_OUT_OF_HOST_MEMORY: return CK_TEXT("VK_ERROR_OUT_OF_HOST_MEMORY");
                case VK_ERROR_OUT_OF_DEVICE_MEMORY: return CK_TEXT("VK_ERROR_OUT_OF_DEVICE_MEMORY");
                case VK_ERROR_INITIALIZATION_FAILED: return CK_TEXT("VK_ERROR_INITIALIZATION_FAILED");
                case VK_ERROR_DEVICE_LOST: return CK_TEXT("VK_ERROR_DEVICE_LOST");
                case VK_ERROR_MEMORY_MAP_FAILED: return CK_TEXT("VK_ERROR_MEMORY_MAP_FAILED");
                case VK_ERROR_LAYER_NOT_PRESENT: return CK_TEXT("VK_ERROR_LAYER_NOT_PRESENT");
                case VK_ERROR_EXTENSION_NOT_PRESENT: return CK_TEXT("VK_ERROR_EXTENSION_NOT_PRESENT");
                case VK_ERROR_FEATURE_NOT_PRESENT: return CK_TEXT("VK_ERROR_FEATURE_NOT_PRESENT");
                case VK_ERROR_INCOMPATIBLE_DRIVER: return CK_TEXT("VK_ERROR_INCOMPATIBLE_DRIVER");
                case VK_ERROR_TOO_MANY_OBJECTS: return CK_TEXT("VK_ERROR_TOO_MANY_OBJECTS");
                case VK_ERROR_FORMAT_NOT_SUPPORTED: return CK_TEXT("VK_ERROR_FORMAT_NOT_SUPPORTED");
                case VK_ERROR_FRAGMENTED_POOL: return CK_TEXT("VK_ERROR_FRAGMENTED_POOL");
                case VK_ERROR_UNKNOWN: return CK_TEXT("VK_ERROR_UNKNOWN");
                case VK_ERROR_VALIDATION_FAILED: return CK_TEXT("VK_ERROR_VALIDATION_FAILED");
                case VK_ERROR_OUT_OF_POOL_MEMORY: return CK_TEXT("VK_ERROR_OUT_OF_POOL_MEMORY");
                case VK_ERROR_INVALID_EXTERNAL_HANDLE: return CK_TEXT("VK_ERROR_INVALID_EXTERNAL_HANDLE");
                case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS: return CK_TEXT("VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS");
                case VK_ERROR_FRAGMENTATION: return CK_TEXT("VK_ERROR_FRAGMENTATION");
                case VK_PIPELINE_COMPILE_REQUIRED: return CK_TEXT("VK_PIPELINE_COMPILE_REQUIRED");
                case VK_ERROR_NOT_PERMITTED: return CK_TEXT("VK_ERROR_NOT_PERMITTED");
                case VK_ERROR_SURFACE_LOST_KHR: return CK_TEXT("VK_ERROR_SURFACE_LOST_KHR");
                case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return CK_TEXT("VK_ERROR_NATIVE_WINDOW_IN_USE_KHR");
                case VK_SUBOPTIMAL_KHR: return CK_TEXT("VK_SUBOPTIMAL_KHR");
                case VK_ERROR_OUT_OF_DATE_KHR: return CK_TEXT("VK_ERROR_OUT_OF_DATE_KHR");
                case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return CK_TEXT("VK_ERROR_INCOMPATIBLE_DISPLAY_KHR");
                case VK_ERROR_INVALID_SHADER_NV: return CK_TEXT("VK_ERROR_INVALID_SHADER_NV");
                case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR: return CK_TEXT("VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR");
                case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR: return CK_TEXT("VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR");
                case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR: return CK_TEXT("VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR");
                case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR: return CK_TEXT("VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR");
                case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR: return CK_TEXT("VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR");
                case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR: return CK_TEXT("VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR");
                case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT: return CK_TEXT("VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT");
                case VK_ERROR_PRESENT_TIMING_QUEUE_FULL_EXT: return CK_TEXT("VK_ERROR_PRESENT_TIMING_QUEUE_FULL_EXT");
                case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT: return CK_TEXT("VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT");
                case VK_THREAD_IDLE_KHR: return CK_TEXT("VK_THREAD_IDLE_KHR");
                case VK_THREAD_DONE_KHR: return CK_TEXT("VK_THREAD_DONE_KHR");
                case VK_OPERATION_DEFERRED_KHR: return CK_TEXT("VK_OPERATION_DEFERRED_KHR");
                case VK_OPERATION_NOT_DEFERRED_KHR: return CK_TEXT("VK_OPERATION_NOT_DEFERRED_KHR");
                case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR: return CK_TEXT("VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR");
                case VK_ERROR_COMPRESSION_EXHAUSTED_EXT: return CK_TEXT("VK_ERROR_COMPRESSION_EXHAUSTED_EXT");
                case VK_INCOMPATIBLE_SHADER_BINARY_EXT: return CK_TEXT("VK_INCOMPATIBLE_SHADER_BINARY_EXT");
                case VK_PIPELINE_BINARY_MISSING_KHR: return CK_TEXT("VK_PIPELINE_BINARY_MISSING_KHR");
                case VK_ERROR_NOT_ENOUGH_SPACE_KHR: return CK_TEXT("VK_ERROR_NOT_ENOUGH_SPACE_KHR");
                case VK_RESULT_MAX_ENUM: return CK_TEXT("VK_RESULT_MAX_ENUM");
            }

            return String::Format(CK_TEXT("Unknown VkResult (%d)"), static_cast<int>(errorCode));
        }
    };

    /**
     * \brief Runtime exception that retains a failing Vulkan result.
     *
     * \ref COCKTAIL_VK_CHECK throws this exception when a Vulkan call does not
     * return \c VK_SUCCESS. Use \ref GetErrorCode to inspect the original
     * \c VkResult.
     */
    using ApiException = GenericException<VkResult, ApiErrorCodeTranslator>;

    /**
     * \brief Inserts \p rhs directly after \p lhs in a Vulkan structure chain.
     *
     * \param lhs Structure whose next pointer receives \p rhs.
     * \param rhs Structure to insert. It can already lead its own chain only when
     *         \p lhs has no next structure.
     *
     * \throws InvalidParameterException When both \p lhs and \p rhs already have
     *         a next structure, which would discard the latter chain.
     */
    template <typename T, typename U>
    void Chain(T& lhs, U& rhs)
    {
        if (lhs.pNext)
        {
            if (rhs.pNext != nullptr)
                throw InvalidParameterException(CK_TEXT("Cannot chain a structure that already has a next structure"));

            rhs.pNext = const_cast<void*>(lhs.pNext);
        }

        lhs.pNext = &rhs;
    }

    /**
     * \brief
     * \param swizzle
     * \return
     */
    VkComponentSwizzle ToVkType(Renderer::TextureSwizzleValue swizzle);

    /**
     * \brief
     * \param colorSpace
     * \return
     */
    Renderer::ColorSpace FromVkType(VkColorSpaceKHR colorSpace);

    /**
     * \brief
     * \param colorSpace
     * \return
     */
    VkColorSpaceKHR ToVkType(Renderer::ColorSpace colorSpace);

    /**
     * \brief
     * \param compareOp
     * \return
     */
    VkCompareOp ToVkType(Renderer::CompareOp compareOp);

    /**
     * \brief
     * \param format
     * \return
     */
    PixelFormat FromVkType(VkFormat format);

    /**
     * \brief
     * \param format
     * \return
     */
    VkFormat ToVkType(const PixelFormat& format);

    /**
     * \brief
     */
    VkPrimitiveTopology ToVkType(Renderer::PrimitiveTopology primitiveTopology);

    /**
     * \brief
     * \param samples
     * \return
     */
    Renderer::RasterizationSamples FromVkType(VkSampleCountFlagBits samples);

    /**
     * \brief
     * \param samples
     * \return
     */
    VkSampleCountFlagBits ToVkType(Renderer::RasterizationSamples samples);

    /**
     * \brief
     * \param extent
     * \return
     */
    VkOffset2D ToVkType(const Extent2D<int>& extent);

    /**
     * \brief
     * \param extent
     * \return
     */
    VkOffset3D ToVkType(const Extent3D<int>& extent);

    /**
     * \brief
     * \param extent
     * \return
     */
    VkExtent2D ToVkType(const Extent2D<unsigned int>& extent);

    /**
     * \brief
     * \param extent
     * \return
     */
    VkExtent3D ToVkType(const Extent3D<unsigned int>& extent);

    /**
     * \brief
     * \param type
     * \return
     */
    VkImageType ToVkType(Renderer::TextureType type);

    /**
     * \brief
     * \param usage
     * \return
     */
    VkBufferUsageFlags ToVkType(Renderer::BufferUsageFlagBits usage);

    /**
     * \brief
     * \param usage
     * \return
     */
    VkBufferUsageFlags ToVkTypes(const Renderer::BufferUsageFlags& usage);

    /**
     * \brief
     * \param type
     * \return
     */
    VkShaderStageFlagBits ToVkType(Renderer::ShaderType type);

    /**
     * \brief
     * \param types
     * \return
     */
    VkShaderStageFlags ToVkTypes(const Flags<Renderer::ShaderType>& types);

    /**
     * \brief
     * \param viewport
     * \return
     */
    VkViewport ToVkType(const Renderer::Viewport& viewport);

    /**
     * \brief
     * \param scissor
     * \return
     */
    VkRect2D ToVkType(const Renderer::Scissor& scissor);

    /**
     * \brief
     * \param blendFactor
     * \return
     */
    VkBlendFactor ToVkType(Renderer::BlendFactor blendFactor);

    /**
     * \brief
     * \param blendOp
     * \return
     */
    VkBlendOp ToVkType(Renderer::BlendOp blendOp);

    /**
     * \brief
     * \param polygonMode
     * \return
     */
    VkPolygonMode ToVkType(Renderer::PolygonMode polygonMode);

    /**
     * \brief
     * \param cullMode
     * \return
     */
    VkCullModeFlagBits ToVkType(Renderer::CullMode cullMode);

    /**
     * \brief
     * \param frontFace
     * \return
     */
    VkFrontFace ToVkType(Renderer::FrontFace frontFace);

    /**
     * \brief
     * \param logicOp
     * \return
     */
    VkLogicOp ToVkType(Renderer::LogicOp logicOp);

    /**
     * \brief
     * \param indexType
     * \return
     */
    VkIndexType ToVkType(Renderer::IndexType indexType);

    /**
     * \brief
     * \param descriptorType
     * \return
     */
    VkDescriptorType ToVkType(Renderer::DescriptorType descriptorType);

    /**
     * \brief
     * \param filter
     * \return
     */
    VkFilter ToVkType(Renderer::Filter filter);

    /**
     * \brief
     * \param mipmapMode
     * \return
     */
    VkSamplerMipmapMode ToVkType(Renderer::SamplerMipmapMode mipmapMode);

    /**
     * \brief
     * \param addressMode
     * \return
     */
    VkSamplerAddressMode ToVkType(Renderer::SamplerAddressMode addressMode);

    /**
     * \brief
     * \param resolveMode
     * \return
     */
    VkResolveModeFlagBitsKHR ToVkType(Renderer::ResolveMode resolveMode);

    /**
     * \brief
     * \param combiner
     * \return
     */
    VkFragmentShadingRateCombinerOpKHR ToVkType(Renderer::ShadingRateCombiner combiner);

    VkLineRasterizationModeKHR ToVkType(Renderer::LineRasterizationMode mode);
}

#endif // COCKTAILENGINE_VULKAN_VULKANUTILS_HPP
