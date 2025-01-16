#ifndef COCKTAIL_GRAPHIC_SCENE_LIGHT_POINTLIGHT_HPP
#define COCKTAIL_GRAPHIC_SCENE_LIGHT_POINTLIGHT_HPP

#include <Cocktail/Graphic/Scene/Light/Light.hpp>
#include <Cocktail/Graphic/Spatial/Transformable.hpp>

#include <Cocktail/Core/Meta/Inherit.hpp>

namespace Ck
{
	class Scene;

	class PointLight : public Inherit<PointLight, Transformable, Light>
	{
	public:

		/**
		 * \brief 
		 * \param scene 
		 * \param color 
		 * \param direction 
		 * \param position 
		 * \return 
		 */
		static Ref<PointLight> Create(Ref<Scene> scene, LinearColor color, Vector3<float> direction, Vector3<float> position);

		/**
		 * \brief 
		 * \param transformationNode 
		 * \param color 
		 * \param direction 
		 */
		PointLight(Ref<TransformationNode> transformationNode, LinearColor color, Vector3<float> direction);

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

		/**
		 * \brief 
		 * \return 
		 */
		LinearColor GetColor() const override;

		/**
		 * \brief 
		 * \return 
		 */
		Vector3<float> GetDirection() const override;

	private:

		LinearColor mColor;
		Vector3<float> mDirection;
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_LIGHT_POINTLIGHT_HPP
