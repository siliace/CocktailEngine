#include <Cocktail/Graphic/Scene/Scene.hpp>
#include <Cocktail/Graphic/Scene/SceneNode.hpp>

namespace Ck
{
	SceneNode::SceneNode(Scene* scene, std::shared_ptr<TransformationNode> transformationNode) :
		Transformable(std::move(transformationNode)),
		mScene(scene),
		mVisible(true)
	{
		/// Nothing
	}

	void SceneNode::AddShape(std::shared_ptr<Shape> shape)
	{
        shape->ExtendBoundingVolume(mBoundingBox);
		mShapes.Add(std::move(shape));
	}

	void SceneNode::AddToQueue(RenderQueue& queue, const Camera& camera)
	{
		assert(mVisible);

		const Transformation& cameraTransformation = camera.GetWorldTransformation();
		const Transformation& worldTransformation = GetTransformationNode()->GetWorldTransformation();

		for (const std::shared_ptr<Shape>& shape : mShapes)
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
