#include <Cocktail/Graphic/Rendering/HeadlessSceneViewer.hpp>
#include <Cocktail/Graphic/Rendering/WindowSceneViewer.hpp>
#include <Cocktail/Graphic/Rendering/Engine/GraphicEngine.hpp>
#include <Cocktail/Graphic/Scene/Scene.hpp>

#include <Cocktail/Renderer/RenderDevice.hpp>

namespace Ck
{
	void SceneViewer::Render() const
	{
		std::vector<Renderable*> renderables = mScene->Collect();
		if (renderables.empty())
			return;

		Ref<GraphicEngine> graphicEngine = mScene->GetGraphicEngine();
		Renderer::FrameContext* frameContext = graphicEngine->GetFrameContext();
		Renderer::Framebuffer* framebuffer = AcquireNextFramebuffer(*frameContext);
		if (!framebuffer)
			return;

		RecordDrawContext drawContext(*frameContext);
		
		Renderer::CommandListCreateInfo commandListCreateInfo;
		commandListCreateInfo.Usage = Renderer::CommandListUsage::Graphic;
		Ref<Renderer::CommandList> commandList = frameContext->CreateCommandList(commandListCreateInfo);

		commandList->Begin(nullptr);
		
		for (const ViewportEntry& viewportEntry : mViewports)
		{
			const Ref<Viewport>& viewport = viewportEntry.Viewport;
			viewport->Bind(*commandList, *framebuffer, drawContext, viewportEntry.Index == 0);
		
			for (Renderable* renderable : renderables)
				renderable->AddToQueue(*mRenderQueue);
			
			mRenderQueue->Flush(*commandList, drawContext);
		
			commandList->EndRenderPass();
		}
		commandList->End();
		
		graphicEngine->GetRenderContext()->ExecuteCommandLists(Renderer::CommandQueueType::Graphic, 1, &commandList, nullptr);
	}

	SceneViewer::SceneViewer(Ref<Scene> scene) :
		mScene(std::move(scene))
	{
		mRenderQueue = RenderQueue::New(mScene->GetGraphicEngine()->GetMaterialProgramManager(), Material::ShadingMode::Phong);
	}

	void SceneViewer::AttachViewport(Ref<Viewport> viewport, unsigned index)
	{
		mViewports.insert(mViewports.end(), ViewportEntry{viewport, index});
	}

	void SceneViewer::DetachViewport(const Ref<Viewport>& viewport)
	{
		auto viewportEntryIt = std::find_if(mViewports.begin(), mViewports.end(), [&](const ViewportEntry& entry) {
			return entry.Viewport == viewport;
		});

		assert(viewportEntryIt != mViewports.end());
		mViewports.erase(viewportEntryIt);
	}

	Ref<Scene> SceneViewer::GetScene() const
	{
		return mScene;
	}

	int SceneViewer::ViewportComparator::operator()(const ViewportEntry& lhs, const ViewportEntry& rhs) const noexcept
	{
		return lhs.Index < rhs.Index;
	}
}
