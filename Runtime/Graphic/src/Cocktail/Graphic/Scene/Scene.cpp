#include <Cocktail/Graphic/Scene/Scene.hpp>
#include <Cocktail/Graphic/Scene/Camera/PerspectiveCamera.hpp>
#include <Cocktail/Graphic/Scene/Light/Light.hpp>

namespace Ck
{
	class FrustumCullAction
	{
	public:

		explicit FrustumCullAction(const Frustum<float>& frustum) :
			mFrustum(frustum)
		{
			mRenderables.reserve(1024);
		}

		void ProcessSceneNode(SceneNode* sceneNode)
		{
			if (!sceneNode->IsVisible())
				return;

			const Volume<float>& boundingVolume = sceneNode->GetBoundingVolume();
			if (boundingVolume.IsNull())
			{
				for (const Ref<SceneNode>& childSceneNode : sceneNode->GetChildren())
					ProcessSceneNode(childSceneNode.Get());
			}
			else
			{
				Box<float> obb;
				const Transformation& worldTransformation = sceneNode->GetWorldTransformation();
				for (std::size_t i = 0; i < boundingVolume.GetVertexCount(); i++)
				{
					Vector3<float> vertex = boundingVolume.GetVertex(i);
					Vector3<float> worldVertex = worldTransformation.Apply(vertex);
					obb.Extend(worldVertex);
				}

				if (mFrustum.Intersect(obb) != Intersection::Outside || obb.Intersect(mFrustum) != Intersection::Outside)
				{
					mRenderables.push_back(sceneNode);
					for (const Ref<SceneNode>& childSceneNode : sceneNode->GetChildren())
						ProcessSceneNode(childSceneNode.Get());
				}
			}
		}

		const std::vector<Renderable*>& GetRenderables() const
		{
			return mRenderables;
		}

	private:

		Frustum<float> mFrustum;
		std::vector<Renderable*> mRenderables;
	};

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

	std::vector<Renderable*> Scene::CollectRenderables(const Camera& camera) const
	{
		FrustumCullAction action(camera.ComputeFrustum());
		action.ProcessSceneNode(mSceneGraph->GetRoot().Get());

		return action.GetRenderables();
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
