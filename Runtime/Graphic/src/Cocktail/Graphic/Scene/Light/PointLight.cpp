#include <Cocktail/Graphic/Scene/Scene.hpp>
#include <Cocktail/Graphic/Scene/Light/PointLight.hpp>

namespace Ck
{
	PointLight* PointLight::Create(std::shared_ptr<Scene> scene, LinearColor color, Vector3<float> position, float intensity)
	{
		Transformation transformation(position, Quaternion<float>::Identity(), Vector3<float>::Unit());
		std::shared_ptr<TransformationNode> transformationNode = scene->CreateTransformationNode(transformation);

		UniquePtr<PointLight> pointLight = MakeUnique<PointLight>(std::move(transformationNode), color, intensity);
		PointLight* lightPtr = pointLight.Get();
		scene->AddLight(std::move(pointLight));

		return lightPtr;
	}

	PointLight::PointLight(std::shared_ptr<TransformationNode> transformationNode, LinearColor color, float intensity) :
		Transformable(std::move(transformationNode)),
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
