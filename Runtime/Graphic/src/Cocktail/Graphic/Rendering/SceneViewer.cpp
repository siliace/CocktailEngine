#include <Cocktail/Graphic/Rendering/HeadlessSceneViewer.hpp>
#include <Cocktail/Graphic/Rendering/WindowSceneViewer.hpp>
#include <Cocktail/Graphic/Rendering/Engine/GraphicEngine.hpp>
#include <Cocktail/Graphic/Scene/Scene.hpp>
#include <Cocktail/Graphic/Scene/Light/DirectionalLight.hpp>
#include <Cocktail/Graphic/Scene/Light/PointLight.hpp>
#include <Cocktail/Graphic/Scene/Light/SpotLight.hpp>

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
			float AmbientFactor;
		};

		SceneInfo sceneInfo;
		sceneInfo.AmbientFactor = 0.1f;
		drawContext.BindPersistentData(*commandList, "sceneInfo", Renderer::BufferUsageFlagBits::Uniform, 0, sizeof(SceneInfo), &sceneInfo);
		
		for (const ViewportEntry& viewportEntry : mViewports)
		{
			const Ref<Viewport>& viewport = viewportEntry.Viewport;
			viewport->Bind(*commandList, *framebuffer, drawContext, viewportEntry.Index == 0);

			std::vector<Light*> lights = mScene->CollectLights(*viewport->GetCamera());

			struct LightInstance
			{
				LinearColor Color;
				Vector3<float> Position;
				float Intensity;
				Vector3<float> Direction;
				unsigned int Type;
				Vector2<float> CutOff;
				float Constants;
				float Linear;
				float Quadratic;
				float __Padding[3];
			};

			unsigned int lightCount = static_cast<unsigned int>(lights.size());
			LightInstance* lightsInfo = COCKTAIL_STACK_ALLOC(LightInstance, lightCount);
			for (unsigned int i = 0; i < lightCount; i++)
			{
				LightInstance& lightInstance = lightsInfo[i];
				lightInstance.Type = static_cast<unsigned int>(lights[i]->GetType());
				lightInstance.Color = lights[i]->GetColor();
				lightInstance.Intensity = lights[i]->GetIntensity() / 100.f;

				if (lights[i]->GetType() != Light::Type::Directional)
				{
					if (lights[i]->GetType() == Light::Type::Spot)
					{
						SpotLight* spotLight = SpotLight::Cast(lights[i]);
						lightInstance.Position = spotLight->GetWorldTransformation().GetTranslation();
						lightInstance.Direction = -spotLight->GetFront();
						lightInstance.Constants = 1.f;
						lightInstance.Linear = 0.09f;
						lightInstance.Quadratic = 0.032f;
						lightInstance.CutOff.X() = std::cos(Angle<float>::Degree(12.5f));
						lightInstance.CutOff.Y() = std::cos(Angle<float>::Degree(17.5f));
					}
					else if (lights[i]->GetType() == Light::Type::Point)
					{
						PointLight* pointLight = PointLight::Cast(lights[i]);
						lightInstance.Position = pointLight->GetWorldTransformation().GetTranslation();
						lightInstance.Constants = 1.f;
						lightInstance.Linear = 0.09f;
						lightInstance.Quadratic = 0.032f;
					}
				}
				else
				{
					DirectionalLight* directionalLight = DirectionalLight::Cast(lights[i]);
					lightInstance.Direction = directionalLight->GetDirection();
				}
			}

			drawContext.BindPersistentData(*commandList, "lightsInfo", Renderer::BufferUsageFlagBits::Storage, 0, lightCount * sizeof(LightInstance), lightsInfo);

			for (Renderable* renderable : mScene->CollectRenderables())
			{
				Ref<Camera> camera = viewport->GetCamera();
				renderable->AddToQueue(*mOpaqueRenderQueue, *camera);
				renderable->AddToQueue(*mBlendingRenderQueue, *camera);
			}
			
			mOpaqueRenderQueue->Flush(*commandList, drawContext);
			mBlendingRenderQueue->Flush(*commandList, drawContext);
		
			commandList->EndRenderPass();
		}
		commandList->End();
		
		graphicEngine->GetRenderContext()->ExecuteCommandLists(Renderer::CommandQueueType::Graphic, 1, &commandList, nullptr);

		mOnRendered.Emit(*graphicEngine->GetRenderContext(), *frameContext, *framebuffer);
	}

	SceneViewer::SceneViewer(Ref<Scene> scene) :
		mScene(std::move(scene))
	{
		mOpaqueRenderQueue = RenderQueue::New(mScene->GetGraphicEngine()->GetMaterialProgramManager(), Material::ShadingMode::Phong, RenderQueue::BlendingMode::Opaque);
		mBlendingRenderQueue = RenderQueue::New(mScene->GetGraphicEngine()->GetMaterialProgramManager(), Material::ShadingMode::Phong, RenderQueue::BlendingMode::Transparent);
	}

	Ref<Scene> SceneViewer::GetScene() const
	{
		return mScene;
	}

	Signal<Renderer::RenderContext&, Renderer::FrameContext&, Renderer::Framebuffer&>& SceneViewer::OnRendered()
	{
		return mOnRendered;
	}

	int SceneViewer::ViewportComparator::operator()(const ViewportEntry& lhs, const ViewportEntry& rhs) const noexcept
	{
		return lhs.Index < rhs.Index;
	}
}
