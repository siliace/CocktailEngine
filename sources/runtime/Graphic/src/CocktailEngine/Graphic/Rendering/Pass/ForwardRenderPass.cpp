#include <CocktailEngine/Graphic/Rendering/Pass/ForwardRenderPass.hpp>
#include <CocktailEngine/Graphic/Scene/SceneView.hpp>

namespace Ck
{
    ForwardRenderPass::ForwardRenderPass(const char* name, SharedPtr<MaterialProgramManager> materialProgramManager, RenderQueue::BlendingMode blendingMode) :
        mName(name),
        mQueue(Move(materialProgramManager), blendingMode)
    {
        /// Nothing
    }

    void ForwardRenderPass::Prepare(SceneView& sceneView, RecordDrawContext& /*drawContext*/)
    {
        sceneView.AddToQueue(mQueue);
    }

    void ForwardRenderPass::Execute(Renderer::CommandList& commandList, RecordDrawContext& drawContext)
    {
        commandList.BeginDebugLabel(mName, LinearColor::White);
        mQueue.Flush(commandList, drawContext);
        commandList.EndDebugLabel();
    }

    const char* ForwardRenderPass::GetName() const
    {
        return mName;
    }

    RenderQueue& ForwardRenderPass::GetQueue()
    {
        return mQueue;
    }
}
