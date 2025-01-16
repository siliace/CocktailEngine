#include <Cocktail/Graphic/Scene/Scene.hpp>
#include <Cocktail/Graphic/Scene/Light/SpotLight.hpp>

namespace Ck
{
	Ref<SpotLight> SpotLight::Create(Ref<Scene> scene, LinearColor color, Vector3<float> direction, Vector3<float> position)
	{
		Transformation transformation(position, Quaternion<float>::Identity(), Vector3<float>::Unit());
		Ref<TransformationNode> transformationNode = scene->CreateTransformationNode(transformation);
		Ref<SpotLight> SpotLight = New(std::move(transformationNode), color, direction);
		scene->AddLight(SpotLight);

		return SpotLight;
	}

	SpotLight::SpotLight(Ref<TransformationNode> transformationNode, LinearColor color, Vector3<float> direction) :
		Inherit(transformationNode),
		mColor(color),
		mDirection(direction)
	{
		/// Nothing
	}

	Intersection SpotLight::FrustumCull(const Frustum<float>& frustum) const
	{
		return Intersection::Inside;
	}

	Light::Type SpotLight::GetType() const
	{
		return Type::Point;
	}

	LinearColor SpotLight::GetColor() const
	{
		return mColor;
	}

	Vector3<float> SpotLight::GetDirection() const
	{
		return mDirection;
	}
}
