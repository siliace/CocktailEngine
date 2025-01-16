#ifndef COCKTAIL_GRAPHIC_SCENE_CAMERA_PERSPECTIVECAMERA_HPP
#define COCKTAIL_GRAPHIC_SCENE_CAMERA_PERSPECTIVECAMERA_HPP

#include <Cocktail/Graphic/Scene/Camera/Camera.hpp>

namespace Ck
{
	class Scene;

	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API PerspectiveCamera : public Extends<PerspectiveCamera, Camera>
	{
	public:

		/**
		 * \brief 
		 * \param scene 
		 * \param fov 
		 * \param aspectRatio 
		 * \param zBounds 
		 * \return 
		 */
		static Ref<PerspectiveCamera> Create(Ref<Scene> scene, Angle<float> fov, float aspectRatio, Vector2<float> zBounds);

		/**
		 * \brief 
		 * \param transformationNode 
		 * \param fov 
		 * \param aspectRatio 
		 * \param zBounds 
		 */
		PerspectiveCamera(Ref<TransformationNode> transformationNode, Angle<float> fov, float aspectRatio, Vector2<float> zBounds);

		/**
		 * \brief 
		 * \return 
		 */
		Angle<float> GetFov() const;

		/**
		 * \brief 
		 * \param fov 
		 */
		void SetFov(Angle<float> fov);

		/**
		 * \brief 
		 * \return 
		 */
		float GetAspectRatio() const;

		/**
		 * \brief 
		 * \param aspectRatio 
		 */
		void SetAspectRatio(float aspectRatio);

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

		Angle<float> mFov;
		float mAspectRatio;
		Vector2<float> mZBounds;
		Frustum<float> mFrustum;
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_CAMERA_PERSPECTIVECAMERA_HPP
