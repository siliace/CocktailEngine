#ifndef COCKTAIL_GRAPHIC_SPATIAL_TRANSFORMATIONNODE_HPP
#define COCKTAIL_GRAPHIC_SPATIAL_TRANSFORMATIONNODE_HPP

#include <CocktailEngine/Core/Utility/AcyclicGraph.hpp>

#include <CocktailEngine/Graphic/Spatial/Transformation.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAILENGINE_GRAPHIC_API TransformationNode : public AcyclicGraphNode<TransformationNode>
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

	protected:

		/**
		 * \brief Invalidates the cached world transformation after a move in the hierarchy
		 *
		 * The world transformation of a node is its local one composed with its
		 * parent's, so reparenting changes it without anything on the node itself having
		 * been written. Without this the cache would keep answering with the position
		 * the node held under its previous parent.
		 *
		 * \param previousParent Parent the node had, null when it had none
		 */
		void OnParentChanged(TransformationNode* previousParent) override;

	private:

		void SetDirty(bool dirty = true);

		Transformation mLocalTransformation;
		LazyValue<Transformation> mWorldTransformation;
	};
}

#endif // COCKTAIL_GRAPHIC_SPATIAL_TRANSFORMATIONNODE_HPP
