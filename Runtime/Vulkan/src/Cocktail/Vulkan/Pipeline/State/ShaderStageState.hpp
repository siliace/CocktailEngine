#ifndef COCKTAIL_VULKAN_PIPELINE_STATE_SHADERSTAGESTATE_HPP
#define COCKTAIL_VULKAN_PIPELINE_STATE_SHADERSTAGESTATE_HPP

namespace Ck::Vulkan
{
	class Shader;

	/**
	 * \brief 
	 */
	struct ShaderStageState
	{
		/**
		 * \brief 
		 */
		const Shader* Shader = nullptr;

		/**
		 * \brief 
		 */
		const AnsiChar* EntryPoint = "main";
	};
}

#endif // COCKTAIL_VULKAN_PIPELINE_STATE_SHADERSTAGESTATE_HPP
