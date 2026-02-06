#ifndef COCKTAIL_GRAPHIC_SCENE_SCENENODE_HPP
#define COCKTAIL_GRAPHIC_SCENE_SCENENODE_HPP

#include <Cocktail/Core/Math/Volume/Box.hpp>

#include <Cocktail/Graphic/Rendering/Renderable.hpp>
#include <Cocktail/Graphic/Scene/Shape/Shape.hpp>
#include <Cocktail/Graphic/Spatial/Transformable.hpp>

namespace Ck
{
	class IndexBuffer;
	class Scene;
	class VertexBuffer;

	class COCKTAIL_GRAPHIC_API SceneNode : public AcyclicGraphNode<SceneNode>, public Renderable, public Transformable
	{
	public:

		/**
		 * \brief 
		 * \param transformationNode 
		 */
		SceneNode(Scene* scene, std::shared_ptr<TransformationNode> transformationNode);

		/**
		 * \brief 
		 * \param shape 
		 */
		void AddShape(std::shared_ptr<Shape> shape);

		/**
		 * \brief
		 * \param queue
		 * \param camera 
		 */
		void AddToQueue(RenderQueue& queue, const Camera& camera) override;

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
		Array<std::shared_ptr<Shape>> mShapes;
		Box<float> mBoundingBox;
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_SCENENODE_HPP
