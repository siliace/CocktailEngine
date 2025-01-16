#ifndef COCKTAIL_GRAPHIC_CAMERA_CONTROLLER_CAMERACONTROLLER_HPP
#define COCKTAIL_GRAPHIC_CAMERA_CONTROLLER_CAMERACONTROLLER_HPP

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
		 * \brief 
		 * \param delta 
		 */
		virtual void Update(const Duration& delta) = 0;
	};
}

#endif // COCKTAIL_GRAPHIC_CAMERA_CONTROLLER_CAMERACONTROLLER_HPP
