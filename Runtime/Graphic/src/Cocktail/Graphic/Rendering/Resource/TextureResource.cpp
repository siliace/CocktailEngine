#include <Cocktail/Graphic/Rendering/Engine/GraphicEngine.hpp>
#include <Cocktail/Graphic/Rendering/Resource/TextureResource.hpp>

#include <Cocktail/Renderer/Texture/TextureViewCreateInfo.hpp>

namespace Ck
{
    namespace
    {
        Renderer::TextureType TextureTypeFromSize(Extent3D<unsigned int> size)
        {
            if (size.Depth > 1)
                return Renderer::TextureType::e3D;

            return size.Height > 1 ? Renderer::TextureType::e2D : Renderer::TextureType::e1D;
        }

        Renderer::TextureViewType TextureViewTypeFromSizeLayer(Extent3D<unsigned int> size, unsigned int layerCount, bool isCube)
        {
            if (isCube)
                return layerCount > 1 ? Renderer::TextureViewType::eCubeArray : Renderer::TextureViewType::eCube;

            if (layerCount > 1)
            {
                assert (size.Depth == 1);
                return size.Height > 1 ? Renderer::TextureViewType::e2DArray : Renderer::TextureViewType::e1DArray;
            }

            if (size.Depth > 1)
                return Renderer::TextureViewType::e3D;

            return size.Height > 1 ? Renderer::TextureViewType::e2D : Renderer::TextureViewType::e1D;
        }
    }

    TextureResource::TextureResource(Ref<GraphicEngine> graphicEngine, Extent3D<unsigned int> size, PixelFormat format, unsigned int layerCount, unsigned int levelCount, bool isCube, std::string_view name) :
        mGraphicEngine(std::move(graphicEngine))
    {
        Renderer::TextureCreateInfo createInfo;
        if (isCube)
            createInfo.Flags |= Renderer::TextureCreateFlagBits::CubemapCompatible;

        createInfo.Type = TextureTypeFromSize(size);
        createInfo.Format = format;
        createInfo.Size = size;
        createInfo.ArrayLayerCount = layerCount;
        createInfo.MipMapsCount = levelCount;
        createInfo.Name = name.data();
        mTexture = mGraphicEngine->GetRenderDevice()->CreateTexture(createInfo);

		Renderer::TextureViewCreateInfo viewCreateInfo;
		viewCreateInfo.Texture = mTexture;
		viewCreateInfo.Type = TextureViewTypeFromSizeLayer(size, layerCount, isCube);
		mView = mGraphicEngine->GetRenderDevice()->CreateTextureView(viewCreateInfo);
    }
    
    void TextureResource::GenerateMipMaps(Renderer::CommandList& commandList) const
    {
        Renderer::TextureSubResource subResource = Renderer::TextureSubResource::AllLayersAllLevels(*mTexture, 0, 1);
        commandList.GenerateMipMaps(mTexture.Get(), Renderer::ResourceState::GraphicShaderResource, subResource);
    }

    Ref<Renderer::TextureView> TextureResource::GetView() const
    {
        return mView;
    }

    Renderer::TextureType TextureResource::GetType() const
    {
        return mTexture->GetType();
    }

    PixelFormat TextureResource::GetFormat() const
    {
        return mTexture->GetFormat();
    }

    Extent3D<unsigned int> TextureResource::GetSize() const
    {
        return mTexture->GetSize();
    }

    unsigned int TextureResource::GetMipMapCount() const
    {
        return mTexture->GetMipMapCount();
    }

    unsigned int TextureResource::GetArrayLayerCount() const
    {
        return mTexture->GetArrayLayerCount();
    }

    Renderer::Texture* TextureResource::GetUnderlyingResource() const
    {
        return mTexture.Get();
    }

    Ref<GraphicEngine> TextureResource::GetGraphicEngine() const
    {
        return mGraphicEngine;
    }
}
