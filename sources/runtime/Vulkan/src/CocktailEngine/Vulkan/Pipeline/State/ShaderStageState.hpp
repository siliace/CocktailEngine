#ifndef COCKTAILENGINE_VULKAN_PIPELINE_STATE_SHADERSTAGESTATE_HPP
#define COCKTAILENGINE_VULKAN_PIPELINE_STATE_SHADERSTAGESTATE_HPP

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
		const Shader* StageShader = nullptr;

		/**
		 * \brief 
		 */
		const AnsiChar* EntryPoint = "main";
	};
}

#endif // COCKTAILENGINE_VULKAN_PIPELINE_STATE_SHADERSTAGESTATE_HPP
