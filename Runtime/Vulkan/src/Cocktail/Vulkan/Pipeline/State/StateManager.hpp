#ifndef COCKTAIL_VULKAN_PIPELINE_STATE_STATEMANAGER_HPP
#define COCKTAIL_VULKAN_PIPELINE_STATE_STATEMANAGER_HPP

#include <Cocktail/Core/Flags.hpp>
#include <Cocktail/Core/Utility/EnumMap.hpp>

#include <Cocktail/Vulkan/Buffer/Buffer.hpp>
#include <Cocktail/Vulkan/DescriptorSet/State/DescriptorSetStateManager.hpp>
#include <Cocktail/Vulkan/Shader/Shader.hpp>
#include <Cocktail/Vulkan/Shader/ShaderProgram.hpp>

namespace Ck::Vulkan
{
	class Pipeline;
	class RenderDevice;

	struct PipelineConstantRange
	{
		unsigned int Offset = 0;
		unsigned int Size = 0;
		const void* Data = nullptr;
	};

	struct DescriptorSetRange
	{
		unsigned int DescriptorSetCount = 0;
		VkDescriptorSet DescriptorSetHandles[32] = {};
		unsigned int DynamicOffsetCount = 0;
		unsigned int DynamicOffsets[32] = {};
	};

	class StateManager
	{
	public:

		/**
		 * \brief Enumeration of state that might changes
		 */
		enum class DirtyFlagBits
		{
			/**
			 * \brief Flag indicating the pipeline state has changed
			 */
			Pipeline = Bit(0),

			/**
			 * \brief Flag indicating at least one binding of one descriptor set has changed
			 */
			DescriptorSet = Bit(1),

			/**
			 * \brief 
			 */
			PipelineConstant = Bit(2),
		};

		/**
		 * \brief 
		 * \param renderDevice 
		 * \param descriptorSetAllocator 
		 */
		explicit StateManager(Ref<RenderDevice> renderDevice, Ref<DescriptorSetAllocator> descriptorSetAllocator);

		/**
		 * \brief 
		 */
		virtual ~StateManager() = default;

		/**
		 * \brief 
		 * \param shaderProgram
		 */
		virtual void SetShaderProgram(const ShaderProgram* shaderProgram);

		/**
		 * \brief 
		 * \param shaderType 
		 * \param name 
		 */
		virtual void SetEntryPoint(Renderer::ShaderType shaderType, const char* name) = 0;

		/**
		 * \brief
		 * \param set
		 * \param binding
		 * \param arrayIndex
		 * \param sampler
		 */
		void BindSampler(unsigned int set, unsigned int binding, unsigned int arrayIndex, const Sampler* sampler);

		/**
		 * \brief 
		 * \param set 
		 * \param binding
		 * \param arrayIndex 
		 * \param textureView 
		 * \param sampler 
		 */
		void BindTextureSampler(unsigned int set, unsigned int binding, unsigned int arrayIndex, const TextureView* textureView, const Sampler* sampler);

		/**
		 * \brief
		 * \param set
		 * \param binding
		 * \param arrayIndex
		 * \param textureView
		 */
		void BindTexture(unsigned int set, unsigned int binding, unsigned int arrayIndex, const TextureView* textureView);

		/**
		 * \brief
		 * \param set
		 * \param binding
		 * \param arrayIndex
		 * \param textureView
		 */
		void BindStorageTexture(unsigned int set, unsigned int binding, unsigned int arrayIndex, const TextureView* textureView);

		/**
		 * \brief 
		 * \param set 
		 * \param binding
		 * \param arrayIndex 
		 * \param uniformBuffer 
		 * \param offset 
		 * \param range 
		 */
		void BindUniformBuffer(unsigned int set, unsigned int binding, unsigned int arrayIndex, const Buffer* uniformBuffer, std::size_t offset, std::size_t range);

		/**
		 * \brief
		 * \param set
		 * \param binding
		 * \param arrayIndex
		 * \param uniformBuffer
		 * \param offset
		 * \param range
		 */
		void BindStorageBuffer(unsigned int set, unsigned int binding, unsigned int arrayIndex, const Buffer* uniformBuffer, std::size_t offset, std::size_t range);

		/**
		 * \brief 
		 * \param shaderType 
		 * \param offset 
		 * \param size 
		 * \param data 
		 */
		void UpdatePipelineConstant(Renderer::ShaderType shaderType, unsigned int offset, unsigned int size, const void* data);

		/**
		 * \brief 
		 * \param set 
		 */
		void ResetBindings(unsigned int set);

		/**
		 * \brief 
		 */
		virtual void ResetBindings();

		/**
		 * \brief 
		 * \return 
		 */
		virtual Ref<Pipeline> CompilePipeline() = 0;

		/**
		 * \brief 
		 * \return 
		 */
		PipelineConstantRange CompilePipelineConstants(Renderer::ShaderType shaderType);

		/**
		 * \brief 
		 * \param setLayout 
		 * \param set 
		 * \param imagesInfo 
		 * \param buffersInfo 
		 * \param writes 
		 * \return 
		 */
		unsigned int CompilePushDescriptors(const Ref<DescriptorSetLayout>& setLayout, unsigned int set, VkDescriptorImageInfo* imagesInfo, VkDescriptorBufferInfo* buffersInfo, VkWriteDescriptorSet* writes);

		/**
		 * \brief 
		 * \param setLayout 
		 * \param descriptorUpdateTemplate 
		 * \param set
		 * \param descriptors 
		 * \return 
		 */
		void CompilePushDescriptorsWithTemplate(const Ref<DescriptorSetLayout>& setLayout, const Ref<DescriptorUpdateTemplate>& descriptorUpdateTemplate, unsigned int set, unsigned char* descriptors);

		/**
		 * \brief 
		 * \param pipelineLayout 
		 * \param set 
		 * \return 
		 */
		DescriptorSetRange CompileDescriptorSets(const Ref<PipelineLayout>& pipelineLayout, unsigned int set);

		/**
		 * \brief
		 * \param flag
		 * \return
		 */
		bool IsDirty(DirtyFlagBits flag) const;

		/**
		 * \brief 
		 * \param set 
		 * \return 
		 */
		bool IsDescriptorSetDirty(unsigned int set) const;

		/**
		 * \brief 
		 * \param shaderType 
		 * \return 
		 */
		bool IsDirtyPipelineConstants(Renderer::ShaderType shaderType) const;

	protected:

		struct PipelineConstantRangeState
		{
			unsigned int Offset = 0;
			unsigned int Size = 0;
			bool Dirty = false;
		};

		Ref<RenderDevice> mRenderDevice;
		Ref<DescriptorSetAllocator> mDescriptorSetAllocator;
		Flags<DirtyFlagBits> mDirtyFlags;
		EnumMap<Renderer::ShaderType, Ref<Shader>> mShaderStages;
		unsigned int mDescriptorSetDirtyFlags;
		std::vector<DescriptorSetStateManager> mDescriptorSetStateManagers;
		unsigned int mPipelineConstantStorageSize;
		std::unique_ptr<unsigned char[]> mPipelineConstantStorage;
		EnumMap<Renderer::ShaderType, PipelineConstantRangeState> mPipelineConstantRanges;
	};
}

#endif // COCKTAIL_VULKAN_PIPELINE_STATE_STATEMANAGER_HPP
