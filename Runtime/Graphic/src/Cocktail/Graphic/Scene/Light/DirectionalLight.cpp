#include <Cocktail/Graphic/Scene/Scene.hpp>
#include <Cocktail/Graphic/Scene/Light/DirectionalLight.hpp>

namespace Ck
{
	DirectionalLight* DirectionalLight::Create(std::shared_ptr<Scene> scene, LinearColor color, Vector3<float> direction, float intensity)
	{
		std::unique_ptr<DirectionalLight> light = std::make_unique<DirectionalLight>(color, direction, intensity);
		DirectionalLight* lightPtr = light.get();
		scene->AddLight(std::move(light));

		return lightPtr;
	}

	DirectionalLight::DirectionalLight(LinearColor color, Vector3<float> direction, float intensity) :
		mColor(color),
		mDirection(direction),
		mIntensity(intensity)
	{
		/// Nothing
	}

	Intersection DirectionalLight::FrustumCull(const Frustum<float>& frustum) const
	{
		return Intersection::Inside;
	}

	Light::Type DirectionalLight::GetType() const
	{
		return Type::Directional;
	}

	LinearColor DirectionalLight::GetColor() const
	{
		return mColor;
	}

	void DirectionalLight::SetColor(LinearColor color)
	{
		mColor = color;
	}

	float DirectionalLight::GetIntensity() const
	{
		return mIntensity;
	}

	void DirectionalLight::SetIntensity(float intensity)
	{
		mIntensity = intensity;
	}

	Vector3<float> DirectionalLight::GetDirection() const
	{
		return mDirection;
	}
}
