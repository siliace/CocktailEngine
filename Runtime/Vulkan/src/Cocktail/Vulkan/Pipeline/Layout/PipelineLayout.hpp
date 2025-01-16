#ifndef COCKTAIL_VULKAN_PIPELINE_LAYOUT_PIPELINELAYOUT_HPP
#define COCKTAIL_VULKAN_PIPELINE_LAYOUT_PIPELINELAYOUT_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>

#include <Cocktail/Renderer/RenderDeviceObject.hpp>

#include <Cocktail/Vulkan/Volk.hpp>
#include <Cocktail/Vulkan/DescriptorSet/DescriptorUpdateTemplate.hpp>
#include <Cocktail/Vulkan/Pipeline/Layout/PipelineLayoutCreateInfo.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;

	/**
	 * \brief 
	 */
	class PipelineLayout : public Inherit<PipelineLayout, Object, Renderer::RenderDeviceObject>
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		PipelineLayout(const Ref<RenderDevice>& renderDevice, const PipelineLayoutCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 */
		~PipelineLayout() override;

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
		unsigned int GetDescriptorSetLayoutCount() const;

		/**
		 * \brief 
		 * \param set 
		 * \return 
		 */
		Ref<DescriptorSetLayout> GetDescriptorSetLayout(unsigned int set) const;

		/**
		 * \brief 
		 * \param set 
		 * \return 
		 */
		Ref<DescriptorUpdateTemplate> GetDescriptorUpdateTemplate(unsigned int set) const;

		/**
		 * \brief 
		 * \return 
		 */
		const Optional<PushConstantBlockInfo>& GetPipelineConstantBlock(Renderer::ShaderType shaderType) const;

		/**
		 * \brief 
		 * \return 
		 */
		VkPipelineBindPoint GetBindPoint() const;

		/**
		 * \brief 
		 * \return 
		 */
		VkPipelineLayout GetHandle() const;

	private:

		Ref<RenderDevice> mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkPipelineLayout mHandle;
		std::vector<Ref<DescriptorSetLayout>> mDescriptorSetLayouts;
		std::vector<Ref<DescriptorUpdateTemplate>> mUpdateTemplates;
		EnumMap<Renderer::ShaderType, Optional<PushConstantBlockInfo>> mPipelineConstantBlocks;
		VkPipelineBindPoint mBindPoint;
	};
}

#endif // COCKTAIL_VULKAN_PIPELINE_LAYOUT_PIPELINELAYOUT_HPP
