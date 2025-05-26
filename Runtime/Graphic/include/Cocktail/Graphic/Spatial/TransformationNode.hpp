#ifndef COCKTAIL_GRAPHIC_SPATIAL_TRANSFORMATIONNODE_HPP
#define COCKTAIL_GRAPHIC_SPATIAL_TRANSFORMATIONNODE_HPP

#include <Cocktail/Core/Utility/AcyclicGraph.hpp>

#include <Cocktail/Graphic/Spatial/Transformation.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API TransformationNode : public AcyclicGraphNode<TransformationNode>
	{
	public:

		/**
		 * \brief 
		 * \param localTransformation 
		 */
		explicit TransformationNode(Transformation localTransformation);

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

	private:

		void SetDirty(bool dirty = true);

		Transformation mLocalTransformation;
		LazyValue<Transformation> mWorldTransformation;
	};
}

#endif // COCKTAIL_GRAPHIC_SPATIAL_TRANSFORMATIONNODE_HPP
