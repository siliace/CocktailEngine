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
	class ShaderProgram : public Inherit<ShaderProgram, Object, Renderer::ShaderProgram>
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		ShaderProgram(const Ref<RenderDevice>& renderDevice, const Renderer::ShaderProgramCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 * \param name 
		 */
		void SetObjectName(const char* name) const override;

		/**
		 * \brief 
		 * \return 
		 */
		Ref<Renderer::RenderDevice> GetRenderDevice() const override;
		
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
		Ref<Renderer::Shader> GetStage(Renderer::ShaderType type) const override;

		/**
		 * \brief 
		 * \param name 
		 * \return 
		 */
		Renderer::UniformSlot* FindUniformSlot(std::string_view name) const override;

		/**
		 * \brief 
		 * \return 
		 */
		Ref<PipelineLayout> GetPipelineLayout() const;

	private:

		/**
		 * \brief Create the pipeline layout to use along with this shader
		 */
		void CreatePipelineLayout();

		/**
		 * \brief Create uniform locations from the pipeline layout
		 */
		void CreateUniformSlots();

		Ref<RenderDevice> mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		Renderer::ShaderProgramType mType;
		EnumMap<Renderer::ShaderType, Ref<Shader>> mShaders;
		std::vector<std::unique_ptr<UniformSlot>> mUniformSlots;
		Ref<PipelineLayout> mPipelineLayout;
	};
}

#endif // COCKTAIL_VULKAN_SHADER_SHADERPROGRAM_HPP
