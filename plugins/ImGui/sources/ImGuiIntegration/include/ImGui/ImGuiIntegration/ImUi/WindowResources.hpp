#ifndef IMGUI_IMGUIINTEGRATION_IMGUIWINDOWRESOURCES_HPP
#define IMGUI_IMGUIINTEGRATION_IMGUIWINDOWRESOURCES_HPP

#include <imgui.h>

#include <CocktailEngine/Renderer/Context/RenderContext.hpp>
#include <CocktailEngine/Renderer/Texture/Texture.hpp>
#include <CocktailEngine/Renderer/Texture/TextureView.hpp>

#include <ImGui/ImGuiIntegration/Export.hpp>

namespace Ck::ImUi
{
    class IMGUI_IMGUIINTEGRATION_API WindowResources
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
        SharedPtr<Renderer::Texture> mFontTexture;
        SharedPtr<Renderer::TextureView> mFontTextureView;
    };
}

#endif // IMGUI_IMGUIINTEGRATION_IMGUIWINDOWRESOURCES_HPP
