#ifndef COCKTAIL_VULKAN_PIPELINE_STATE_SHADERSTAGESTATE_HPP
#define COCKTAIL_VULKAN_PIPELINE_STATE_SHADERSTAGESTATE_HPP

#include <Cocktail/Vulkan/Shader/Shader.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 */
	struct ShaderStageState
	{
		const Shader* Shader = nullptr;
		const char* EntryPoint = "main";
	};
}

#endif // COCKTAIL_VULKAN_PIPELINE_STATE_SHADERSTAGESTATE_HPP
