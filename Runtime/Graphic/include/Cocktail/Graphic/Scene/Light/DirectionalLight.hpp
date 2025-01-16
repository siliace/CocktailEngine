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
		 * \param intensity 
		 * \return 
		 */
		static Ref<DirectionalLight> Create(Ref<Scene> scene, LinearColor color, Vector3<float> direction, float intensity = 100.f);

		/**
		 * \brief
		 * \param color
		 * \param direction
		 */
		DirectionalLight(LinearColor color, Vector3<float> direction, float intensity = 100.f);

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
		 * \param color 
		 */
		void SetColor(LinearColor color) override;

		/**
		 * \brief 
		 * \return 
		 */
		float GetIntensity() const override;

		/**
		 * \brief 
		 * \param intensity 
		 */
		void SetIntensity(float intensity) override;

		/**
		 * \brief 
		 * \return 
		 */
		Vector3<float> GetDirection() const;

	private:

		LinearColor mColor;
		Vector3<float> mDirection;
		float mIntensity;
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_LIGHT_DIRECTIONALLIGHT_HPP
