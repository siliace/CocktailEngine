#ifndef COCKTAIL_VULKAN_SHADER_SHADERPROGRAM_HPP
#define COCKTAIL_VULKAN_SHADER_SHADERPROGRAM_HPP

#include <Cocktail/Core/Utility/EnumMap.hpp>

#include <Cocktail/Renderer/Shader/ShaderProgram.hpp>
#include <Cocktail/Renderer/Shader/ShaderProgramCreateInfo.hpp>

#include <Cocktail/Vulkan/Pipeline/Layout/PipelineLayout.hpp>
#include <Cocktail/Vulkan/Shader/Shader.hpp>
#include <Cocktail/Vulkan/Shader/UniformSlot.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief 
	 */
	class ShaderProgram : public Renderer::ShaderProgram
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		ShaderProgram(RenderDevice* renderDevice, const Renderer::ShaderProgramCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 * \param name 
		 */
		void SetObjectName(const char* name) const override;

		/**
		 * \brief 
		 * \return 
		 */
		Renderer::RenderDevice* GetRenderDevice() const override;
		
		/**
		 * \brief 
		 * \return 
		 */
		Renderer::ShaderProgramType GetType() const override;

		/**
		 * \brief 
		 * \param type 
		 * \return 
		 */
		std::shared_ptr<Renderer::Shader> GetStage(Renderer::ShaderType type) const override;

		/**
		 * \brief 
		 * \param name 
		 * \return 
		 */
		Renderer::UniformSlot* FindUniformSlot(AnsiStringView name) const override;

		/**
		 * \brief 
		 * \return 
		 */
		std::shared_ptr<PipelineLayout> GetPipelineLayout() const;

	private:

		/**
		 * \brief 
		 * \param staticSamplerCount 
		 * \param staticSamplers 
		 */
		void CreatePipelineLayout(unsigned int staticSamplerCount, const Renderer::StaticSamplerInfo* staticSamplers);

		/**
		 * \brief Create uniform locations from the pipeline layout
		 */
		void CreateUniformSlots();

		RenderDevice* mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		Renderer::ShaderProgramType mType;
		EnumMap<Renderer::ShaderType, std::shared_ptr<Shader>> mShaders;
		Array<UniquePtr<UniformSlot>> mUniformSlots;
		std::shared_ptr<PipelineLayout> mPipelineLayout;
	};
}

#endif // COCKTAIL_VULKAN_SHADER_SHADERPROGRAM_HPP
