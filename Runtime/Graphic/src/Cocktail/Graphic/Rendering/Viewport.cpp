#include <Cocktail/Graphic/Rendering/Queue/RecordDrawContext.hpp>
#include <Cocktail/Graphic/Rendering/Viewport.hpp>

namespace Ck
{
    Viewport::Viewport(UniquePtr<SceneView> sceneView, const Rectangle<float>& area) :
        mSceneView(std::move(sceneView)),
        mArea(area)
    {
        /// Nothing
    }

    void Viewport::Bind(Renderer::CommandList& commandList, Renderer::Framebuffer& framebuffer, RecordDrawContext& drawContext, bool clear)
    {
        Renderer::RenderPassBeginInfo renderPassBeginInfo;
        renderPassBeginInfo.Mode = clear ? Renderer::RenderPassMode::Clear : Renderer::RenderPassMode::Load;
        renderPassBeginInfo.Framebuffer = &framebuffer;
        commandList.BeginRenderPass(renderPassBeginInfo);

        Rectangle<float> clientArea = ComputeClientArea(MakeExtent(framebuffer.GetSize().Width, framebuffer.GetSize().Height));

        Renderer::Viewport viewport;
        viewport.X = clientArea.Position.X();
        viewport.Y = clientArea.Position.Y();
        viewport.Width = clientArea.Extent.Width;
        viewport.Height = clientArea.Extent.Height;
        viewport.MinDepth = 0.f;
        viewport.MaxDepth = 1.f;
        commandList.SetViewport(viewport);

        Renderer::Scissor scissor;
        scissor.Position.Width = clientArea.Position.X();
        scissor.Position.Height = clientArea.Position.Y();
        scissor.Size.Width = clientArea.Extent.Width;
        scissor.Size.Height = clientArea.Extent.Height;
        commandList.SetScissor(scissor);

        mSceneView->Bind(drawContext);
    }

    SceneView* Viewport::GetSceneView() const
    {
        return mSceneView.Get();
    }

    const Rectangle<float>& Viewport::GetArea() const
    {
        return mArea;
    }

    void Viewport::SetArea(const Rectangle<float>& area)
    {
        mArea = area;
    }

    Rectangle<float> Viewport::ComputeClientArea(const Extent2D<unsigned int>& clientSize)
    {
        Rectangle<float> clientArea;
        clientArea.Position.X() = mArea.Position.X() * clientSize.Width;
        clientArea.Position.Y() = mArea.Position.Y() * clientSize.Height;
        clientArea.Extent.Width = mArea.Extent.Width * clientSize.Width;
        clientArea.Extent.Height = mArea.Extent.Height * clientSize.Height;

        return clientArea;
    }
}
