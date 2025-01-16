#include <Cocktail/Graphic/Scene/Scene.hpp>
#include <Cocktail/Graphic/Scene/Light/PointLight.hpp>

namespace Ck
{
	Ref<PointLight> PointLight::Create(Ref<Scene> scene, LinearColor color, Vector3<float> position, float intensity)
	{
		Transformation transformation(position, Quaternion<float>::Identity(), Vector3<float>::Unit());
		Ref<TransformationNode> transformationNode = scene->CreateTransformationNode(transformation);
		Ref<PointLight> pointLight = New(std::move(transformationNode), color, intensity);
		scene->AddLight(pointLight);

		return pointLight;
	}

	PointLight::PointLight(Ref<TransformationNode> transformationNode, LinearColor color, float intensity) :
		Inherit(transformationNode),
		mColor(color),
		mIntensity(intensity)
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

	void PointLight::SetColor(LinearColor color)
	{
		mColor = color;
	}

	float PointLight::GetIntensity() const
	{
		return mIntensity;
	}

	void PointLight::SetIntensity(float intensity)
	{
		mIntensity = intensity;
	}
}
