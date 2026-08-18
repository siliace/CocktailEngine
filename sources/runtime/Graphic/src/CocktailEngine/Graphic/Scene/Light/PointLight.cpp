#include <CocktailEngine/Core/Math/Volume/Sphere.hpp>
#include <CocktailEngine/Graphic/Scene/Scene.hpp>
#include <CocktailEngine/Graphic/Scene/Light/PointLight.hpp>

namespace Ck
{
    PointLight* PointLight::Create(SharedPtr<Scene> scene, String name, float range, LinearColor color, Vector3<float> position, float intensity)
    {
        Transformation transformation(position, Quaternion<float>::Identity(), Vector3<float>::Unit());
        TransformationNode* transformationNode = scene->CreateTransformationNode(transformation);

        UniquePtr<PointLight> pointLight = MakeUnique<PointLight>(transformationNode, Ck::Move(name), range, color, intensity);
        PointLight* lightPtr = pointLight.Get();
        scene->AddLight(Ck::Move(pointLight));

        return lightPtr;
    }

    PointLight::PointLight(TransformationNode* transformationNode, String name,  float range, LinearColor color, float intensity) :
        PositionalLight(transformationNode, Ck::Move(name), range, color, intensity)
    {
        /// Nothing
    }

    Intersection PointLight::FrustumCull(const Frustum<float>& frustum) const
    {
        return Sphere(GetRange(), GetPosition()).Intersect(frustum);
    }

    Light::Type PointLight::GetType() const
    {
        return Type::Point;
    }
}
