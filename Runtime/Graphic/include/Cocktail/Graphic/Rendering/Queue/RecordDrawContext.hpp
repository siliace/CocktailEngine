#ifndef COCKTAIL_GRAPHIC_RENDERING_QUEUE_RECORDDRAWCONTEXT_HPP
#define COCKTAIL_GRAPHIC_RENDERING_QUEUE_RECORDDRAWCONTEXT_HPP

#include <Cocktail/Core/Utility/CompositeKey.hpp>

#include <Cocktail/Graphic/Geometry/Vertex/VertexLayout.hpp>
#include <Cocktail/Graphic/Material/Shading/MaterialProgramVariant.hpp>

#include <Cocktail/Renderer/Buffer/BufferArea.hpp>
#include <Cocktail/Renderer/Command/CommandList.hpp>
#include <Cocktail/Renderer/Context/FrameContext.hpp>

namespace Ck
{
	class COCKTAIL_GRAPHIC_API RecordDrawContext
	{
	public:

		/**
		 * \brief 
		 */
		enum class RenderingModifierBits
		{
			/**
			 * \brief 
			 */
			Wireframe = Bit(0),
		};

		using RenderingModifiers = Flags<RenderingModifierBits>;

		/**
		 * \brief 
		 * \param frameContext 
		 * \param modifiers 
		 */
		explicit RecordDrawContext(Renderer::FrameContext& frameContext, RenderingModifiers modifiers = {});

		/**
		 * \brief 
		 * \param commandList
		 * \param materialProgramVariant 
		 */
		void BindMaterialProgram(Renderer::CommandList& commandList, const MaterialProgramVariant& materialProgramVariant);

		/**
		 * \brief 
		 * \param commandList 
		 * \param stage 
		 * \param offset 
		 * \param size 
		 * \param data 
		 */
		void BindPipelineConstantsData(Renderer::CommandList& commandList, Renderer::ShaderType stage, unsigned int offset, unsigned int size, const void* data) const;

		/**
		 * \brief 
		 * \param commandList 
		 * \param binding 
		 * \param vertexLayout 
		 * \param vertexCount 
		 * \param data 
		 */
		void BindVertexData(Renderer::CommandList& commandList, unsigned int binding, const VertexLayout& vertexLayout, unsigned int vertexCount, const void* data) const;

		/**
		 * \brief 
		 * \param commandList 
		 * \param indexType 
		 * \param indexCount 
		 * \param data 
		 */
		void BindIndexData(Renderer::CommandList& commandList, Renderer::IndexType indexType, unsigned int indexCount, const void* data) const;

		/**
		 * \brief
		 * \param commandList
		 * \param arrayIndex
		 * \param size
		 * \param data
		 */
		void BindData(Renderer::CommandList& commandList, std::string_view name, Renderer::BufferUsageFlags usage, unsigned int arrayIndex, std::size_t size, const void* data) const;

		/**
		 * \brief
		 * \param commandList
		 * \param arrayIndex
		 * \param size
		 * \param data
		 */
		void BindPersistentData(Renderer::CommandList& commandList, std::string_view name, Renderer::BufferUsageFlags usage, unsigned int arrayIndex, std::size_t size, const void* data);

		/**
		 * \brief 
		 * \param commandList 
		 * \param binding 
		 * \param vertexLayout 
		 */
		void SetVertexInputAttributes(Renderer::CommandList& commandList, unsigned int binding, const VertexLayout& vertexLayout) const;

		/**
		 * \brief
		 * \param stage
		 * \return
		 */
		Renderer::UniformSlot* FindPipelineConstantsSlot(Renderer::ShaderType stage) const;

		/**
		 * \brief
		 * \param name
		 * \return
		 */
		Renderer::UniformSlot* FindUniformSlot(std::string_view name) const;

		/**
		 * \brief 
		 * \return 
		 */
		RenderingModifiers GetModifiers() const;

	private:

		/**
		 * \brief 
		 * \param elementCount 
		 * \return 
		 */
		static PixelFormat::Layout GetVertexInputAttributeFormatLayout(unsigned int elementCount);

		Renderer::FrameContext* mFrameContext;
		RenderingModifiers mModifiers;
		const MaterialProgramVariant* mCurrentMaterialProgram;
		std::unordered_map<CompositeKey<std::string, unsigned int>, Renderer::BufferArea> mPersistentBuffers;
	};
}

#endif // COCKTAIL_GRAPHIC_RENDERING_QUEUE_RECORDDRAWCONTEXT_HPP
