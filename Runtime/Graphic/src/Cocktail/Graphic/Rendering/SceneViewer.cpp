#include <Cocktail/Graphic/Rendering/HeadlessSceneViewer.hpp>
#include <Cocktail/Graphic/Rendering/WindowSceneViewer.hpp>
#include <Cocktail/Graphic/Rendering/Engine/GraphicEngine.hpp>
#include <Cocktail/Graphic/Scene/Scene.hpp>
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
			alignas(16) float AmbientFactor;
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
				unsigned int Type;
				unsigned int __Padding_1[3];
				LinearColor Color;
				Vector3<float> Position;
				float __Padding_2;
				Vector3<float> Direction;
				float __Padding_3;
				Vector2<float> CutOff;
				Vector2<float> __Padding_4;
				float Constants;
				float Linear;
				float Quadratic;
			};

			unsigned int lightCount = static_cast<unsigned int>(lights.size());

			std::size_t lightInfoSize = sizeof(LightInstance) * lightCount + 4 * sizeof(unsigned int);
			unsigned char* lightsInfo = COCKTAIL_STACK_ALLOC(unsigned char, lightInfoSize);

			std::size_t offset = 0;
			std::memcpy(&lightsInfo[offset], &lightCount, sizeof(unsigned int));
			offset += 4 * sizeof(unsigned int);

			for (unsigned int i = 0; i < lightCount; i++)
			{
				LightInstance* lightInstance = reinterpret_cast<LightInstance*>(&lightsInfo[offset]);
				lightInstance->Type = static_cast<unsigned int>(lights[i]->GetType());
				lightInstance->Color = lights[i]->GetColor();
				lightInstance->Direction = lights[i]->GetDirection();

				if (lights[i]->GetType() != Light::Type::Directional)
				{
					if (lights[i]->GetType() == Light::Type::Spot)
					{
						SpotLight* spotLight = SpotLight::Cast(lights[i]);

						lightInstance->Position = spotLight->GetPosition();
						lightInstance->Constants = 1.f;
						lightInstance->Linear = 0.09f;
						lightInstance->Quadratic = 0.032f;
						lightInstance->CutOff.X() = std::cos(Angle<float>::Degree(12.5f));
						lightInstance->CutOff.Y() = std::cos(Angle<float>::Degree(17.5f));
					}
					else if (lights[i]->GetType() == Light::Type::Point)
					{
						PointLight* pointLight = PointLight::Cast(lights[i]);

						lightInstance->Position = pointLight->GetPosition();
						lightInstance->Constants = 1.f;
						lightInstance->Linear = 0.09f;
						lightInstance->Quadratic = 0.032f;
					}
				}
				else
				{
					lightInstance->Position = Vector3<float>::Zero();
					lightInstance->CutOff = Vector2<float>::Zero();
					lightInstance->Constants = 0.f;
					lightInstance->Linear = 0.f;
					lightInstance->Quadratic = 0.f;
				}

				offset += sizeof(LightInstance);
			}

			drawContext.BindPersistentData(*commandList, "lightsInfo", Renderer::BufferUsageFlagBits::Storage, 0, lightInfoSize, lightsInfo);

			for (Renderable* renderable : mScene->CollectRenderables())
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
