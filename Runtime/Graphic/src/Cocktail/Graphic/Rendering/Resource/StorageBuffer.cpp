#include <Cocktail/Graphic/Rendering/Engine/GraphicEngine.hpp>
#include <Cocktail/Graphic/Rendering/Resource/StorageBuffer.hpp>

namespace Ck
{
    StorageBuffer::StorageBuffer(const SharedPtr<GraphicEngine>& graphicEngine, std::size_t size, const AnsiChar* name) :
        BufferResource(graphicEngine, Renderer::BufferUsageFlagBits::Storage, size, name)
    {
        /// Nothing
    }

    void StorageBuffer::Upload(std::size_t offset, std::size_t length, const void* data)
    {
        GetGraphicEngine()->UploadBuffer(AsShared(), offset, length, data);
    }
}
