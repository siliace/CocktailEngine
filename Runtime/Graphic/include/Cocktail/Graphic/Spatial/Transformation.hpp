#ifndef COCKTAIL_GRAPHIC_SPATIAL_TRANSFORMATION_HPP
#define COCKTAIL_GRAPHIC_SPATIAL_TRANSFORMATION_HPP

#include <Cocktail/Core/Math/Quaternion.hpp>
#include <Cocktail/Core/Math/Matrix/Matrix4.hpp>
#include <Cocktail/Core/Math/Vector/Vector3.hpp>
#include <Cocktail/Core/Utility/LazyValue.hpp>

#include <Cocktail/Graphic/Export.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API Transformation
	{
	public:

		/**
		 * \brief 
		 * \return 
		 */
		static Transformation Identity();

		static Transformation Decompose(const Matrix4<float>& transformationMatrix);

		/**
		 * \brief 
		 */
		Transformation() = default;

		/**
		 * \brief 
		 * \param translation 
		 * \param rotation 
		 * \param scale 
		 */
		Transformation(Vector3<float> translation, Quaternion<float> rotation, Vector3<float> scale);

		/**
		 * \brief 
		 * \return 
		 */
		bool IsScaleUniform() const;

		/**
		 * \brief 
		 * \param parent 
		 * \return 
		 */
		Transformation Compose(const Transformation& parent) const;

		/**
		 * \brief 
		 * \return 
		 */
		Transformation GetInverse() const;

		/**
		 * \brief 
		 * \param vertex 
		 * \return 
		 */
		Vector3<float> Apply(const Vector3<float>& vertex) const;

		/**
		 * \brief 
		 * \return 
		 */
		Vector3<float> GetTranslation() const;

		/**
		 * \brief 
		 * \param translation  
		 * \return 
		 */
		Transformation& SetTranslation(Vector3<float> translation);

		/**
		 * \brief 
		 * \return 
		 */
		Quaternion<float> GetRotation() const;

		/**
		 * \brief 
		 * \param rotation  
		 * \return 
		 */
		Transformation& SetRotation(Quaternion<float> rotation);

		/**
		 * \brief 
		 * \return 
		 */
		Vector3<float> GetScale() const;

		/**
		 * \brief 
		 * \param scale 
		 * \return 
		 */
		Transformation& SetScale(float scale);

		/**
		 * \brief 
		 * \param scale 
		 * \return  
		 */
		Transformation& SetScale(Vector3<float> scale);

		/**
		 * \brief Convert the transformation to a matrix
		 * \return The matrix
		 */
		const Matrix4<float>& ToMatrix() const;

	private:

		Vector3<float> mTranslation;
		Quaternion<float> mRotation;
		Vector3<float> mScale;
		LazyValue<Matrix4<float>> mMatrix;
	};
}
#endif // COCKTAIL_GRAPHIC_SPATIAL_TRANSFORMATION_HPP
