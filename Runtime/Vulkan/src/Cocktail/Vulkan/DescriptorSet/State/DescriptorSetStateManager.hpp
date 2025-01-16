#ifndef COCKTAIL_VULKAN_DESCRIPTORSET_STATE_DESCRIPTORSETSTATEMANAGER_HPP
#define COCKTAIL_VULKAN_DESCRIPTORSET_STATE_DESCRIPTORSETSTATEMANAGER_HPP

#include <Cocktail/Vulkan/DescriptorSet/DescriptorUpdateTemplate.hpp>
#include <Cocktail/Vulkan/DescriptorSet/Allocator/DescriptorSetAllocator.hpp>
#include <Cocktail/Vulkan/DescriptorSet/Layout/DescriptorSetLayout.hpp>

namespace Ck::Vulkan
{
	class Buffer;
	class Sampler;
	class TextureView;

	struct ImageStateInfo
	{
		const TextureView* TextureView = nullptr;
		const Sampler* Sampler = nullptr;
	};

	struct BufferStateInfo
	{
		const Buffer* Buffer = nullptr;
		std::size_t Offset = 0;
		std::size_t Range = 0;
	};

	struct DescriptorState
	{
		unsigned int Binding = 0;
		unsigned int ArrayElement = 0;
		Renderer::DescriptorType Type = Renderer::DescriptorType::Texture;
		ImageStateInfo ImageInfo;
		BufferStateInfo BufferInfo;
		bool Dirty = false;
	};

	class DescriptorSetStateManager
	{
	public:

		/**
		 * \brief 
		 */
		DescriptorSetStateManager() = delete;
		
		/**
		 * \brief Constructor
		 * \param renderDevice 
		 * \param descriptorSetAllocator 
		 */
		explicit DescriptorSetStateManager(Ref<RenderDevice> renderDevice, Ref<DescriptorSetAllocator> descriptorSetAllocator);

		/**
		 * \brief 
		 * \param other 
		 */
		DescriptorSetStateManager(const DescriptorSetStateManager& other) = delete;

		/**
		 * \brief 
		 * \param other 
		 */
		DescriptorSetStateManager(DescriptorSetStateManager&& other) noexcept = default;

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		DescriptorSetStateManager& operator=(const DescriptorSetStateManager& other) = delete;

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		DescriptorSetStateManager& operator=(DescriptorSetStateManager&& other) noexcept = default;

		/**
		 * \brief 
		 * \param binding
		 * \param arrayIndex 
		 * \param sampler
		 * \return
		 */
		bool BindSampler(unsigned int binding, unsigned int arrayIndex, const Sampler* sampler);

		/**
		 * \brief 
		 * \param binding
		 * \param arrayIndex 
		 * \param textureView 
		 * \param sampler
		 * \return
		 */
		bool BindTextureSampler(unsigned int binding, unsigned int arrayIndex, const TextureView* textureView, const Sampler* sampler);

		/**
		 * \brief
		 * \param binding
		 * \param arrayIndex
		 * \param textureView
		 * \return
		 */
		bool BindTexture(unsigned int binding, unsigned int arrayIndex, const TextureView* textureView);

		/**
		 * \brief
		 * \param binding
		 * \param arrayIndex
		 * \param textureView
		 * \return
		 */
		bool BindStorageTexture(unsigned int binding, unsigned int arrayIndex, const TextureView* textureView);

		/**
		 * \brief 
		 * \param binding
		 * \param arrayIndex 
		 * \param buffer 
		 * \param offset 
		 * \param range 
		 * \return
		 */
		bool BindUniformBuffer(unsigned int binding, unsigned int arrayIndex, const Buffer* buffer, std::size_t offset, std::size_t range);

		/**
		 * \brief
		 * \param binding
		 * \param arrayIndex
		 * \param buffer
		 * \param offset
		 * \param range
		 * \return
		 */
		bool BindStorageBuffer(unsigned int binding, unsigned int arrayIndex, const Buffer* buffer, std::size_t offset, std::size_t range);

		/**
		 * \brief 
		 */
		void ResetBindings();

		/**
		 * \brief 
		 * \param layout
		 * \param imagesInfo
		 * \param buffersInfo 
		 * \param writes 
		 * \return 
		 */
		unsigned int CompileDescriptors(const Ref<DescriptorSetLayout>& layout, VkDescriptorImageInfo* imagesInfo, VkDescriptorBufferInfo* buffersInfo, VkWriteDescriptorSet* writes);

		/**
		 * \brief 
		 * \param layout 
		 * \param descriptorUpdateTemplate 
		 * \param descriptors 
		 */
		void CompileDescriptorsWithTemplate(const Ref<DescriptorSetLayout>& layout, const Ref<DescriptorUpdateTemplate>& descriptorUpdateTemplate, unsigned char* descriptors);

		/**
		 * \brief 
		 * \param descriptorSetLayout 
		 * \return 
		 */
		Ref<DescriptorSet> CompileSet(const Ref<DescriptorSetLayout>& descriptorSetLayout);

		/**
		 * \brief 
		 * \param descriptorSetLayout 
		 * \param descriptorUpdateTemplate 
		 * \return 
		 */
		Ref<DescriptorSet> CompileSetWithTemplate(const Ref<DescriptorSetLayout>& descriptorSetLayout, const Ref<DescriptorUpdateTemplate>& descriptorUpdateTemplate);

		/**
		 * \brief 
		 * \param binding 
		 * \return 
		 */
		bool IsBindingDirty(unsigned int binding) const;

	private:

		/**
		 * \brief
		 * \param state
		 * \param type
		 * \param textureView
		 * \param sampler
		 * \return
		 */
		static bool FillImageState(DescriptorState* state, Renderer::DescriptorType type, const TextureView* textureView, const Sampler* sampler);

		/**
		 * \brief 
		 * \param state 
		 * \param type 
		 * \param buffer 
		 * \param offset 
		 * \param range 
		 * \return 
		 */
		static bool FillBufferState(DescriptorState* state, Renderer::DescriptorType type, const Buffer* buffer, std::size_t offset, std::size_t range);

		/**
		 * \brief
		 * \param binding
		 * \param arrayElement
		 * \return
		 */
		DescriptorState* FindDescriptorState(unsigned int binding, unsigned int arrayElement);

		/**
		 * \brief 
		 * \param binding
		 * \param arrayElement 
		 * \return 
		 */
		DescriptorState* FindOrCreateDescriptorState(unsigned int binding, unsigned int arrayElement);

		Ref<RenderDevice> mRenderDevice;
		Ref<DescriptorSetAllocator> mDescriptorSetAllocator;
		Ref<DescriptorSet> mLastSet;
		std::vector<DescriptorState> mBindingStates;
		unsigned int mBindingDirtyFlags;
	};
}

#endif // COCKTAIL_VULKAN_DESCRIPTORSET_STATE_DESCRIPTORSETSTATEMANAGER_HPP
