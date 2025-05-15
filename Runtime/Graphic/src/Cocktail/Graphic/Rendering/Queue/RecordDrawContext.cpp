#include <Cocktail/Graphic/Rendering/Queue/RecordDrawContext.hpp>

#include <Cocktail/Renderer/Buffer/BufferAllocator.hpp>
#include <Cocktail/Renderer/Shader/UniformSlot.hpp>

namespace Ck
{
	RecordDrawContext::RecordDrawContext(Renderer::FrameContext& frameContext, RenderingModifiers modifiers) :
		mFrameContext(&frameContext),
		mModifiers(modifiers),
		mCurrentMaterialProgram(nullptr)
	{
		/// Nothing
	}

	void RecordDrawContext::BindMaterialProgram(Renderer::CommandList& commandList, const MaterialProgramVariant& materialProgramVariant)
	{
		mCurrentMaterialProgram = &materialProgramVariant;
		commandList.BindShaderProgram(mCurrentMaterialProgram->GetShaderProgram().Get());
		for (const auto& [key, area] : mPersistentBuffers)
		{
			std::string name = std::get<0>(key);
			unsigned int arrayIndex = std::get<1>(key);

			Renderer::UniformSlot* slot = mCurrentMaterialProgram->GetShaderProgram()->FindUniformSlot(name);
			if (!slot)
				continue;

			commandList.BindBuffer(slot, arrayIndex, area.Buffer, area.BaseOffset, area.Range);
		}
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

	void RecordDrawContext::BindData(Renderer::CommandList& commandList, std::string_view name, Renderer::BufferUsageFlags usage, unsigned int arrayIndex, std::size_t size, const void* data) const
	{
		std::size_t allocationSize = size;
		Renderer::BufferArea area = mFrameContext->GetBufferAllocator(usage, Renderer::MemoryType::Unified)->PushData(allocationSize, data);

		Renderer::UniformSlot* slot = mCurrentMaterialProgram->GetShaderProgram()->FindUniformSlot(name);
		assert(slot);

		commandList.BindBuffer(slot, arrayIndex, area.Buffer, area.BaseOffset, area.Range);
	}

	void RecordDrawContext::BindPersistentData(Renderer::CommandList& commandList, std::string_view name, Renderer::BufferUsageFlags usage, unsigned int arrayIndex, std::size_t size, const void* data)
	{
		std::size_t allocationSize = size;
		Renderer::BufferArea area = mFrameContext->GetBufferAllocator(usage, Renderer::MemoryType::Unified)->PushData(allocationSize, data);

		std::string n(name.data(), name.length());
		mPersistentBuffers[CompositeKey<std::string, unsigned int>(n, arrayIndex)] = area;

		if (mCurrentMaterialProgram)
		{
			Renderer::UniformSlot* slot = mCurrentMaterialProgram->GetShaderProgram()->FindUniformSlot(name);
			if (slot)
				commandList.BindBuffer(slot, arrayIndex, area.Buffer, area.BaseOffset, area.Range);
		}
	}

	void RecordDrawContext::SetVertexInputAttributes(Renderer::CommandList& commandList, unsigned int binding, const VertexLayout& vertexLayout) const
	{
		commandList.EnableVertexBinding(binding, true);
		const std::vector<VertexAttribute>& attributes = vertexLayout.GetAttributes();
		Renderer::VertexInputAttribute* vertexInputAttributes = COCKTAIL_STACK_ALLOC(Renderer::VertexInputAttribute, attributes.size());

		unsigned int attributeCount = 0;
		for (const VertexAttribute& attribute : attributes)
		{
			PixelFormat::Layout formatLayout = GetVertexInputAttributeFormatLayout(attribute.GetElementCount());
			PixelFormat::Encoding formatEncoding = attribute.IsNormalized() ? PixelFormat::Encoding::Normalized : PixelFormat::Encoding::Raw;

			Renderer::VertexAttributeLocation* attributeLocation = mCurrentMaterialProgram->GetVertexAttributeLocation(attribute.GetSemantic());
			if (!attributeLocation)
				continue;

			vertexInputAttributes[attributeCount].Location = attributeLocation->GetLocation();
			vertexInputAttributes[attributeCount].Format = PixelFormat::Color(formatLayout, attribute.GetType(), formatEncoding);
			vertexInputAttributes[attributeCount].Offset = attribute.GetOffset();
			++attributeCount;
		}

		commandList.SetVertexInputAttributes(binding, attributeCount, vertexInputAttributes);
	}

	RecordDrawContext::RenderingModifiers RecordDrawContext::GetModifiers() const
	{
		return mModifiers;
	}

	PixelFormat::Layout RecordDrawContext::GetVertexInputAttributeFormatLayout(unsigned int elementCount)
	{
		assert(elementCount > 0 && elementCount < 5);

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

		COCKTAIL_UNREACHABLE();
	}
}
