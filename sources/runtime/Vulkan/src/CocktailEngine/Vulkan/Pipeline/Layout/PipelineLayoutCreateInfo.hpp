#ifndef COCKTAILENGINE_VULKAN_PIPELINE_LAYOUT_PIPELINELAYOUTCREATEINFO_HPP
#define COCKTAILENGINE_VULKAN_PIPELINE_LAYOUT_PIPELINELAYOUTCREATEINFO_HPP

#include <CocktailEngine/Core/Utility/EnumMap.hpp>

#include <CocktailEngine/Vulkan/DescriptorSet/Layout/DescriptorSetLayout.hpp>
#include <CocktailEngine/Vulkan/Shader/Reflection/PushConstantBlock.hpp>

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
		Array<SharedPtr<DescriptorSetLayout>> DescriptorSetLayouts;

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

#endif // COCKTAILENGINE_VULKAN_PIPELINE_LAYOUT_PIPELINELAYOUTCREATEINFO_HPP
