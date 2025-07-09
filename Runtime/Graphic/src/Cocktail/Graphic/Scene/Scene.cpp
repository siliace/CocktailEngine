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
			mRenderables.Reserve(1024);
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
					mRenderables.Add(sceneNode);
					for (const std::shared_ptr<SceneNode>& childSceneNode : sceneNode->GetChildren())
						ProcessSceneNode(childSceneNode.get());
				}
			}
		}

		const Array<Renderable*>& GetRenderables() const
		{
			return mRenderables;
		}

	private:

		Frustum<float> mFrustum;
		Array<Renderable*> mRenderables;
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
		mOnCameraAdded.Emit(
			mCameras.Emplace(std::move(camera)).get()
		);
	}

	void Scene::RemoveCamera(const Camera* camera)
	{
		mCameras.FilterInPlace([&](const std::unique_ptr<Camera>& sceneCamera) {
			return sceneCamera.get() == camera;
		});
	}

	void Scene::AddLight(std::unique_ptr<Light> light)
	{
		mOnLightAdded.Emit(
			mLights.Emplace(std::move(light)).get()
		);
	}

	void Scene::RemoveLight(const Light* light)
	{
		mLights.FilterInPlace([&](const std::unique_ptr<Light>&sceneLight) {
			return sceneLight.get() == light;
		});
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

	Array<Renderable*> Scene::CollectRenderables(const Camera& camera) const
	{
		FrustumCullAction action(camera.ComputeFrustum());
		action.ProcessSceneNode(mSceneGraph->GetRoot().get());

		return action.GetRenderables();
	}

	Array<Light*> Scene::CollectLights(const Camera& camera) const
	{
		Array<Light*> lights;
		lights.Reserve(mLights.GetSize());

		Frustum<float> cameraFrustum = camera.ComputeFrustum();

		for (unsigned int i = 0; i < mLights.GetSize(); i++)
		{
			Light* light = mLights[i].get();

			if (NearlyEqual(light->GetIntensity(), 0.f))
				continue;

			if (light->FrustumCull(cameraFrustum) == Intersection::Outside)
				continue;

			lights.Add(light);
		}

		return lights;
	}

	std::shared_ptr<GraphicEngine> Scene::GetGraphicEngine() const
	{
		return mGraphicEngine;
	}
}
