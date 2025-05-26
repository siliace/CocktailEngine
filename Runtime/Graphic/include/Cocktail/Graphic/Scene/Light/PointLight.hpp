#ifndef COCKTAIL_GRAPHIC_SCENE_LIGHT_POINTLIGHT_HPP
#define COCKTAIL_GRAPHIC_SCENE_LIGHT_POINTLIGHT_HPP

#include <Cocktail/Graphic/Scene/Light/Light.hpp>
#include <Cocktail/Graphic/Spatial/Transformable.hpp>

namespace Ck
{
	class Scene;

	class COCKTAIL_GRAPHIC_API PointLight : public Light, public Transformable
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
		static std::shared_ptr<PointLight> Create(std::shared_ptr<Scene> scene, LinearColor color = LinearColor(1.f, 1.f, 1.f), Vector3<float> position = Vector3<float>::Zero(), float intensity = 100.f);

		/**
		 * \brief 
		 * \param transformationNode 
		 * \param color
		 * \param intensity 
		 */
		PointLight(std::shared_ptr<TransformationNode> transformationNode, LinearColor color, float intensity = 100.f);

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

	private:

		LinearColor mColor;
		float mIntensity;
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_LIGHT_POINTLIGHT_HPP
