#ifndef COCKTAIL_GRAPHIC_SCENEGRAPH_HPP
#define COCKTAIL_GRAPHIC_SCENEGRAPH_HPP

#include <Cocktail/Graphic/SceneNode.hpp>

namespace Ck
{
	/**
	 * \brief
	 */
	class COCKTAIL_GRAPHIC_API SceneGraph : public Extends<SceneGraph, AcyclicGraph<SceneNode>>
	{
	public:

		/**
		 * \brief
		 */
		explicit SceneGraph(Ref<TransformationNode> rootTransformationNode);
	};
}

#endif // COCKTAIL_GRAPHIC_SCENEGRAPH_HPP
