#ifndef COCKTAIL_GRAPHIC_SPATIAL_TRANSFORMATIONGRAPH_HPP
#define COCKTAIL_GRAPHIC_SPATIAL_TRANSFORMATIONGRAPH_HPP

#include <Cocktail/Graphic/Spatial/TransformationNode.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API TransformationGraph : public AcyclicGraph<TransformationNode>
	{
	public:

		/**
		 * \brief 
		 */
		TransformationGraph();
	};
}

#endif // COCKTAIL_GRAPHIC_SPATIAL_TRANSFORMATIONGRAPH_HPP
