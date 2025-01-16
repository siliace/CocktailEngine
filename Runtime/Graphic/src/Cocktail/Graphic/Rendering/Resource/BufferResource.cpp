#include <Cocktail/Graphic/Rendering/Engine/GraphicEngine.hpp>
#include <Cocktail/Graphic/Rendering/Resource/BufferResource.hpp>

#include <Cocktail/Renderer/Buffer/BufferCreateInfo.hpp>

namespace Ck
{
	BufferResource::BufferResource(Ref<GraphicEngine> graphicEngine, Renderer::BufferUsageFlags usage, std::size_t size, std::string_view name) :
		mGraphicEngine(std::move(graphicEngine))
	{
		Renderer::BufferCreateInfo createInfo;
		createInfo.Usage = usage;
		createInfo.Size = size;
		createInfo.Name = name.data();
		mBuffer = mGraphicEngine->GetRenderDevice()->CreateBuffer(createInfo);
	}

	Renderer::Buffer* BufferResource::GetUnderlyingResource() const
	{
		return mBuffer.Get();
	}

	Ref<GraphicEngine> BufferResource::GetGraphicEngine() const
	{
		return mGraphicEngine;
	}
}
