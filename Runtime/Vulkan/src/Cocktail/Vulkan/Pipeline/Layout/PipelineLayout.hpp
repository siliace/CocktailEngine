#ifndef COCKTAIL_VULKAN_PIPELINE_LAYOUT_PIPELINELAYOUT_HPP
#define COCKTAIL_VULKAN_PIPELINE_LAYOUT_PIPELINELAYOUT_HPP

#include <Cocktail/Renderer/RenderDeviceObject.hpp>

#include <Cocktail/Vulkan/Volk.hpp>
#include <Cocktail/Vulkan/Pipeline/Layout/PipelineLayoutCreateInfo.hpp>

namespace Ck::Vulkan
{
	class DescriptorUpdateTemplate;
	class RenderDevice;

	/**
	 * \brief 
	 */
	class PipelineLayout : public Renderer::RenderDeviceObject
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		PipelineLayout(RenderDevice* renderDevice, const PipelineLayoutCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

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
		Renderer::RenderDevice* GetRenderDevice() const override;

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
		std::shared_ptr<DescriptorSetLayout> GetDescriptorSetLayout(unsigned int set) const;

		/**
		 * \brief 
		 * \return 
		 */
		const Array<std::shared_ptr<DescriptorSetLayout>>& GetDescriptorSetLayouts();

		/**
		 * \brief 
		 * \param set 
		 * \return 
		 */
		std::shared_ptr<DescriptorUpdateTemplate> GetDescriptorUpdateTemplate(unsigned int set) const;

		/**
		 * \brief 
		 * \return 
		 */
		Optional<const PushConstantBlockInfo&> GetPipelineConstantBlock(Renderer::ShaderType shaderType) const;

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

		RenderDevice* mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkPipelineLayout mHandle;
		Array<std::shared_ptr<DescriptorSetLayout>> mDescriptorSetLayouts;
		Array<std::shared_ptr<DescriptorUpdateTemplate>> mUpdateTemplates;
		EnumMap<Renderer::ShaderType, Optional<PushConstantBlockInfo>> mPipelineConstantBlocks;
		VkPipelineBindPoint mBindPoint;
	};
}

#endif // COCKTAIL_VULKAN_PIPELINE_LAYOUT_PIPELINELAYOUT_HPP
