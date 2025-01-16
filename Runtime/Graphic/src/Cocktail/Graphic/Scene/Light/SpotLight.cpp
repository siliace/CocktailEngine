#include <Cocktail/Graphic/Scene/Scene.hpp>
#include <Cocktail/Graphic/Scene/Light/SpotLight.hpp>

namespace Ck
{
	Ref<SpotLight> SpotLight::Create(Ref<Scene> scene, LinearColor color, Vector3<float> position, float intensity)
	{
		Transformation transformation(position, Quaternion<float>::Identity(), Vector3<float>::Unit());
		Ref<TransformationNode> transformationNode = scene->CreateTransformationNode(transformation);
		Ref<SpotLight> SpotLight = New(std::move(transformationNode), color, intensity);
		scene->AddLight(SpotLight);

		return SpotLight;
	}

	SpotLight::SpotLight(Ref<TransformationNode> transformationNode, LinearColor color, float intensity) :
		Inherit(transformationNode),
		mColor(color),
		mIntensity(intensity)
	{
		/// Nothing
	}

	Intersection SpotLight::FrustumCull(const Frustum<float>& frustum) const
	{
		return Intersection::Inside;
	}

	Light::Type SpotLight::GetType() const
	{
		return Type::Spot;
	}

	LinearColor SpotLight::GetColor() const
	{
		return mColor;
	}

	void SpotLight::SetColor(LinearColor color)
	{
		mColor = color;
	}

	float SpotLight::GetIntensity() const
	{
		return mIntensity;
	}

	void SpotLight::SetIntensity(float intensity)
	{
		mIntensity = intensity;
	}
}
