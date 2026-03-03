#ifndef COCKTAIL_SPONZA_IMGUI_IMGUIWINDOWRENDERER_HPP
#define COCKTAIL_SPONZA_IMGUI_IMGUIWINDOWRENDERER_HPP

#include <memory>

#include <Cocktail/Core/Extent2D.hpp>

#include <Cocktail/Renderer/Shader/ShaderProgram.hpp>

#include <Cocktail/Sponza/ImUi/WindowResources.hpp>

namespace Ck::ImUi
{
    class WindowRenderer
    {
    public:

        WindowRenderer(Renderer::RenderDevice* renderDevice);

        void Draw(ImGuiContext* context, WindowResources* windowResources, Renderer::RenderContext* renderContext, Renderer::Framebuffer* framebuffer) const;

    private:

        void SetupRenderState(ImGuiContext* context, WindowResources* resources, Renderer::CommandList* commandList, Extent2D<unsigned int> framebufferSize) const;

        std::shared_ptr<Renderer::ShaderProgram> mShaderProgram;
        Renderer::UniformSlot* mTextureUniformSlot;
    };
}

#endif // COCKTAIL_SPONZA_IMGUI_IMGUIWINDOWRENDERER_HPP
