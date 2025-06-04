#ifndef COCKTAIL_VULKAN_SHADER_SHADER_HPP
#define COCKTAIL_VULKAN_SHADER_SHADER_HPP

#include <Cocktail/Renderer/Shader/Shader.hpp>
#include <Cocktail/Renderer/Shader/ShaderCreateInfo.hpp>

#include <Cocktail/Vulkan/Shader/Reflection/DescriptorSetInfo.hpp>
#include <Cocktail/Vulkan/Shader/Reflection/PushConstantBlock.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;
	class ValidationCache;
	class VertexAttributeLocation;
	
	/**
	 * \brief 
	 */
	class Shader : public Renderer::Shader
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice
		 * \param validationCache 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		Shader(std::shared_ptr<RenderDevice> renderDevice, const ValidationCache* validationCache, const Renderer::ShaderCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

		/**
		 * \brief 
		 */
		~Shader() override;

		/**
		 * \brief 
		 * \param name 
		 */
		void SetObjectName(const char* name) const override;

		/**
		 * \brief 
		 * \return 
		 */
		std::shared_ptr<Renderer::RenderDevice> GetRenderDevice() const override;

		/**
		 * \brief
		 * \return
		 */
		Renderer::ShaderType GetType() const override;

		/**
		 * \brief 
		 * \param name 
		 * \return 
		 */
		Renderer::VertexAttributeLocation* FindInputAttribute(std::string_view name) const override;

		/**
		 * \brief 
		 * \param name 
		 * \return 
		 */
		Renderer::VertexAttributeLocation* FindOutputAttribute(std::string_view name) const override;

		/**
		 * \brief
		 * \return
		 */
		unsigned int GetDescriptorSetInfoCount() const;
		
		/**
		 * \brief 
		 * \param index 
		 * \return 
		 */
		const DescriptorSetInfo* GetDescriptorSetInfo(unsigned int index) const;

		/**
		 * \brief 
		 * \return 
		 */
		unsigned int GetPushConstantBlockCount() const;

		/**
		 * \brief 
		 * \param index 
		 * \return 
		 */
		const PushConstantBlockInfo* GetPushConstantBlock(unsigned int index) const;

		/**
		 * \brief 
		 * \param set 
		 * \param binding 
		 * \return 
		 */
		Optional<DescriptorSetBindingInfo> GetDescriptorSetBindingInfo(unsigned int set, unsigned int binding) const;

		/**
		 * \brief 
		 * \return 
		 */
		VkShaderModule GetHandle() const;

	private:

		std::shared_ptr<RenderDevice> mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkShaderModule mHandle;
		Renderer::ShaderType mType;
		std::vector<DescriptorSetInfo> mDescriptorSetInfo;
		std::vector<PushConstantBlockInfo> mPushConstantBlockInfo;
		std::vector<std::unique_ptr<VertexAttributeLocation>> mInputAttributeLocations;
		std::vector<std::unique_ptr<VertexAttributeLocation>> mOutputAttributeLocations;
	};
}

#endif // COCKTAIL_VULKAN_SHADER_SHADER_HPP
