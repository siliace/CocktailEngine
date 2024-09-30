#ifndef COCKTAIL_GRAPHIC_SCENE_SCENENODE_HPP
#define COCKTAIL_GRAPHIC_SCENE_SCENENODE_HPP

#include <Cocktail/Core/Meta/Inherit.hpp>

#include <Cocktail/Graphic/Rendering/Renderable.hpp>
#include <Cocktail/Graphic/Spatial/Transformable.hpp>

#include "Cocktail/Graphic/Material/Material.hpp"
#include "Cocktail/Graphic/Mesh/Mesh.hpp"

namespace Ck
{
	class COCKTAIL_GRAPHIC_API Shape : public Extends<Shape, Object>
	{
	public:

		enum class BlendingMode
		{
			Opaque,
			Additive,
			PreMultiplicate,
		};

		Shape(Ref<Mesh> mesh, Ref<Material> material, BlendingMode blendingMode = BlendingMode::Opaque, unsigned int coplanarityIndex = 0) :
			mMesh(std::move(mesh)),
			mMaterial(std::move(material)),
			mBlendingMode(blendingMode),
			mCoplanarityIndex(coplanarityIndex)
		{
			
		}

		Ref<Mesh> GetMesh() const
		{
			return mMesh;
		}

		Ref<Material> GetMaterial() const
		{
			return mMaterial;
		}

		BlendingMode GetBlendingMode() const
		{
			return mBlendingMode;
		}

		unsigned int GetCoplanarityIndex() const
		{
			return mCoplanarityIndex;
		}

	private:

		Ref<Mesh> mMesh;
		Ref<Material> mMaterial;
		BlendingMode mBlendingMode;
		unsigned int mCoplanarityIndex;
	};

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
		 * \param shape 
		 */
		void AddShape(Ref<Shape> shape);

		/**
		 * \brief 
		 * \param queue 
		 */
		void AddToQueue(RenderQueue& queue) override;

	private:

		std::vector<Ref<Shape>> mShapes;
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_SCENENODE_HPP
