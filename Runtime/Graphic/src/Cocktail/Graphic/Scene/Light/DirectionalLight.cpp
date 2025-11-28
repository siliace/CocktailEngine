#include <Cocktail/Graphic/Scene/Scene.hpp>
#include <Cocktail/Graphic/Scene/Light/DirectionalLight.hpp>

namespace Ck
{
    DirectionalLight* DirectionalLight::Create(std::shared_ptr<Scene> scene, Vector3<float> direction, LinearColor color, float intensity)
    {
        UniquePtr<DirectionalLight> light = MakeUnique<DirectionalLight>(direction.Normalized(), color, intensity);
        DirectionalLight* lightPtr = light.Get();
        scene->AddLight(std::move(light));

        return lightPtr;
    }

    DirectionalLight::DirectionalLight(Vector3<float> direction, LinearColor color, float intensity) :
        Light(color, intensity),
        mDirection(direction)
    {
        assert(NearlyEqual(mDirection.GetLength(), 1.f));
    }

    Intersection DirectionalLight::FrustumCull(const Frustum<float>& frustum) const
    {
        return Intersection::Inside;
    }

    Light::Type DirectionalLight::GetType() const
    {
        return Type::Directional;
    }

    Vector3<float> DirectionalLight::GetDirection() const
    {
        return mDirection;
    }

    void DirectionalLight::SetDirection(Vector3<float> direction)
    {
        mDirection = direction.Normalized();
    }
}
