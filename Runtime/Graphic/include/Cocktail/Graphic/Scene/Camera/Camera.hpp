#ifndef COCKTAIL_GRAPHIC_SCENE_CAMERA_CAMERA_HPP
#define COCKTAIL_GRAPHIC_SCENE_CAMERA_CAMERA_HPP

#include <Cocktail/Core/Math/Volume/Frustum.hpp>

#include <Cocktail/Graphic/Scene/Scene.hpp>
#include <Cocktail/Graphic/Spatial/Transformable.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API Camera : public Transformable
	{
	public:

	    enum class Type
	    {
	        Perspective,
	        Orthographic,
	    };

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

        /**
         * \brief
         * \return
         */
	    virtual Type GetType() const = 0;

        /**
         * \brief
         * \return
         */
	    const String& GetName() const;

	protected:

		/**
		 * \brief Constructor
		 *
		 * \param transformationNode The transformation node giving the position and rotation of the camera in the world
		 */
		Camera(TransformationNode* transformationNode, String name);

	private:

	    String mName;
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_CAMERA_CAMERA_HPP
