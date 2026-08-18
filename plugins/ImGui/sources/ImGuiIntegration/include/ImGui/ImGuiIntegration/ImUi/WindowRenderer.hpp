#ifndef IMGUI_IMGUIINTEGRATION_IMGUIWINDOWRENDERER_HPP
#define IMGUI_IMGUIINTEGRATION_IMGUIWINDOWRENDERER_HPP

#include <memory>

#include <CocktailEngine/Core/Extent2D.hpp>

#include <CocktailEngine/Renderer/Shader/ShaderProgram.hpp>

#include <ImGui/ImGuiIntegration/Export.hpp>
#include <ImGui/ImGuiIntegration/ImUi/WindowResources.hpp>

namespace Ck::ImUi
{
    class IMGUI_IMGUIINTEGRATION_API WindowRenderer
    {
    public:

        WindowRenderer(Renderer::RenderDevice* renderDevice);

        void Draw(ImGuiContext* context, WindowResources* windowResources, Renderer::RenderContext* renderContext, Renderer::Framebuffer* framebuffer) const;

    private:

        void SetupRenderState(ImGuiContext* context, WindowResources* resources, Renderer::CommandList* commandList, Extent2D<unsigned int> framebufferSize) const;

        SharedPtr<Renderer::ShaderProgram> mShaderProgram;
        Renderer::UniformSlot* mTextureUniformSlot;
    };
}

#endif // IMGUI_IMGUIINTEGRATION_IMGUIWINDOWRENDERER_HPP
