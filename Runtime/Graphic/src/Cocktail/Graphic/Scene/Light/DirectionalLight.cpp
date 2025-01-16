#include <Cocktail/Graphic/Scene/Scene.hpp>
#include <Cocktail/Graphic/Scene/Light/DirectionalLight.hpp>

namespace Ck
{
	Ref<DirectionalLight> DirectionalLight::Create(Ref<Scene> scene, LinearColor color, Vector3<float> direction, float intensity)
	{
		Ref<DirectionalLight> light = DirectionalLight::New(color, direction, intensity);
		scene->AddLight(light);

		return light;
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
