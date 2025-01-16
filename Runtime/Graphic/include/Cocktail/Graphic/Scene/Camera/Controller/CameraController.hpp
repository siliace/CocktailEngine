#ifndef COCKTAIL_GRAPHIC_SCENE_CAMERA_CONTROLLER_CAMERACONTROLLER_HPP
#define COCKTAIL_GRAPHIC_SCENE_CAMERA_CONTROLLER_CAMERACONTROLLER_HPP

#include <Cocktail/Core/Meta/Implements.hpp>
#include <Cocktail/Core/Utility/Time/Duration.hpp>

#include <Cocktail/Graphic/Export.hpp>

namespace Ck
{
	class Camera;

	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API CameraController : public Implements<CameraController, Interface>
	{
	public:

		/**
		 * \brief Perform updates on the transformation of the underlying cameras
		 * \param elapsedTime The amount of time since the last update
		 */
		virtual void Update(const Duration& elapsedTime) = 0;
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_CAMERA_CONTROLLER_CAMERACONTROLLER_HPP
