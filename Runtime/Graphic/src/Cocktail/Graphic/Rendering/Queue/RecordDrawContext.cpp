#include <Cocktail/Graphic/Rendering/Queue/RecordDrawContext.hpp>

#include <Cocktail/Renderer/Buffer/Buffer.hpp>
#include <Cocktail/Renderer/Buffer/BufferAllocator.hpp>
#include <Cocktail/Renderer/Shader/UniformSlot.hpp>

namespace Ck
{
	RecordDrawContext::RecordDrawContext(Renderer::RenderContext& renderContext, RenderingModifiers modifiers) :
		mRenderContext(&renderContext),
		mModifiers(modifiers),
		mCurrentMaterialProgram(nullptr)
	{
		/// Nothing
	}

	void RecordDrawContext::BindMaterialProgram(Renderer::CommandList& commandList, const MaterialProgramVariant* materialProgramVariant)
	{
	    Renderer::ShaderProgram* shaderProgram = materialProgramVariant->GetShaderProgram();
	    if (mCurrentMaterialProgram != materialProgramVariant)
	    {
	        mCurrentMaterialProgram = materialProgramVariant;
	    }

	    /// TODO: move this to the if when moving the command list in the context
	    commandList.BindShaderProgram(shaderProgram);
	}

    void RecordDrawContext::BindData(ShaderBindingDomain domain, BindingSlot bindingSlot, Renderer::BufferUsageFlags usage, unsigned int arrayIndex, std::size_t size, const void* data)
	{
        Renderer::BufferAllocator* allocator = mRenderContext->GetBufferAllocator(usage, Renderer::MemoryType::Unified);
	    Renderer::BufferArea area = allocator->PushData(size, data);

	    BindBuffer(domain, bindingSlot, area.Buffer, arrayIndex, area.BaseOffset, area.Range);
	}

    void RecordDrawContext::BindBuffer(ShaderBindingDomain domain, BindingSlot bindingSlot, const Renderer::Buffer* buffer, unsigned int arrayIndex)
    {
	    BindBuffer(domain, bindingSlot, buffer, arrayIndex, 0, buffer->GetSize());
    }

    void RecordDrawContext::BindBuffer(ShaderBindingDomain domain, BindingSlot bindingSlot, const Renderer::Buffer* buffer, unsigned int arrayIndex, std::size_t offset, std::size_t range)
    {
	    assert(bindingSlot != InvalidBindingSlot);
	    assert(buffer != nullptr);
	    assert(offset + range < buffer->GetSize());

	    mBindingTable.BindBuffer(domain, bindingSlot, buffer, arrayIndex, offset, range);
    }

    void RecordDrawContext::BindSampler(ShaderBindingDomain domain, BindingSlot bindingSlot, const Renderer::Sampler* sampler, unsigned int arrayIndex)
    {
	    mBindingTable.BindSampler(domain, bindingSlot, sampler, arrayIndex);
    }

    void RecordDrawContext::BindTexture(ShaderBindingDomain domain, BindingSlot bindingSlot, const Renderer::TextureView* texture, unsigned int arrayIndex)
    {
	    mBindingTable.BindTexture(domain, bindingSlot, texture, arrayIndex);
    }

    void RecordDrawContext::BindTextureSampler(ShaderBindingDomain domain, BindingSlot bindingSlot, const Renderer::TextureView* texture, const Renderer::Sampler* sampler, unsigned int arrayIndex)
    {
	    mBindingTable.BindTextureSampler(domain, bindingSlot, texture, sampler, arrayIndex);
    }

    void RecordDrawContext::BindVertexData(Renderer::CommandList& commandList, unsigned int binding, const VertexLayout* vertexLayout, unsigned vertexCount, const void* data) const
	{
		unsigned int stride = vertexLayout->GetStride();
		std::size_t allocationSize = vertexCount * stride;
		Renderer::BufferArea area = mRenderContext->GetBufferAllocator(Renderer::BufferUsageFlagBits::Vertex, Renderer::MemoryType::Unified)->PushData(allocationSize, data);
        BindVertexBuffer(commandList, binding, vertexLayout, area.Buffer, area.BaseOffset);
	}

