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

    const String& Light::GetName() const
    {
        return mName;
    }

    Light::Light(String name, LinearColor color, float intensity) :
        mName(std::move(name)),
        mColor(color),
        mIntensity(intensity)
    {
        /// Nothing
    }
}
