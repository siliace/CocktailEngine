#ifndef COCKTAIL_GRAPHIC_SPATIAL_TRANSFORMATIONNODE_HPP
#define COCKTAIL_GRAPHIC_SPATIAL_TRANSFORMATIONNODE_HPP

#include <Cocktail/Core/Meta/Inherit.hpp>
#include <Cocktail/Core/Utility/AcyclicGraph.hpp>

#include <Cocktail/Graphic/Spatial/Transformation.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API TransformationNode : public Inherit<TransformationNode, Object, AcyclicGraphNode<TransformationNode>>
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
		const Transformation& GetLocalTransformation() const;

		/**
		 * \brief
		 * \return
		 */
		const Transformation& GetWorldTransformation() const;

	private:

		Transformation mLocalTransformation;
		LazyValue<Transformation> mWorldTransformation;
	};
}

#endif // COCKTAIL_GRAPHIC_SPATIAL_TRANSFORMATIONNODE_HPP
