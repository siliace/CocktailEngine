#ifndef COCKTAIL_GRAPHIC_SCENE_CAMERA_CONTROLLER_FREEFLYCAMERACONTROLLER_HPP
#define COCKTAIL_GRAPHIC_SCENE_CAMERA_CONTROLLER_FREEFLYCAMERACONTROLLER_HPP

#include <Cocktail/Core/Math/Angle.hpp>
#include <Cocktail/Core/Utility/EnumMap.hpp>

#include <Cocktail/Graphic/Scene/Camera/Controller/CameraController.hpp>

namespace Ck
{
	class COCKTAIL_GRAPHIC_API FreeFlyCameraViewController : public CameraController
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
		 * \brief Constructor
		 * Create a new FreeFlyCameraViewController to move a camera
		 * \param camera The camera to move
		 * \param pitchLimit The min and max values of pitch possible
		 */
		explicit FreeFlyCameraViewController(std::shared_ptr<Camera> camera, Angle<float> pitchLimit = Angle<float>::Degree(89.f));

		/**
		 * \brief Perform updates on the transformation of the underlying cameras
		 * \param elapsedTime The amount of time since the last update
		 */
		void Update(const Duration& elapsedTime) override;

		/**
		 * \brief Enable or disable a translation axis
		 * \param axis The axis to alter
		 * \param enable True to enable the axis, false to disable it
		 */
		void EnableTranslationAxis(TranslationAxis axis, bool enable = true);

		/**
		 * \brief Enable or disable a rotation axis
		 * \param axis The axis to alter
		 * \param enable True to enable the axis, false to disable it
		 */
		void EnableRotationAxis(RotationAxis axis, bool enable = true);

		/**
		 * \brief Apply translation along an axis
		 * \param axis The axis to translate on
		 * \param amount The quantity of the translation to apply
		 */
		void Translate(TranslationAxis axis, float amount);

		/**
		 * \brief Apply rotation along an axis
		 * \param axis The axis to rotate on
		 * \param amount The quantity of the rotation to apply
		 */
		void Rotate(RotationAxis axis, Angle<float> amount);

	private:

		std::shared_ptr<Camera> mCamera;
		Angle<float> mPitchLimit;
		EnumMap<TranslationAxis, bool> mEnabledTranslations;
		EnumMap<TranslationAxis, float> mTranslations;
		EnumMap<RotationAxis, bool> mEnabledRotations;
		EnumMap<RotationAxis, Angle<float>> mRotations;
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_CAMERA_CONTROLLER_FREEFLYCAMERACONTROLLER_HPP
