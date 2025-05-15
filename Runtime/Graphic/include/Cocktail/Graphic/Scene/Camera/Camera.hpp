#ifndef COCKTAIL_GRAPHIC_SCENE_CAMERA_CAMERA_HPP
#define COCKTAIL_GRAPHIC_SCENE_CAMERA_CAMERA_HPP

#include <Cocktail/Core/Math/Volume/Frustum.hpp>

#include <Cocktail/Graphic/Spatial/Transformable.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API Camera : public Extends<Camera, Transformable>
	{
	public:

		/**
		 * \brief 
		 * \return 
		 */
		virtual Frustum<float> ComputeFrustum() const = 0;

		/**
		 * \brief 
		 * \return 
		 */
		Matrix4<float> ComputeViewMatrix() const;

		/**
		 * \brief 
		 * \return 
		 */
		virtual Matrix4<float> ComputeProjectionViewMatrix() = 0;

	protected:

		/**
		 * \brief 
		 * \param transformationNode 
		 */
		explicit Camera(Ref<TransformationNode> transformationNode);
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_CAMERA_CAMERA_HPP
