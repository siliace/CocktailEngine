#ifndef COCKTAIL_GRAPHIC_SPATIAL_TRANSFORMABLE_HPP
#define COCKTAIL_GRAPHIC_SPATIAL_TRANSFORMABLE_HPP

#include <Cocktail/Graphic/Spatial/TransformationNode.hpp>

namespace Ck
{
	/**
	 * \brief Utility base class of object with a 3D position in the world
	 */
	class COCKTAIL_GRAPHIC_API Transformable : public Extends<Transformable, Object>
	{
	public:

		/**
		 * \brief 
		 * \param offset 
		 */
		void Move(Vector3<float> offset);

		/**
		 * \brief 
		 * \param parent 
		 */
		void AttachTo(const Transformable& parent);

		/**
		 * \brief 
		 * \return 
		 */
		Vector3<float> GetRight() const;

		/**
		 * \brief 
		 * \return 
		 */
		Vector3<float> GetUp() const;

		/**
		 * \brief 
		 * \return 
		 */
		Vector3<float> GetFront() const;

		/**
		 * \brief 
		 * \return 
		 */
		Vector3<float> GetPosition() const;

		/**
		 * \brief 
		 * \param position 
		 */
		void SetPosition(Vector3<float> position);

		/**
		 * \brief 
		 * \return 
		 */
		Quaternion<float> GetRotation() const;

		/**
		 * \brief 
		 * \param rotation 
		 */
		void SetRotation(Quaternion<float> rotation);

		/**
		 * \brief 
		 * \return 
		 */
		const Transformation& GetLocalTransformation() const;

		/**
		 * \brief 
		 * \param transformation 
		 */
		void SetLocalTransformation(const Transformation& transformation);

		/**
		 * \brief 
		 * \return 
		 */
		const Transformation& GetWorldTransformation() const;

	protected:

		/**
		 * \brief
		 * \param transformationNode
		 */
		explicit Transformable(Ref<TransformationNode> transformationNode);

		/**
		 * \brief
		 * \return
		 */
		Ref<TransformationNode> GetTransformationNode() const;

	private:

		Ref<TransformationNode> mTransformationNode;
	};
}

#endif // COCKTAIL_GRAPHIC_SPATIAL_TRANSFORMABLE_HPP
