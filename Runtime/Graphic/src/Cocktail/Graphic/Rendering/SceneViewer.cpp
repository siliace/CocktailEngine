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
    void SceneViewer::AttachViewport(UniquePtr<Viewport> viewport)
    {
	    mViewports.Add(std::move(viewport));
    }

    void SceneViewer::Render()
	{
		std::shared_ptr<GraphicEngine> graphicEngine = mScene->GetGraphicEngine();
		Renderer::Framebuffer* framebuffer = AcquireNextFramebuffer(*graphicEngine->GetRenderContext());
		if (!framebuffer)
			return;

		Renderer::CommandListCreateInfo commandListCreateInfo;
		commandListCreateInfo.Usage = Renderer::CommandListUsageBits::Graphic;
		Renderer::CommandList* commandList = graphicEngine->GetRenderContext()->CreateCommandList(commandListCreateInfo);

		commandList->Begin(nullptr);

		struct SceneInfo
		{
			float AmbientFactor;
		};

		SceneInfo sceneInfo;
		sceneInfo.AmbientFactor = 0.1f;
	    mDrawContext.BindData(ShaderBindingDomain::Scene, SceneBindingSlots::SceneInfo, Renderer::BufferUsageFlagBits::Uniform, 0, sizeof(SceneInfo), &sceneInfo);

        bool needClean = true;
		for (const UniquePtr<Viewport>& viewport : mViewports)
		{
		    viewport->Bind(*commandList, *framebuffer, mDrawContext, needClean);
		    needClean = false;

		    SceneView* viewportSceneView = viewport->GetSceneView();

		    viewportSceneView->Update();
			Array<Light*> lights = viewportSceneView->GetLights();

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

			unsigned int lightCount = lights.GetSize();
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
						SpotLight* spotLight = static_cast<SpotLight*>(lights[i]);
						lightInstance.Position = spotLight->GetWorldTransformation().GetTranslation();
						lightInstance.Direction = -spotLight->GetFront();
						lightInstance.Constants = spotLight->GetConstantAttenuationFactor();
						lightInstance.Linear = spotLight->GetLinearAttenuationFactor();
						lightInstance.Quadratic = spotLight->GetQuadraticAttenuationFactor();
						lightInstance.CutOff.X() = std::cos(spotLight->GetInnerCutoff());
						lightInstance.CutOff.Y() = std::cos(spotLight->GetOuterCutoff());
					}
					else if (lights[i]->GetType() == Light::Type::Point)
					{
						PointLight* pointLight = static_cast<PointLight*>(lights[i]);
					    lightInstance.Position = pointLight->GetWorldTransformation().GetTranslation();
					    lightInstance.Constants = pointLight->GetConstantAttenuationFactor();
					    lightInstance.Linear = pointLight->GetLinearAttenuationFactor();
					    lightInstance.Quadratic = pointLight->GetQuadraticAttenuationFactor();
					}
				}
				else
				{
					DirectionalLight* directionalLight = static_cast<DirectionalLight*>(lights[i]);
					lightInstance.Direction = directionalLight->GetDirection();
				}
			}

			mDrawContext.BindData(ShaderBindingDomain::Viewport, ViewportBindingSlots::Lights, Renderer::BufferUsageFlagBits::Storage, 0, lightCount * sizeof(LightInstance), lightsInfo);

		    viewportSceneView->AddToQueue(*mOpaqueRenderQueue);
		    viewportSceneView->AddToQueue(*mBlendingRenderQueue);

			mOpaqueRenderQueue->Flush(*commandList, mDrawContext);
			mBlendingRenderQueue->Flush(*commandList, mDrawContext);
		
			commandList->EndRenderPass();
		}
		commandList->End();

		graphicEngine->GetRenderContext()->SubmitCommandLists(Renderer::CommandQueueType::Graphic, 1, &commandList, nullptr);

		mOnRendered.Emit(*graphicEngine->GetRenderContext(), *framebuffer);
	}

	SceneViewer::SceneViewer(std::shared_ptr<Scene> scene) :
		mScene(std::move(scene)),
		mDrawContext(*mScene->GetGraphicEngine()->GetRenderContext())
	{
		mOpaqueRenderQueue = MakeUnique<RenderQueue>(mScene->GetGraphicEngine()->GetMaterialProgramManager(), RenderQueue::BlendingMode::Opaque);
		mBlendingRenderQueue = MakeUnique<RenderQueue>(mScene->GetGraphicEngine()->GetMaterialProgramManager(), RenderQueue::BlendingMode::Transparent);
	}

	std::shared_ptr<Scene> SceneViewer::GetScene() const
	{
		return mScene;
	}

	Signal<Renderer::RenderContext&, Renderer::Framebuffer&>& SceneViewer::OnRendered()
	{
		return mOnRendered;
	}
}
