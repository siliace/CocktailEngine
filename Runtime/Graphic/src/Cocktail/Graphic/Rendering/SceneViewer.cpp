#include <Cocktail/Graphic/Rendering/HeadlessSceneViewer.hpp>
#include <Cocktail/Graphic/Rendering/WindowSceneViewer.hpp>
#include <Cocktail/Graphic/Rendering/Engine/GraphicEngine.hpp>
#include <Cocktail/Graphic/Scene/Scene.hpp>

#include <Cocktail/Renderer/RenderDevice.hpp>
#include <Cocktail/Renderer/Context/RenderContext.hpp>

namespace Ck
{
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

	void SceneViewer::Render()
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

		struct SceneInfo
		{
			alignas(16) float AmbientFactor;
			alignas(16) LinearColor DirectionalLightColor;
			alignas(16) Vector3<float> DirectionalLightDirection;
		};

		SceneInfo sceneInfo;
		sceneInfo.AmbientFactor = 0.1f;
		sceneInfo.DirectionalLightColor = LinearColor(1.f, 1.f, 1.f);
		sceneInfo.DirectionalLightDirection = Vector3<float>::Normalize(Vector3<float>(-1.f, -1.f, 0.f));
		drawContext.BindPersistentUniformData(*commandList, "sceneInfo", 0, sizeof(SceneInfo), &sceneInfo);
		
		for (const ViewportEntry& viewportEntry : mViewports)
		{
			const Ref<Viewport>& viewport = viewportEntry.Viewport;
			viewport->Bind(*commandList, *framebuffer, drawContext, viewportEntry.Index == 0);
		
			for (Renderable* renderable : renderables)
			{
				Ref<Camera> camera = viewport->GetCamera();
				renderable->AddToQueue(*mRenderQueue, *camera);
			}
			
			mRenderQueue->Flush(*commandList, drawContext);
		
			commandList->EndRenderPass();
		}
		commandList->End();

		mOnRendered.Emit(*frameContext, *framebuffer);
		
		graphicEngine->GetRenderContext()->ExecuteCommandLists(Renderer::CommandQueueType::Graphic, 1, &commandList, nullptr);
	}

	SceneViewer::SceneViewer(Ref<Scene> scene) :
		mScene(std::move(scene))
	{
		mRenderQueue = RenderQueue::New(mScene->GetGraphicEngine()->GetMaterialProgramManager(), Material::ShadingMode::Phong);
	}

	Ref<Scene> SceneViewer::GetScene() const
	{
		return mScene;
	}

	Signal<Renderer::FrameContext&, Renderer::Framebuffer&>& SceneViewer::OnRendered()
	{
		return mOnRendered;
	}

	int SceneViewer::ViewportComparator::operator()(const ViewportEntry& lhs, const ViewportEntry& rhs) const noexcept
	{
		return lhs.Index < rhs.Index;
	}
}
