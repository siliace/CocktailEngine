#include <CocktailEngine/Graphic/Rendering/Engine/GraphicEngine.hpp>
#include <CocktailEngine/Graphic/Rendering/HeadlessSceneViewer.hpp>
#include <CocktailEngine/Graphic/Rendering/Pipeline/ForwardRenderPipeline.hpp>
#include <CocktailEngine/Graphic/Scene/Scene.hpp>

#include <CocktailEngine/Renderer/Context/RenderContext.hpp>
#include <CocktailEngine/Renderer/RenderDevice.hpp>

namespace Ck
{
    void SceneViewer::AttachViewport(UniquePtr<Viewport> viewport)
    {
        mViewports.Add(Move(viewport));
    }

    void SceneViewer::Render()
    {
        SharedPtr<GraphicEngine> graphicEngine = mScene->GetGraphicEngine();
        Renderer::Framebuffer* framebuffer = AcquireNextFramebuffer(*graphicEngine->GetRenderContext());
        if (!framebuffer)
            return;

        Renderer::CommandListCreateInfo commandListCreateInfo;
        commandListCreateInfo.Usage = Renderer::CommandListUsageBits::Graphic;
        Renderer::CommandList* commandList = graphicEngine->GetRenderContext()->CreateCommandList(commandListCreateInfo);

        commandList->Begin(nullptr);
        mDrawContext.Reset();

        mRenderPipeline->Render(mViewports, *commandList, *framebuffer, mDrawContext, mRenderSettings);

        commandList->End();

        graphicEngine->GetRenderContext()->SubmitCommandLists(Renderer::CommandQueueType::Graphic, 1, &commandList, nullptr);

        mOnRendered.Emit(*graphicEngine->GetRenderContext(), *framebuffer);
    }

    SceneViewer::SceneViewer(SharedPtr<Scene> scene) :
        mScene(Move(scene)),
        mDrawContext(*mScene->GetGraphicEngine()->GetRenderContext())
    {
        mRenderPipeline = MakeUnique<ForwardRenderPipeline>(mScene->GetGraphicEngine()->GetMaterialProgramManager());
    }

    SharedPtr<Scene> SceneViewer::GetScene() const
    {
        return mScene;
    }

    SceneRenderSettings& SceneViewer::GetRenderSettings()
    {
        return mRenderSettings;
    }

    const SceneRenderSettings& SceneViewer::GetRenderSettings() const
    {
        return mRenderSettings;
    }

    void SceneViewer::SetRenderPipeline(UniquePtr<RenderPipeline> pipeline)
    {
        mRenderPipeline = Move(pipeline);
    }

    RenderPipeline* SceneViewer::GetRenderPipeline() const
    {
        return mRenderPipeline.Get();
    }

    Signal<Renderer::RenderContext&, Renderer::Framebuffer&>& SceneViewer::OnRendered()
    {
        return mOnRendered;
    }
}
