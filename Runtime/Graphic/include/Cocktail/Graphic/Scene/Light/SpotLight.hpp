#ifndef COCKTAIL_GRAPHIC_SCENE_LIGHT_SPOTLIGHT_HPP
#define COCKTAIL_GRAPHIC_SCENE_LIGHT_SPOTLIGHT_HPP

#include <Cocktail/Graphic/Scene/Light/Light.hpp>
#include <Cocktail/Graphic/Scene/Light/PositionalLight.hpp>
#include <Cocktail/Graphic/Spatial/Transformable.hpp>

namespace Ck
{
    class Scene;

    class COCKTAIL_GRAPHIC_API SpotLight : public PositionalLight
    {
    public:

        /**
         * \brief
         * \param scene
         * \param color
         * \param position
         * \param intensity
         * \return
         */
        static SpotLight* Create(std::shared_ptr<Scene> scene, float range, LinearColor color, Vector3<float> position = Vector3<float>::Zero(), float intensity = 100.f);

        /**
         * \brief
         * \param transformationNode
         * \param color
         * \param intensity
         */
        SpotLight(std::shared_ptr<TransformationNode> transformationNode, float range, LinearColor color, float intensity = 100.f);

        /**
         * \brief
         * \param frustum
         * \return
         */
        Intersection FrustumCull(const Frustum<float>& frustum) const override;

        /**
         * \brief
         * \return
         */
        Type GetType() const override;

        Angle<float> GetInnerCutoff() const;

        void SetInnerCutoff(const Angle<float>& innerCutoff);

        Angle<float> GetOuterCutoff() const;

        void SetOuterCutoff(const Angle<float>& outerCutoff);

    private:

        Angle<float> mInnerCutoff;
        Angle<float> mOuterCutoff;
    };
}

#endif // COCKTAIL_GRAPHIC_SCENE_LIGHT_SPOTLIGHT_HPP
