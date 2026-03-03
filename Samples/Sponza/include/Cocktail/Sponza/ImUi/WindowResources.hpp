#ifndef COCKTAIL_SPONZA_IMGUI_IMGUIWINDOWRESOURCES_HPP
#define COCKTAIL_SPONZA_IMGUI_IMGUIWINDOWRESOURCES_HPP

#include <imgui.h>

#include <Cocktail/Renderer/Context/RenderContext.hpp>
#include <Cocktail/Renderer/Texture/Texture.hpp>
#include <Cocktail/Renderer/Texture/TextureView.hpp>

namespace Ck::ImUi
{
    class WindowResources
    {
    public:

        WindowResources(ImGuiContext* context, Renderer::RenderDevice* renderDevice);

        void Initialize(Renderer::RenderContext* renderContext);
        bool NeedInitialization() const;

        Renderer::Texture* GetFontTexture() const;
        Renderer::TextureView* GetFontTextureView() const;

    private:

        bool mNeedInitialization;
        ByteArray mFontTexturePixels;
        std::shared_ptr<Renderer::Texture> mFontTexture;
        std::shared_ptr<Renderer::TextureView> mFontTextureView;
    };
}

#endif // COCKTAIL_SPONZA_IMGUI_IMGUIWINDOWRESOURCES_HPP
