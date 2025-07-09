#ifndef COCKTAIL_GRAPHIC_SCENE_SCENENODE_HPP
#define COCKTAIL_GRAPHIC_SCENE_SCENENODE_HPP

#include <Cocktail/Core/Math/Volume/Box.hpp>

#include <Cocktail/Graphic/Material/Material.hpp>
#include <Cocktail/Graphic/Mesh/Mesh.hpp>
#include <Cocktail/Graphic/Rendering/Renderable.hpp>
#include <Cocktail/Graphic/Spatial/Transformable.hpp>

namespace Ck
{
	class IndexBuffer;
	class Scene;
	class VertexBuffer;

	class COCKTAIL_GRAPHIC_API Shape
	{
	public:

		/**
		 * \brief 
		 */
		struct Geometry
		{
			/**
			 * \brief 
			 */
			unsigned int Count = 0;

			/**
			 * \brief 
			 */
			unsigned int FirstVertex = 0;

			/**
			 * \brief 
			 */
			unsigned int FirstIndex = 0;

			/**
			 * \brief 
			 */
			Renderer::PrimitiveTopology PrimitiveTopology = Renderer::PrimitiveTopology::Triangle;
		};

		Shape(GraphicEngine& graphicEngine, std::shared_ptr<Mesh> mesh, const Array<std::shared_ptr<Material>>& materials);

		const std::shared_ptr<Mesh>& GetMesh() const;

		const std::shared_ptr<VertexBuffer>& GetVertexBuffer() const;

		const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const;

		const std::unordered_set<std::shared_ptr<Material>>& GetMaterials() const;

		const Array<Geometry>& GetGeometries(const Material* material) const;

	private:

		std::shared_ptr<Mesh> mMesh;
		std::shared_ptr<VertexBuffer> mVertexBuffer;
		std::shared_ptr<IndexBuffer> mIndexBuffer;
		std::unordered_set<std::shared_ptr<Material>> mMaterials;
		std::unordered_map<const Material*, Array<Geometry>> mGeometries;
	};

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
