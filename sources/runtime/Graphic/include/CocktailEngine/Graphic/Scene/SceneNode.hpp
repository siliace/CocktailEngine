#ifndef COCKTAIL_GRAPHIC_SCENE_SCENENODE_HPP
#define COCKTAIL_GRAPHIC_SCENE_SCENENODE_HPP

#include <CocktailEngine/Core/Math/Volume/Box.hpp>

#include <CocktailEngine/Graphic/Rendering/Renderable.hpp>
#include <CocktailEngine/Graphic/Scene/Shape/Shape.hpp>
#include <CocktailEngine/Graphic/Spatial/Transformable.hpp>

namespace Ck
{
	class IndexBuffer;
	class Scene;
	class VertexBuffer;

	class COCKTAILENGINE_GRAPHIC_API SceneNode : public AcyclicGraphNode<SceneNode>, public Renderable, public Transformable
	{
	public:

		/**
		 * \brief 
		 * \param transformationNode 
		 */
		SceneNode(Scene* scene, TransformationNode* transformationNode);

		/**
		 * \brief 
		 * \param shape 
		 */
		void AddShape(SharedPtr<Shape> shape);

		/**
		 * \brief
		 * \param queue
		 * \param camera 
		 */
		void AddToQueue(RenderQueue& queue, const Camera* camera) override;

		/**
		 * \brief
		 * \return 
		 */
		bool IsVisible() const;

		/**
		 * \brief 
		 * \param visible 
		 */
		void SetVisible(bool visible = true);

		/**
		 * \brief 
		 * \return 
		 */
		const Volume<float>& GetBoundingVolume() const override;

	private:

		Scene* mScene;
		bool mVisible;
		Array<SharedPtr<Shape>> mShapes;
		Box<float> mBoundingBox;
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_SCENENODE_HPP
