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
				for (const std::shared_ptr<SceneNode>& childSceneNode : sceneNode->GetChildren())
					ProcessSceneNode(childSceneNode.get());
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
					for (const std::shared_ptr<SceneNode>& childSceneNode : sceneNode->GetChildren())
						ProcessSceneNode(childSceneNode.get());
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

	Scene::Scene(std::shared_ptr<GraphicEngine> graphicEngine) :
		mGraphicEngine(std::move(graphicEngine))
	{
		mTransformationGraph = std::make_unique<TransformationGraph>();
		mSceneGraph = std::make_unique<SceneGraph>(this, mTransformationGraph->GetRoot());
	}

	Scene::~Scene()
	{
		mTransformationGraph->Detach();
		mSceneGraph->Detach();
	}

	void Scene::AddCamera(std::unique_ptr<Camera> camera)
	{
		mCameras.push_back(std::move(camera));
		mOnCameraAdded.Emit(mCameras.back().get());
	}

	void Scene::RemoveCamera(const Camera* camera)
	{
		auto it = std::find_if(mCameras.begin(), mCameras.end(), [&](const std::unique_ptr<Camera>& existingCamera) {
			return existingCamera.get() == camera;
		});

		if (it != mCameras.end())
			mCameras.erase(it);
	}

	void Scene::AddLight(std::unique_ptr<Light> light)
	{
		mLights.push_back(std::move(light));
		mOnLightAdded.Emit(mLights.back().get());
	}

	void Scene::RemoveLight(const Light* light)
	{
		auto it = std::find_if(mLights.begin(), mLights.end(), [&](const std::unique_ptr<Light>& existingLight) {
			return existingLight.get() == light;
		});

		if (it != mLights.end())
			mLights.erase(it);
	}

	std::shared_ptr<SceneNode> Scene::CreateSceneNode()
	{
		std::shared_ptr<TransformationNode> transformationNode = CreateTransformationNode();
		std::shared_ptr<SceneNode> sceneNode = mSceneGraph->CreateNode(this, std::move(transformationNode));

		mSceneGraph->GetRoot()->InsertChild(sceneNode);

		mOnSceneNodeAdded.Emit(sceneNode);

		return sceneNode;
	}

	std::shared_ptr<TransformationNode> Scene::CreateTransformationNode(const Transformation& transformation)
	{
		std::shared_ptr<TransformationNode> transformationNode = mTransformationGraph->CreateNode(transformation);
		std::shared_ptr<TransformationNode> rootTransformation = mTransformationGraph->GetRoot();
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

	Signal<std::shared_ptr<SceneNode>>& Scene::OnSceneNodeAdded()
	{
		return mOnSceneNodeAdded;
	}

	std::vector<Renderable*> Scene::CollectRenderables(const Camera& camera) const
	{
		FrustumCullAction action(camera.ComputeFrustum());
		action.ProcessSceneNode(mSceneGraph->GetRoot().get());

		return action.GetRenderables();
	}

	std::vector<Light*> Scene::CollectLights(const Camera& camera) const
	{
		std::vector<Light*> lights;
		Frustum<float> cameraFrustum = camera.ComputeFrustum();
		for (const std::unique_ptr<Light>& light : mLights)
		{
			if (NearlyEqual(light->GetIntensity(), 0.f))
				continue;

			if (light->FrustumCull(cameraFrustum) == Intersection::Outside)
				continue;

			lights.push_back(light.get());
		}

		return lights;
	}

	std::shared_ptr<GraphicEngine> Scene::GetGraphicEngine() const
	{
		return mGraphicEngine;
	}
}
