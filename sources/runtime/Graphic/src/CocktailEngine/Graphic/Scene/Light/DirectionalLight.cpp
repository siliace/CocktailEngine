#include <CocktailEngine/Graphic/Scene/Scene.hpp>
#include <CocktailEngine/Graphic/Scene/Light/DirectionalLight.hpp>

namespace Ck
{
    DirectionalLight* DirectionalLight::Create(SharedPtr<Scene> scene, String name, Vector3<float> direction, LinearColor color, float intensity)
    {
        UniquePtr<DirectionalLight> light = MakeUnique<DirectionalLight>(Move(name), direction.Normalized(), color, intensity);
        DirectionalLight* lightPtr = light.Get();
        scene->AddLight(Move(light));

        return lightPtr;
    }

    DirectionalLight::DirectionalLight(String name, Vector3<float> direction, LinearColor color, float intensity) :
        Light(Move(name), color, intensity),
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
