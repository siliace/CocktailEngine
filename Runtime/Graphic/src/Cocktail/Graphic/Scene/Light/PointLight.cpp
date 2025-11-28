#include <Cocktail/Core/Math/Volume/Sphere.hpp>
#include <Cocktail/Graphic/Scene/Scene.hpp>
#include <Cocktail/Graphic/Scene/Light/PointLight.hpp>

namespace Ck
{
    namespace
    {

    }

    PointLight* PointLight::Create(std::shared_ptr<Scene> scene, float range, LinearColor color, Vector3<float> position, float intensity)
    {
        Transformation transformation(position, Quaternion<float>::Identity(), Vector3<float>::Unit());
        std::shared_ptr<TransformationNode> transformationNode = scene->CreateTransformationNode(transformation);

        UniquePtr<PointLight> pointLight = MakeUnique<PointLight>(std::move(transformationNode), range, color, intensity);
        PointLight* lightPtr = pointLight.Get();
        scene->AddLight(std::move(pointLight));

        return lightPtr;
    }

    PointLight::PointLight(std::shared_ptr<TransformationNode> transformationNode, float range, LinearColor color, float intensity) :
        PositionalLight(std::move(transformationNode), range, color, intensity)
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
