#include <Cocktail/Graphic/Scene/Scene.hpp>
#include <Cocktail/Graphic/Scene/Light/SpotLight.hpp>

namespace Ck
{
	SpotLight* SpotLight::Create(std::shared_ptr<Scene> scene, LinearColor color, Vector3<float> position, float intensity)
	{
		Transformation transformation(position, Quaternion<float>::Identity(), Vector3<float>::Unit());
		std::shared_ptr<TransformationNode> transformationNode = scene->CreateTransformationNode(transformation);

		std::unique_ptr<SpotLight> spotLight = std::make_unique<SpotLight>(std::move(transformationNode), color, intensity);
		SpotLight* spotLightPtr = spotLight.get();
		scene->AddLight(std::move(spotLight));

		return spotLightPtr;
	}

	SpotLight::SpotLight(std::shared_ptr<TransformationNode> transformationNode, LinearColor color, float intensity) :
		Transformable(transformationNode),
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
