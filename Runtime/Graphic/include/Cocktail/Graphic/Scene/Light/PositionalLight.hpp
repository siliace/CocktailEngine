#ifndef COCKTAIL_GRAPHIC_SCENE_LIGHT_POSITIONALLIGHT_HPP
#define COCKTAIL_GRAPHIC_SCENE_LIGHT_POSITIONALLIGHT_HPP

#include <Cocktail/Core/Math/InterpolationCurve.hpp>

#include <Cocktail/Graphic/Scene/Light/Light.hpp>
#include <Cocktail/Graphic/Spatial/Transformable.hpp>

namespace Ck
{
    /**
     * \brief
     */
    class COCKTAIL_GRAPHIC_API PositionalLight : public Transformable, public Light
    {
    public:

        /**
         * \brief
         */
        static const ScalarInterpolationCurve<float> DefaultLinearAttenuationCurve;

        /**
         * \brief
         */
        static const ScalarInterpolationCurve<float> DefaultQuadraticAttenuationCurve;

        /**
         * \brief
         *
         * \return
         */
        float GetRange() const;

        /**
         * \brief
         *
         * \param range
         */
        void SetRange(float range);

        /**
         * \brief Compute the attenuation for at a given distance from the light
         */
        float ComputeAttenuation(float distance) const;

        /**
         * \brief
         * \return
         */
        float GetConstantAttenuationFactor() const;

        /**
         * \brief
         * \return
         */
        float GetLinearAttenuationFactor() const;

        /**
         * \brief
         * \return
         */
        float GetQuadraticAttenuationFactor() const;

    protected:

        /**
         * \brief
         *
         * \param transformationNode
         * \param range
         * \param color
         * \param intensity
         */
        PositionalLight(const std::shared_ptr<TransformationNode>& transformationNode, float range, LinearColor color, float intensity);

    private:

        float mRange;
        LazyValue<float> mLinearAttenuationFactor;
        LazyValue<float> mQuadraticAttenuationFactor;
        ScalarInterpolationCurve<float> mLinearAttenuationCurve;
        ScalarInterpolationCurve<float> mQuadraticAttenuationCurve;
    };
}

#endif // COCKTAIL_GRAPHIC_SCENE_LIGHT_POSITIONALLIGHT_HPP
