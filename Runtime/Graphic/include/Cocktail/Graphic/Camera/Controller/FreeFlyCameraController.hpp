#ifndef COCKTAIL_GRAPHIC_CAMERA_CONTROLLER_FREEFLYCAMERACONTROLLER_HPP
#define COCKTAIL_GRAPHIC_CAMERA_CONTROLLER_FREEFLYCAMERACONTROLLER_HPP

#include <Cocktail/Core/Object.hpp>
#include <Cocktail/Core/Math/Angle.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>
#include <Cocktail/Core/Utility/EnumMap.hpp>

#include <Cocktail/Graphic/Camera/Controller/CameraController.hpp>

namespace Ck
{
	class COCKTAIL_GRAPHIC_API FreeFlyCameraViewController : public Inherit<FreeFlyCameraViewController, Object, CameraController>
	{
	public:

		enum class TranslationAxis
		{
			Right,
			Up,
			Front,
		};

		enum class RotationAxis
		{
			Yaw,
			Pitch,
		};

		/**
		 * \brief 
		 * \param camera
		 * \param pitchLimit 
		 */
		explicit FreeFlyCameraViewController(Ref<Camera> camera, Angle<float> pitchLimit = Angle<float>::Degree(89.f));

		/**
		 * \brief 
		 */
		 void Update() override;

		/**
		 * \brief
		 * \param axis
		 * \param enable
		 */
		void EnableTranslationAxis(TranslationAxis axis, bool enable = true);

		/**
		 * \brief
		 * \param axis
		 * \param enable
		 */
		void EnableRotationAxis(RotationAxis axis, bool enable = true);

		/**
		 * \brief
		 * \param axis
		 * \param amount
		 */
		void Translate(TranslationAxis axis, float amount);

		/**
		 * \brief
		 * \param axis
		 * \param amount
		 */
		void Rotate(RotationAxis axis, Angle<float> amount);

	private:

		Ref<Camera> mCamera;
		Angle<float> mPitchLimit;
		EnumMap<TranslationAxis, bool> mEnabledTranslations;
		EnumMap<TranslationAxis, float> mTranslations;
		EnumMap<RotationAxis, bool> mEnabledRotations;
		EnumMap<RotationAxis, Angle<float>> mRotations;
	};
}

#endif // COCKTAIL_GRAPHIC_CAMERA_CONTROLLER_FREEFLYCAMERACONTROLLER_HPP
