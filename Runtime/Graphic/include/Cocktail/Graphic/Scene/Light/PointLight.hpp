#ifndef COCKTAIL_GRAPHIC_SCENE_LIGHT_POINTLIGHT_HPP
#define COCKTAIL_GRAPHIC_SCENE_LIGHT_POINTLIGHT_HPP

#include <Cocktail/Graphic/Scene/Light/PositionalLight.hpp>

namespace Ck
{
	class Scene;

	class COCKTAIL_GRAPHIC_API PointLight : public PositionalLight
	{
	public:

		/**
		 * \brief
		 *
		 * \param scene 
		 * \param range
		 * \param color
		 * \param position
		 * \param intensity
		 *
		 * \return 
		 */
		static PointLight* Create(std::shared_ptr<Scene> scene, float range, LinearColor color = LinearColor(1.f, 1.f, 1.f), Vector3<float> position = Vector3<float>::Zero(), float intensity = 100.f);

		/**
		 * \brief
		 *
		 * \param transformationNode
		 * \param range
		 * \param color
		 * \param intensity 
		 */
		PointLight(std::shared_ptr<TransformationNode> transformationNode, float range, LinearColor color, float intensity = 100.f);

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
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_LIGHT_POINTLIGHT_HPP
