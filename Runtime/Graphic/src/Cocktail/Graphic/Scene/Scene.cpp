#include <Cocktail/Graphic/Scene/Scene.hpp>
#include <Cocktail/Graphic/Scene/Camera/PerspectiveCamera.hpp>
#include <Cocktail/Graphic/Scene/Light/Light.hpp>

namespace Ck
{
	Scene::Scene(Ref<GraphicEngine> graphicEngine, Ref<TransformationGraph> transformationGraph) :
		mGraphicEngine(std::move(graphicEngine)),
		mTransformationGraph(std::move(transformationGraph))
	{
		mSceneGraph = SceneGraph::New(this, mTransformationGraph->GetRoot());
	}

	Scene::~Scene()
	{
		mTransformationGraph->Detach();
		mSceneGraph->Detach();
	}

	void Scene::AddCamera(const Ref<Camera>& camera)
	{
		mCameras.emplace_back(camera);

		mOnCameraAdded.Emit(camera);
	}

	void Scene::AddLight(const Ref<Light>& light)
	{
		mLights.emplace_back(light);

		mOnLightAdded.Emit(light);
	}

	Ref<SceneNode> Scene::CreateSceneNode()
	{
		Ref<TransformationNode> transformationNode = CreateTransformationNode();
		Ref<SceneNode> sceneNode = mSceneGraph->CreateNode(this, std::move(transformationNode));

		mSceneGraph->GetRoot()->InsertChild(sceneNode);

		mOnSceneNodeAdded.Emit(sceneNode);

		return sceneNode;
	}

	Ref<TransformationNode> Scene::CreateTransformationNode(const Transformation& transformation)
	{
		Ref<TransformationNode> transformationNode = mTransformationGraph->CreateNode(transformation);
		Ref<TransformationNode> rootTransformation = mTransformationGraph->GetRoot();
		rootTransformation->InsertChild(transformationNode);

		return transformationNode;
	}

	Signal<Ref<Camera>>& Scene::OnCameraAdded()
	{
		return mOnCameraAdded;
	}

	Signal<Ref<Light>>& Scene::OnLightAdded()
	{
		return mOnLightAdded;
	}

	Signal<Ref<SceneNode>>& Scene::OnSceneNodeAdded()
	{
		return mOnSceneNodeAdded;
	}

	std::vector<Renderable*> Scene::CollectRenderables() const
	{
		std::vector<Renderable*> renderables;
		mSceneGraph->Visit([&](SceneNode* node) {
			if (node->IsVisible())
				renderables.push_back(node);
		});

		return renderables;
	}

	std::vector<Light*> Scene::CollectLights(const Camera& camera) const
	{
		std::vector<Light*> lights;
		Frustum<float> cameraFrustum = camera.ComputeFrustum();
		for (const Ref<Light>& light : mLights)
		{
			if (NearlyEqual(light->GetIntensity(), 0.f))
				continue;

			if (light->FrustumCull(cameraFrustum) == Intersection::Outside)
				continue;

			lights.push_back(light.Get());
		}

		return lights;
	}

	Ref<GraphicEngine> Scene::GetGraphicEngine() const
	{
		return mGraphicEngine;
	}
}
