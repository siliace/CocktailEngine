#ifndef COCKTAIL_VULKAN_SHADER_SHADER_HPP
#define COCKTAIL_VULKAN_SHADER_SHADER_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>

#include <Cocktail/Renderer/Shader/Shader.hpp>
#include <Cocktail/Renderer/Shader/ShaderCreateInfo.hpp>

#include <Cocktail/Vulkan/Volk.hpp>
#include <Cocktail/Vulkan/Shader/ValidationCache.hpp>
#include <Cocktail/Vulkan/Shader/Reflection/DescriptorSetInfo.hpp>
#include <Cocktail/Vulkan/Shader/Reflection/PushConstantBlock.hpp>

namespace Ck::Vulkan
{
	class RenderDevice;
	class VertexAttributeLocation;
	
	/**
	 * \brief 
	 */
	class Shader : public Inherit<Shader, Object, Renderer::Shader>
	{
	public:

		/**
		 * \brief 
		 * \param renderDevice
		 * \param validationCache 
		 * \param createInfo 
		 * \param allocationCallbacks 
		 */
		Shader(const Ref<RenderDevice>& renderDevice, const ValidationCache* validationCache, const Renderer::ShaderCreateInfo& createInfo, const VkAllocationCallbacks* allocationCallbacks);

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
		Ref<Renderer::RenderDevice> GetRenderDevice() const override;

		/**
		 * \brief
		 * \return
		 */
		Renderer::ShaderType GetType() const override;

		/**
		 * \brief 
		 * \return 
		 */
		std::size_t GetInputAttributeCount() const override;

		/**
		 * \brief 
		 * \param locations 
		 * \param count 
		 * \param first 
		 * \return 
		 */
		std::size_t GetInputAttributes(Renderer::VertexAttributeLocation** locations, std::size_t count, std::size_t first) const override;

		/**
		 * \brief 
		 * \return 
		 */
		std::size_t GetOutputAttributeCount() const override;

		/**
		 * \brief 
		 * \param locations 
		 * \param count 
		 * \param first 
		 * \return 
		 */
		std::size_t GetOutputAttributes(Renderer::VertexAttributeLocation** locations, std::size_t count, std::size_t first) const override;

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

		Ref<RenderDevice> mRenderDevice;
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
