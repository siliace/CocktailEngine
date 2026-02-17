#include <Cocktail/Graphic/Scene/Light/PositionalLight.hpp>

namespace Ck
{
    const ScalarInterpolationCurve<float> PositionalLight::DefaultLinearAttenuationCurve = ScalarInterpolationCurve<float>::Builder()
        .SetKeyFrame(7.f, 0.7f)
        .SetKeyFrame(13.f, 0.35f)
        .SetKeyFrame(20.f, 0.22f)
        .SetKeyFrame(32.f, 0.14f)
        .SetKeyFrame(50.f, 0.09f)
        .SetKeyFrame(65.f, 0.07f)
        .SetKeyFrame(100.f, 0.0045f)
        .SetKeyFrame(160.f, 0.0027f)
        .SetKeyFrame(200.f, 0.0022f)
        .SetKeyFrame(325.f, 0.0014f)
        .SetKeyFrame(600.f, 0.007f)
        .SetKeyFrame(3250.f, 0.00014f)
        .SetInterpolationFunction(Smoothstep)
        .Get();

    const ScalarInterpolationCurve<float> PositionalLight::DefaultQuadraticAttenuationCurve = ScalarInterpolationCurve<float>::Builder()
        .SetKeyFrame(7.f, 1.8f)
        .SetKeyFrame(13.f, 0.44f)
        .SetKeyFrame(20.f, 0.2f)
        .SetKeyFrame(32.f, 0.07f)
        .SetKeyFrame(50.f, 0.032f)
        .SetKeyFrame(65.f, 0.017f)
        .SetKeyFrame(100.f, 0.0075f)
        .SetKeyFrame(160.f, 0.0028f)
        .SetKeyFrame(200.f, 0.0019f)
        .SetKeyFrame(325.f, 0.0007f)
        .SetKeyFrame(600.f, 0.0002f)
        .SetKeyFrame(3250.f, 0.000007f)
        .SetInterpolationFunction(Smoothstep)
        .Get();

    float PositionalLight::GetRange() const
    {
        return mRange;
    }

    void PositionalLight::SetRange(float range)
    {
        mRange = range;
        mLinearAttenuationFactor.SetDirty();
        mQuadraticAttenuationFactor.SetDirty();
    }

    float PositionalLight::ComputeAttenuation(float distance) const
    {
        float constantFactor = GetConstantAttenuationFactor();
        float linearFactor = GetLinearAttenuationFactor() * distance;
        float quadraticFactor = GetQuadraticAttenuationFactor() * std::pow(distance, 2.f);

        return 1.f / (constantFactor + linearFactor + quadraticFactor);
    }

    float PositionalLight::GetConstantAttenuationFactor() const
    {
        return 1.f;
    }

    float PositionalLight::GetLinearAttenuationFactor() const
    {
        return mLinearAttenuationFactor.Get([&]() {
            return mLinearAttenuationCurve.Get(mRange);
        });
    }

    float PositionalLight::GetQuadraticAttenuationFactor() const
    {
        return mQuadraticAttenuationFactor.Get([&]() {
            return mQuadraticAttenuationCurve.Get(mRange);
        });
    }

    PositionalLight::PositionalLight(std::shared_ptr<TransformationNode> transformationNode, float range, LinearColor color, float intensity) :
        Transformable(std::move(transformationNode)),
        Light(color, intensity),
        mRange(range),
        mLinearAttenuationCurve(DefaultLinearAttenuationCurve),
        mQuadraticAttenuationCurve(DefaultQuadraticAttenuationCurve)
    {
        mLinearAttenuationFactor.SetDirty();
        mQuadraticAttenuationFactor.SetDirty();
    }
}
