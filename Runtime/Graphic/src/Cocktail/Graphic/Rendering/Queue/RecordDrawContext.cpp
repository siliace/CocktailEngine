#include <fmt/core.h>

#include <Cocktail/Graphic/Rendering/Queue/RecordDrawContext.hpp>

#include "Cocktail/Core/Log/Log.hpp"

namespace Ck
{
	RecordDrawContext::RecordDrawContext(Renderer::FrameContext& frameContext, RenderingModifiers modifiers) :
		mFrameContext(&frameContext),
		mModifiers(modifiers)
	{
		/// Nothing
	}

	void RecordDrawContext::BindMaterialProgram(Renderer::CommandList& commandList, const MaterialProgram& materialProgram)
	{
		mCurrentMaterialProgram = &materialProgram;
		commandList.BindShaderProgram(mCurrentMaterialProgram->GetShaderProgram().Get());
	}

	void RecordDrawContext::BindPipelineConstantsData(Renderer::CommandList& commandList, Renderer::ShaderType stage, unsigned int offset, unsigned int size, const void* data) const
	{
		Renderer::UniformSlot* slot = FindPipelineConstantsSlot(stage);
		assert(slot);

		commandList.UpdatePipelineConstant(slot, offset, size, data);
	}

	void RecordDrawContext::BindVertexData(Renderer::CommandList& commandList, unsigned int binding, const VertexLayout& vertexLayout, unsigned vertexCount, const void* data) const
	{
		unsigned int stride = vertexLayout.GetStride();

		std::size_t allocationSize = vertexCount * stride;
		Renderer::BufferArea area = mFrameContext->GetBufferAllocator(Renderer::BufferUsageFlagBits::Vertex, Renderer::MemoryType::Unified)->PushData(allocationSize, data);

		commandList.EnableVertexBinding(binding, true);
		commandList.BindVertexBuffer(binding, area.Buffer, area.BaseOffset, stride, vertexLayout.IsInstanced(), vertexLayout.GetDivisor());
		SetVertexInputAttributes(commandList, binding, vertexLayout);
	}

	void RecordDrawContext::BindIndexData(Renderer::CommandList& commandList, Renderer::IndexType indexType, unsigned int indexCount, const void* data) const
	{
		std::size_t allocationSize = indexCount * Renderer::ToDataType(indexType).GetSize();
		Renderer::BufferArea area = mFrameContext->GetBufferAllocator(Renderer::BufferUsageFlagBits::Index, Renderer::MemoryType::Unified)->PushData(allocationSize, data);

		commandList.BindIndexBuffer(area.Buffer, area.BaseOffset, indexType);
	}

	void RecordDrawContext::BindUniformData(Renderer::CommandList& commandList, std::string_view name, unsigned int arrayIndex, std::size_t size, const void* data) const
	{
		std::size_t allocationSize = size;
		Renderer::BufferArea area = mFrameContext->GetBufferAllocator(Renderer::BufferUsageFlagBits::Vertex, Renderer::MemoryType::Unified)->PushData(allocationSize, data);

		Renderer::UniformSlot* slot = FindUniformSlot(name);
		assert(slot);

		commandList.BindUniformBuffer(slot, arrayIndex, area.Buffer, area.BaseOffset, area.Range);
	}

	void RecordDrawContext::SetVertexInputAttributes(Renderer::CommandList& commandList, unsigned int binding, const VertexLayout& vertexLayout) const
	{
		const std::vector<VertexAttribute>& attributes = vertexLayout.GetAttributes();
		Renderer::VertexInputAttribute* vertexInputAttributes = COCKTAIL_STACK_ALLOC(Renderer::VertexInputAttribute, attributes.size());
		for (std::size_t i = 0; i < attributes.size(); i++)
		{
			const VertexAttribute& attribute = attributes[i];

			PixelFormat::Layout formatLayout = GetVertexInputAttributeFormatLayout(attribute.GetElementCount());
			PixelFormat::Encoding formatEncoding = attribute.IsNormalized() ? PixelFormat::Encoding::Normalized : PixelFormat::Encoding::Raw;

			Renderer::VertexAttributeLocation* attributeLocation = mCurrentMaterialProgram->GetAttributeLocation(attribute.GetSemantic());
			if (!attributeLocation)
			{
				Log::Error("Missing attribute {} in MaterialProgram {}", Enum<VertexAttributeSemantic>::ToString(attribute.GetSemantic()), mCurrentMaterialProgram->GetName());
				continue;
			}

			vertexInputAttributes[i].Location = attributeLocation->GetLocation();
			vertexInputAttributes[i].Format = PixelFormat::Color(formatLayout, attribute.GetType(), formatEncoding);
			vertexInputAttributes[i].Offset = attribute.GetOffset();
		}

		commandList.SetVertexInputAttributes(binding, static_cast<unsigned int>(attributes.size()), vertexInputAttributes);
	}

	Renderer::UniformSlot* RecordDrawContext::FindPipelineConstantsSlot(Renderer::ShaderType stage) const
	{
		assert(mCurrentMaterialProgram);
		Renderer::UniformSlot* slot = nullptr;
		for (unsigned int i = 0; i < mCurrentMaterialProgram->GetShaderProgram()->GetUniformSlotCount(); i++)
		{
			mCurrentMaterialProgram->GetShaderProgram()->GetUniformSlots(&slot, 1, i);
			if (slot->GetDescriptorType() == Renderer::DescriptorType::PipelineConstants && slot->GetShaderStages() & stage)
				return slot;
		}

		return nullptr;
	}

	Renderer::UniformSlot* RecordDrawContext::FindUniformSlot(std::string_view name) const
	{
		assert(mCurrentMaterialProgram);
		Renderer::UniformSlot* slot = nullptr;
		for (unsigned int i = 0; i < mCurrentMaterialProgram->GetShaderProgram()->GetUniformSlotCount(); i++)
		{
			mCurrentMaterialProgram->GetShaderProgram()->GetUniformSlots(&slot, 1, i);
			if (slot->GetName() == name)
				return slot;
		}

		return nullptr;
	}

	RecordDrawContext::RenderingModifiers RecordDrawContext::GetModifiers() const
	{
		return mModifiers;
	}

	PixelFormat::Layout RecordDrawContext::GetVertexInputAttributeFormatLayout(unsigned elementCount)
	{
		switch (elementCount)
		{
		case 1:
			return PixelFormat::Layout::R;

		case 2:
			return PixelFormat::Layout::RG;

		case 3:
			return PixelFormat::Layout::RGB;

		case 4:
			return PixelFormat::Layout::RGBA;
		}

		throw std::runtime_error(fmt::format("Cannot create a vertex input attribute with {} elements", elementCount));
	}
}
