#ifndef COCKTAIL_VULKAN_SHADER_SLOT_PIPELINECONSTANTSLOT_HPP
#define COCKTAIL_VULKAN_SHADER_SLOT_PIPELINECONSTANTSLOT_HPP

#include <Cocktail/Core/Meta/Extends.hpp>

#include <Cocktail/Vulkan/Shader/Reflection/PushConstantBlock.hpp>
#include <Cocktail/Vulkan/Shader/Slot/UniformSlot.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief 
	 */
	class PipelineConstantSlot : public Extends<PipelineConstantSlot, UniformSlot>
	{
	public:

		/**
		 * \brief 
		 * \param programType 
		 * \param shaderType
		 * \param pushConstantBlock 
		 */
		PipelineConstantSlot(Renderer::ShaderProgramType programType, Renderer::ShaderType shaderType, const PushConstantBlockInfo& pushConstantBlock);

		/**
		 * \brief 
		 * \param offset 
		 * \return 
		 */
		unsigned int ComputeOffset(unsigned int offset) const;

		/**
		 * \brief 
		 * \return 
		 */
		Renderer::DescriptorType GetDescriptorType() const override;

		/**
		 * \brief 
		 * \return 
		 */
		bool IsArray() const override;

		/**
		 * \brief 
		 * \return 
		 */
		unsigned GetArrayLength() const override;

		/**
		 * \brief 
		 * \return 
		 */
		Flags<Renderer::ShaderType> GetShaderStages() const override;

	private:

		Renderer::ShaderType mShaderType;
		unsigned int mBaseOffset;
	};
}

#endif // COCKTAIL_VULKAN_SHADER_SLOT_PIPELINECONSTANTSLOT_HPP
