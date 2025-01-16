#include <Cocktail/Graphic/Scene/Scene.hpp>
#include <Cocktail/Graphic/Camera/PerspectiveCamera.hpp>

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

	Ref<SceneNode> Scene::CreateSceneNode()
	{
		Ref<TransformationNode> transformationNode = CreateTransformationNode();
		Ref<SceneNode> sceneNode = mSceneGraph->CreateNode(this, std::move(transformationNode));

		mSceneGraph->GetRoot()->InsertChild(sceneNode);

		mOnSceneNodeAdded.Emit(sceneNode);

		return sceneNode;
	}

	void Scene::AddCamera(const Ref<Camera>& camera)
	{
		mCameras.emplace_back(camera);

		mOnCameraAdded.Emit(camera);
	}

	Signal<Ref<Camera>>& Scene::OnCameraAdded()
	{
		return mOnCameraAdded;
	}

	Ref<TransformationNode> Scene::CreateTransformationNode(const Transformation& transformation)
	{
		Ref<TransformationNode> transformationNode = mTransformationGraph->CreateNode(transformation);
		Ref<TransformationNode> rootTransformation = mTransformationGraph->GetRoot();
		rootTransformation->InsertChild(transformationNode);

		return transformationNode;
	}

	Signal<Ref<SceneNode>>& Scene::OnSceneNodeAdded()
	{
		return mOnSceneNodeAdded;
	}

	std::vector<Renderable*> Scene::Collect() const
	{
		std::vector<Renderable*> renderables;
		mSceneGraph->Visit([&](const Ref<SceneNode>& node) {
			renderables.push_back(node.Get());
		});

		return renderables;
	}

	Ref<GraphicEngine> Scene::GetGraphicEngine() const
	{
		return mGraphicEngine;
	}
}
