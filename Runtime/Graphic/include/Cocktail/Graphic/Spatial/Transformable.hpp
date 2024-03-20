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
		 * \return 
		 */
		Ref<TransformationNode> GetTransformationNode() const;

	protected:

		/**
		 * \brief
		 * \param transformationNode
		 */
		explicit Transformable(Ref<TransformationNode> transformationNode);

	private:

		Ref<TransformationNode> mTransformationNode;
	};
}

#endif // COCKTAIL_GRAPHIC_SPATIAL_TRANSFORMABLE_HPP
