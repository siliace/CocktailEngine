#ifndef COCKTAILENGINE_VULKAN_PIPELINE_LAYOUT_PIPELINELAYOUT_HPP
#define COCKTAILENGINE_VULKAN_PIPELINE_LAYOUT_PIPELINELAYOUT_HPP

#include <CocktailEngine/Renderer/RenderDeviceObject.hpp>

#include <CocktailEngine/Vulkan/Volk.hpp>
#include <CocktailEngine/Vulkan/Pipeline/Layout/PipelineLayoutCreateInfo.hpp>

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
		SharedPtr<DescriptorSetLayout> GetDescriptorSetLayout(unsigned int set) const;

		/**
		 * \brief 
		 * \return 
		 */
		const Array<SharedPtr<DescriptorSetLayout>>& GetDescriptorSetLayouts();

		/**
		 * \brief 
		 * \param set 
		 * \return 
		 */
		SharedPtr<DescriptorUpdateTemplate> GetDescriptorUpdateTemplate(unsigned int set) const;

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
		Array<SharedPtr<DescriptorSetLayout>> mDescriptorSetLayouts;
		Array<SharedPtr<DescriptorUpdateTemplate>> mUpdateTemplates;
		EnumMap<Renderer::ShaderType, Optional<PushConstantBlockInfo>> mPipelineConstantBlocks;
		VkPipelineBindPoint mBindPoint;
	};
}

#endif // COCKTAILENGINE_VULKAN_PIPELINE_LAYOUT_PIPELINELAYOUT_HPP
