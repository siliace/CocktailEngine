#include <Cocktail/Core/Math/InterpolationCurve.hpp>

#include <Cocktail/Graphic/Scene/Scene.hpp>
#include <Cocktail/Graphic/Scene/Light/SpotLight.hpp>

namespace Ck
{
    SpotLight* SpotLight::Create(std::shared_ptr<Scene> scene, float range, LinearColor color, Vector3<float> position, float intensity)
    {
        Transformation transformation(position, Quaternion<float>::Identity(), Vector3<float>::Unit());
        std::shared_ptr<TransformationNode> transformationNode = scene->CreateTransformationNode(transformation);

        UniquePtr<SpotLight> spotLight = MakeUnique<SpotLight>(std::move(transformationNode), range, color, intensity);
        SpotLight* spotLightPtr = spotLight.Get();
        scene->AddLight(std::move(spotLight));

        return spotLightPtr;
    }

    SpotLight::SpotLight(std::shared_ptr<TransformationNode> transformationNode, float range, LinearColor color, float intensity) :
        PositionalLight(transformationNode, range, color, intensity),
        mInnerCutoff(Angle<float>::Degree(15.f)),
        mOuterCutoff(Angle<float>::Degree(30.f))
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

    Angle<float> SpotLight::GetInnerCutoff() const
    {
        return mInnerCutoff;
    }

    void SpotLight::SetInnerCutoff(const Angle<float>& innerCutoff)
    {
        mInnerCutoff = std::clamp(innerCutoff, Angle<float>::Degree(0.f), Angle<float>::Degree(90.f));
    }

    Angle<float> SpotLight::GetOuterCutoff() const
    {
        return mOuterCutoff;
    }

    void SpotLight::SetOuterCutoff(const Angle<float>& outerCutoff)
    {
        mOuterCutoff = std::clamp(outerCutoff, Angle<float>::Degree(0.f), Angle<float>::Degree(90.f));
    }
}