    void RecordDrawContext::BindVertexBuffer(Renderer::CommandList& commandList, unsigned int binding, const VertexLayout* vertexLayout, const Renderer::Buffer* buffer, std::size_t offset) const
    {
	    assert(buffer->GetUsage() & Renderer::BufferUsageFlagBits::Vertex);

		unsigned int stride = vertexLayout->GetStride();
	    Array<Renderer::VertexInputAttribute, LinearAllocator<32>> vertexInputAttributes;

	    for (const VertexAttribute& attribute : vertexLayout->GetAttributes())
	    {
	        PixelFormat::Layout formatLayout = GetVertexInputAttributeFormatLayout(attribute.GetElementCount());
	        PixelFormat::Encoding formatEncoding = attribute.IsNormalized() ? PixelFormat::Encoding::Normalized : PixelFormat::Encoding::Raw;

	        Renderer::VertexAttributeLocation* attributeLocation = mCurrentMaterialProgram->GetVertexAttributeLocation(attribute.GetSemantic());
	        if (!attributeLocation)
	            continue;

	        Renderer::VertexInputAttribute& inputAttribute = vertexInputAttributes.Emplace();
	        inputAttribute.Location = attributeLocation->GetLocation();
	        inputAttribute.Format = PixelFormat::Color(formatLayout, attribute.GetType(), formatEncoding);
	        inputAttribute.Offset = attribute.GetOffset();
	    }

	    commandList.EnableVertexBinding(binding, true);
	    commandList.SetVertexInputAttributes(binding, vertexInputAttributes.GetSize(), vertexInputAttributes.GetData());
	    commandList.BindVertexBuffer(binding, buffer, offset, stride, vertexLayout->IsInstanced(), vertexLayout->GetDivisor());
    }

    void RecordDrawContext::BindIndexData(Renderer::CommandList& commandList, Renderer::IndexType indexType, unsigned int indexCount, const void* data) const
	{
		std::size_t allocationSize = indexCount * Renderer::ToDataType(indexType).GetSize();
		Renderer::BufferArea area = mRenderContext->GetBufferAllocator(Renderer::BufferUsageFlagBits::Index, Renderer::MemoryType::Unified)->PushData(allocationSize, data);

		BindIndexBuffer(commandList, area.Buffer, indexType, area.BaseOffset);
	}

    void RecordDrawContext::BindIndexBuffer(Renderer::CommandList& commandList, const Renderer::Buffer* buffer, Renderer::IndexType indexType, std::size_t offset) const
    {
	    assert(buffer->GetUsage() & Renderer::BufferUsageFlagBits::Index);

	    commandList.BindIndexBuffer(buffer, offset, indexType);
    }

    void RecordDrawContext::Draw(Renderer::CommandList& commandList, unsigned int vertexCount, unsigned int instanceCount, unsigned int firstVertex, unsigned int firstInstance) const
    {
	    for (const auto& [key, entry] : mBindingTable.GetEntries())
	    {
	        ShaderBindingDomain domain = std::get<0>(key);
	        unsigned int index = std::get<1>(key);

	        const Renderer::UniformSlot* slot = mCurrentMaterialProgram->GetSlot(domain, index);
	        if (slot != nullptr)
	            entry->Bind(commandList, slot);
	    }

	    commandList.Draw(vertexCount, instanceCount, firstVertex, firstInstance);
    }

    void RecordDrawContext::DrawIndexed(Renderer::CommandList& commandList, unsigned int indexCount, unsigned int instanceCount, unsigned int firstIndex, int indexOffset, unsigned int firstInstance) const
	{
	    for (const auto& [key, entry] : mBindingTable.GetEntries())
	    {
	        ShaderBindingDomain domain = std::get<0>(key);
	        unsigned int index = std::get<1>(key);

	        const Renderer::UniformSlot* slot = mCurrentMaterialProgram->GetSlot(domain, index);
	        if (slot != nullptr)
	            entry->Bind(commandList, slot);
	    }

	    commandList.DrawIndexed(indexCount, instanceCount, firstIndex, indexOffset, firstInstance);
    }

    void RecordDrawContext::EnableVertexBindings(Renderer::CommandList& commandList, unsigned int firstBinding, unsigned int bindingsCount, bool enable)
    {
	    for (unsigned int i = firstBinding; i < bindingsCount; ++i)
	        commandList.EnableVertexBinding(i, enable);
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
