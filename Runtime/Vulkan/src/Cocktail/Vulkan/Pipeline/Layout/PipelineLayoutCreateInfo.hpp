#ifndef COCKTAIL_VULKAN_PIPELINE_LAYOUT_PIPELINELAYOUTCREATEINFO_HPP
#define COCKTAIL_VULKAN_PIPELINE_LAYOUT_PIPELINELAYOUTCREATEINFO_HPP

#include <Cocktail/Core/Utility/EnumMap.hpp>

#include <Cocktail/Vulkan/DescriptorSet/Layout/DescriptorSetLayout.hpp>
#include <Cocktail/Vulkan/Shader/Reflection/PushConstantBlock.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 */
	struct PipelineLayoutCreateInfo
	{
		/**
		 * \brief 
		 */
		VkPipelineBindPoint BindPoint = VK_PIPELINE_BIND_POINT_MAX_ENUM;

		/**
		 * \brief 
		 */
		std::vector<Ref<DescriptorSetLayout>> DescriptorSetLayouts;

		/**
		 * \brief 
		 */
		EnumMap<Renderer::ShaderType, Optional<PushConstantBlockInfo>> PushConstantBlocks;

		/**
		 * \brief 
		 */
		const char* Name = nullptr;
	};
}

#endif // COCKTAIL_VULKAN_PIPELINE_LAYOUT_PIPELINELAYOUTCREATEINFO_HPP
