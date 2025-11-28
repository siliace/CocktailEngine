#include <Cocktail/Graphic/Scene/Light/Light.hpp>

namespace Ck
{
    LinearColor Light::GetColor() const
    {
        return mColor;
    }

    void Light::SetColor(LinearColor color)
    {
        mColor = color;
    }

    float Light::GetIntensity() const
    {
        return mIntensity;
    }

    void Light::SetIntensity(float intensity)
    {
        mIntensity = intensity;
    }

    Light::Light(LinearColor color, float intensity) :
        mColor(color),
        mIntensity(intensity)
    {
        /// Nothing
    }
}
