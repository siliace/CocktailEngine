#ifndef COCKTAIL_GRAPHIC_SCENENODE_HPP
#define COCKTAIL_GRAPHIC_SCENENODE_HPP

#include <Cocktail/Core/Meta/Inherit.hpp>

#include <Cocktail/Graphic/Rendering/Renderable.hpp>
#include <Cocktail/Graphic/Spatial/Transformable.hpp>

namespace Ck
{
	class COCKTAIL_GRAPHIC_API SceneNode : public Inherit<SceneNode, Transformable, AcyclicGraphNode<SceneNode>, Renderable>
	{
	public:

		/**
		 * \brief 
		 * \param transformationNode 
		 */
		explicit SceneNode(Ref<TransformationNode> transformationNode);

		/**
		 * \brief 
		 * \param queue 
		 */
		void AddToQueue(RenderQueue& queue) override;
	};
}

#endif // COCKTAIL_GRAPHIC_SCENENODE_HPP
