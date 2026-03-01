#ifndef COCKTAIL_GRAPHIC_SCENE_CAMERA_ORHTOGRAPHICCAMERA_HPP
#define COCKTAIL_GRAPHIC_SCENE_CAMERA_ORHTOGRAPHICCAMERA_HPP

#include <Cocktail/Core/Math/Polygon/Rectangle.hpp>

#include <Cocktail/Graphic/Scene/Camera/Camera.hpp>

namespace Ck
{
	class Scene;

	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API OrthographicCamera : public Camera
	{
	public:

		/**
		 * \brief 
		 * \param scene 
		 * \param zBounds 
		 * \return 
		 */
		static OrthographicCamera* Create(std::shared_ptr<Scene> scene, const Rectangle<float>& area, Vector2<float> zBounds);

		/**
		 * \brief 
		 * \param transformationNode 
		 * \param area 
		 * \param zBounds 
		 */
		OrthographicCamera(std::shared_ptr<TransformationNode> transformationNode, Rectangle<float> area, Vector2<float> zBounds);

        /**
         * \brief
         * \return
         */
        const Rectangle<float>& GetArea() const;

        /**
         * \brief
         * \param area
         */
	    void SetArea(const Rectangle<float>& area);

        /**
         * \brief
         * \return
         */
		Frustum<float> ComputeFrustum() const override;

		/**
		 * \brief 
		 * \return 
		 */
		Matrix4<float> ComputeProjectionViewMatrix() override;

	private:

		Rectangle<float> mArea;
		Vector2<float> mZBounds;
		LazyValue<Matrix4<float>> mProjectionViewMatrix;
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_CAMERA_ORHTOGRAPHICCAMERA_HPP
