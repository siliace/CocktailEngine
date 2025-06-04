#ifndef COCKTAIL_VULKAN_PIPELINE_STATE_COMPUTESTATEMANAGER_HPP
#define COCKTAIL_VULKAN_PIPELINE_STATE_COMPUTESTATEMANAGER_HPP

#include <Cocktail/Vulkan/Pipeline/State/ComputeState.hpp>
#include <Cocktail/Vulkan/Pipeline/State/StateManager.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief
	 */
	class ComputeStateManager : public StateManager
	{
	public:

		/**
		 * \brief
		 * \param renderDevice
		 * \param descriptorSetAllocator
		 */
		ComputeStateManager(std::shared_ptr<RenderDevice> renderDevice, DescriptorSetAllocator* descriptorSetAllocator);

		/**
		 * \brief 
		 * \param shaderProgram 
		 */
		void SetShaderProgram(const ShaderProgram* shaderProgram) override;

		/**
		 * \brief 
		 * \param shaderType 
		 * \param name 
		 */
		void SetEntryPoint(Renderer::ShaderType shaderType, const char* name) override;

		/**
		 * \brief 
		 * \return 
		 */
		std::shared_ptr<Pipeline> CompilePipeline() override;

	private:

		ComputeState mState;
		const ShaderProgram* mShaderProgram;
	};
}

#endif // COCKTAIL_VULKAN_PIPELINE_STATE_COMPUTESTATEMANAGER_HPP
