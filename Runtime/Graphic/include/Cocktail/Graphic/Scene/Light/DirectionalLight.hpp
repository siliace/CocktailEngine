#ifndef COCKTAIL_GRAPHIC_SCENE_LIGHT_DIRECTIONALLIGHT_HPP
#define COCKTAIL_GRAPHIC_SCENE_LIGHT_DIRECTIONALLIGHT_HPP

#include <memory>

#include <Cocktail/Graphic/Scene/Light/Light.hpp>

namespace Ck
{
	class Scene;

	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API DirectionalLight : public Light
	{
	public:

		/**
		 * \brief
		 *
		 * \param scene
		 * \param direction
		 * \param color
		 * \param intensity
		 *
		 * \return 
		 */
		static DirectionalLight* Create(std::shared_ptr<Scene> scene, Vector3<float> direction, LinearColor color,  float intensity = 100.f);

		/**
		 * \brief
		 * \param direction
		 * \param color
		 * \param intensity
		 */
		DirectionalLight(Vector3<float> direction, LinearColor color, float intensity = 100.f);

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
		Vector3<float> GetDirection() const;

        /**
	     * \brief
	     * \param direction
	     */
	    void SetDirection(Vector3<float> direction);

	private:

		Vector3<float> mDirection;
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_LIGHT_DIRECTIONALLIGHT_HPP
