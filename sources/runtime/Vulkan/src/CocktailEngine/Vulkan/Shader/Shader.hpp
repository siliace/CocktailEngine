#ifndef COCKTAILENGINE_VULKAN_SHADER_SHADER_HPP
#define COCKTAILENGINE_VULKAN_SHADER_SHADER_HPP

#include <CocktailEngine/Renderer/Shader/Shader.hpp>
#include <CocktailEngine/Renderer/Shader/ShaderCreateInfo.hpp>

#include <CocktailEngine/Vulkan/Shader/Reflection/DescriptorSetInfo.hpp>
#include <CocktailEngine/Vulkan/Shader/Reflection/PushConstantBlock.hpp>

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
		Shader(RenderDevice* renderDevice, const ValidationCache* validationCache, const Renderer::ShaderCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

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
		Renderer::RenderDevice* GetRenderDevice() const override;

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
		Renderer::VertexAttributeLocation* FindInputAttribute(AsciiStringView name) const override;

		/**
		 * \brief 
		 * \param name 
		 * \return 
		 */
		Renderer::VertexAttributeLocation* FindOutputAttribute(AsciiStringView name) const override;

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

		RenderDevice* mRenderDevice;
		const VkAllocationCallbacks* mAllocationCallbacks;
		VkShaderModule mHandle;
		Renderer::ShaderType mType;
		Array<DescriptorSetInfo> mDescriptorSetInfo;
		Array<PushConstantBlockInfo> mPushConstantBlockInfo;
		Array<UniquePtr<VertexAttributeLocation>> mInputAttributeLocations;
		Array<UniquePtr<VertexAttributeLocation>> mOutputAttributeLocations;
	};
}

#endif // COCKTAILENGINE_VULKAN_SHADER_SHADER_HPP
