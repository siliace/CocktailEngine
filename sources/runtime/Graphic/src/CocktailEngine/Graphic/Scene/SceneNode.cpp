#include <CocktailEngine/Graphic/Scene/Scene.hpp>
#include <CocktailEngine/Graphic/Scene/SceneNode.hpp>
#include <CocktailEngine/Graphic/Scene/Camera/Camera.hpp>

namespace Ck
{
	SceneNode::SceneNode(Scene* scene, TransformationNode* transformationNode) :
		Transformable(transformationNode),
		mScene(scene),
		mVisible(true)
	{
		/// Nothing
	}

	void SceneNode::AddShape(SharedPtr<Shape> shape)
	{
        shape->ExtendBoundingVolume(mBoundingBox);
		mShapes.Add(Ck::Move(shape));
	}

	void SceneNode::AddToQueue(RenderQueue& queue, const Camera* camera)
	{
		assert(mVisible);

		const Transformation& cameraTransformation = camera->GetWorldTransformation();
		const Transformation& worldTransformation = GetTransformationNode()->GetWorldTransformation();

		for (const SharedPtr<Shape>& shape : mShapes)
            shape->AddToQueue(queue, cameraTransformation, worldTransformation);
	}

	bool SceneNode::IsVisible() const
	{
		return mVisible;
	}

	void SceneNode::SetVisible(bool visible)
	{
		mVisible = visible;
	}

	const Volume<float>& SceneNode::GetBoundingVolume() const
	{
		return mBoundingBox;
	}
}
