#ifndef COCKTAIL_GRAPHIC_SCENE_LIGHT_DIRECTIONALLIGHT_HPP
#define COCKTAIL_GRAPHIC_SCENE_LIGHT_DIRECTIONALLIGHT_HPP

#include <Cocktail/Core/Meta/Inherit.hpp>

#include <Cocktail/Graphic/Scene/Light/Light.hpp>

namespace Ck
{
	class Scene;

	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API DirectionalLight : public Inherit<DirectionalLight, Object, Light>
	{
	public:

		/**
		 * \brief 
		 * \param scene 
		 * \param color 
		 * \param direction 
		 * \return 
		 */
		static Ref<DirectionalLight> Create(Ref<Scene> scene, LinearColor color, Vector3<float> direction);

		/**
		 * \brief
		 * \param color
		 * \param direction
		 */
		DirectionalLight(LinearColor color, Vector3<float> direction);

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

#endif // COCKTAIL_GRAPHIC_SCENE_LIGHT_DIRECTIONALLIGHT_HPP
