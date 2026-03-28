#include <Cocktail/Graphic/Scene/Scene.hpp>
#include <Cocktail/Graphic/Scene/Camera/PerspectiveCamera.hpp>
#include <Cocktail/Graphic/Scene/Light/Light.hpp>

namespace Ck
{
	Scene::Scene(std::shared_ptr<GraphicEngine> graphicEngine) :
		mGraphicEngine(std::move(graphicEngine))
	{
		mTransformationGraph = MakeUnique<TransformationGraph>();
		mSceneGraph = MakeUnique<SceneGraph>(this, mTransformationGraph->GetRoot());
	}

	Scene::~Scene()
	{
		mTransformationGraph->Detach();
		mSceneGraph->Detach();
	}

	void Scene::AddCamera(UniquePtr<Camera> camera)
	{
		mOnCameraAdded.Emit(
			mCameras.Emplace(std::move(camera)).Get()
		);
	}

	void Scene::RemoveCamera(const Camera* camera)
	{
		mCameras.FilterInPlace([&](const UniquePtr<Camera>& sceneCamera) {
			return sceneCamera.Get() == camera;
		});
	}

	void Scene::AddLight(UniquePtr<Light> light)
	{
		mOnLightAdded.Emit(
			mLights.Emplace(std::move(light)).Get()
		);
	}

	void Scene::RemoveLight(const Light* light)
	{
		mLights.FilterInPlace([&](const UniquePtr<Light>&sceneLight) {
			return sceneLight.Get() == light;
		});
	}

	SceneNode* Scene::CreateSceneNode()
	{
		TransformationNode* transformationNode = CreateTransformationNode();
		SceneNode* sceneNode = mSceneGraph->CreateNode(this, transformationNode);

		mSceneGraph->GetRoot()->InsertChild(sceneNode);

		mOnSceneNodeAdded.Emit(sceneNode);

		return sceneNode;
	}

	TransformationNode* Scene::CreateTransformationNode(const Transformation& transformation)
	{
		TransformationNode* transformationNode = mTransformationGraph->CreateNode(transformation);
		TransformationNode* rootTransformation = mTransformationGraph->GetRoot();
		rootTransformation->InsertChild(transformationNode);

		return transformationNode;
	}

	Signal<Camera*>& Scene::OnCameraAdded()
	{
		return mOnCameraAdded;
	}

	Signal<Light*>& Scene::OnLightAdded()
	{
		return mOnLightAdded;
	}

	Signal<SceneNode*>& Scene::OnSceneNodeAdded()
	{
		return mOnSceneNodeAdded;
	}

    void Scene::PerformAction(SceneAction& action) const
    {
        action.Accept(mSceneGraph->GetRoot());
    }

    const Array<UniquePtr<Camera>>& Scene::GetCameras() const
    {
	    return mCameras;
    }

    const Array<UniquePtr<Light>>& Scene::GetLights() const
    {
	    return mLights;
    }

    std::shared_ptr<GraphicEngine> Scene::GetGraphicEngine() const
	{
		return mGraphicEngine;
	}
}
