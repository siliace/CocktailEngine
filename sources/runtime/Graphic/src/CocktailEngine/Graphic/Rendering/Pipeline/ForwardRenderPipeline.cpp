#include <CocktailEngine/Graphic/Rendering/Lighting/DefaultLightCollector.hpp>
#include <CocktailEngine/Graphic/Rendering/Pass/ForwardRenderPass.hpp>
#include <CocktailEngine/Graphic/Rendering/Pipeline/ForwardRenderPipeline.hpp>
#include <CocktailEngine/Graphic/Rendering/Queue/RecordDrawContext.hpp>
#include <CocktailEngine/Graphic/Rendering/Viewport.hpp>
#include <CocktailEngine/Graphic/Scene/SceneView.hpp>

namespace Ck
{
    ForwardRenderPipeline::ForwardRenderPipeline(SharedPtr<MaterialProgramManager> materialProgramManager) :
        mLightCollector(MakeUnique<DefaultLightCollector>())
    {
        mPasses.Add(MakeUnique<ForwardRenderPass>("Opaque", materialProgramManager, RenderQueue::BlendingMode::Opaque));
        mPasses.Add(MakeUnique<ForwardRenderPass>("Transparent", materialProgramManager, RenderQueue::BlendingMode::Transparent));
    }

    void ForwardRenderPipeline::AddPass(UniquePtr<RenderPass> pass)
    {
        mPasses.Add(Move(pass));
    }

    void ForwardRenderPipeline::SetLightCollector(UniquePtr<LightCollector> lightCollector)
    {
        mLightCollector = Move(lightCollector);
    }

    void ForwardRenderPipeline::Render(const Array<UniquePtr<Viewport>>& viewports, Renderer::CommandList& commandList, Renderer::Framebuffer& framebuffer,
                                       RecordDrawContext& drawContext, const SceneRenderSettings& settings)
    {
        struct SceneInfo
        {
            float AmbientFactor;
        };

        SceneInfo sceneInfo;
        sceneInfo.AmbientFactor = settings.AmbientFactor;
        drawContext.BindData(ShaderBindingDomain::Scene, SceneBindingSlots::SceneInfo, Renderer::BufferUsageFlagBits::Uniform, 0, sizeof(SceneInfo), &sceneInfo);

        bool needClean = true;
        for (const UniquePtr<Viewport>& viewport : viewports)
        {
            commandList.BeginDebugLabel("Viewport", LinearColor::Red);
            viewport->Bind(commandList, framebuffer, drawContext, needClean);
            needClean = false;

            SceneView* sceneView = viewport->GetSceneView();
            sceneView->Update();

            if (mLightCollector)
                mLightCollector->Collect(*sceneView, drawContext);

            for (const UniquePtr<RenderPass>& pass : mPasses)
                pass->Prepare(*sceneView, drawContext);

            for (const UniquePtr<RenderPass>& pass : mPasses)
                pass->Execute(commandList, drawContext);

            commandList.EndRenderPass();
            commandList.EndDebugLabel();
        }
    }
}
