#ifndef COCKTAIL_VULKAN_VULKANUTILS_HPP
#define COCKTAIL_VULKAN_VULKANUTILS_HPP

#include <Cocktail/Renderer/ColorSpace.hpp>
#include <Cocktail/Renderer/RasterizationSamples.hpp>
#include <Cocktail/Renderer/Renderer.hpp>
#include <Cocktail/Renderer/Buffer/BufferUsage.hpp>
#include <Cocktail/Renderer/Buffer/IndexType.hpp>
#include <Cocktail/Renderer/Framebuffer/FramebufferCreateInfo.hpp>
#include <Cocktail/Renderer/Shader/DescriptorType.hpp>
#include <Cocktail/Renderer/Shader/ShaderType.hpp>
#include <Cocktail/Renderer/Texture/SamplerCreateInfo.hpp>
#include <Cocktail/Renderer/Texture/TextureCreateInfo.hpp>
#include <Cocktail/Renderer/Texture/TextureSwizzle.hpp>

#include <Cocktail/Vulkan/Volk.hpp>

#define COCKTAIL_VK_CHECK(call) \
	do { \
		VkResult result_ = call; \
		assert(result_ == VK_SUCCESS); \
	} while (0)

namespace Ck::Vulkan
{
	template <typename T, typename U>
	void Chain(T& lhs, U& rhs)
	{
		if (lhs.pNext)
		{
			if (rhs.pNext)
				throw std::invalid_argument("Right chain object already has pNext");

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
}

#endif // COCKTAIL_VULKAN_VULKANUTILS_HPP
