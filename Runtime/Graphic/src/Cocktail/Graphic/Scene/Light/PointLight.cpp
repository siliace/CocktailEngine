#include <Cocktail/Graphic/Scene/Scene.hpp>
#include <Cocktail/Graphic/Scene/Light/PointLight.hpp>

namespace Ck
{
	Ref<PointLight> PointLight::Create(Ref<Scene> scene, LinearColor color, Vector3<float> direction, Vector3<float> position)
	{
		Transformation transformation(position, Quaternion<float>::Identity(), Vector3<float>::Unit());
		Ref<TransformationNode> transformationNode = scene->CreateTransformationNode(transformation);
		Ref<PointLight> pointLight = New(std::move(transformationNode), color, direction);
		scene->AddLight(pointLight);

		return pointLight;
	}

	PointLight::PointLight(Ref<TransformationNode> transformationNode, LinearColor color, Vector3<float> direction) :
		Inherit(transformationNode),
		mColor(color),
		mDirection(direction)
	{
		/// Nothing
	}

	Intersection PointLight::FrustumCull(const Frustum<float>& frustum) const
	{
		return Intersection::Inside;
	}

	Light::Type PointLight::GetType() const
	{
		return Type::Point;
	}

	LinearColor PointLight::GetColor() const
	{
		return mColor;
	}

	Vector3<float> PointLight::GetDirection() const
	{
		return mDirection;
	}
}
