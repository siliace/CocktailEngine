#ifndef COCKTAIL_GRAPHIC_SCENE_SCENENODE_HPP
#define COCKTAIL_GRAPHIC_SCENE_SCENENODE_HPP

#include <Cocktail/Core/Math/Volume/Box.hpp>
#include <Cocktail/Core/Meta/Inherit.hpp>

#include <Cocktail/Graphic/Material/Material.hpp>
#include <Cocktail/Graphic/Mesh/Mesh.hpp>
#include <Cocktail/Graphic/Rendering/Renderable.hpp>
#include <Cocktail/Graphic/Spatial/Transformable.hpp>

namespace Ck
{
	class IndexBuffer;
	class Scene;
	class VertexBuffer;

	class COCKTAIL_GRAPHIC_API Shape : public Extends<Shape, Object>
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

		Shape(GraphicEngine& graphicEngine, Ref<Mesh> mesh, const std::vector<Ref<Material>>& materials);

		const Ref<Mesh>& GetMesh() const;

		const Ref<VertexBuffer>& GetVertexBuffer() const;

		const Ref<IndexBuffer>& GetIndexBuffer() const;

		const std::unordered_set<Ref<Material>>& GetMaterials() const;

		const std::vector<Geometry>& GetGeometries(const Material* material) const;

	private:

		Ref<Mesh> mMesh;
		Ref<VertexBuffer> mVertexBuffer;
		Ref<IndexBuffer> mIndexBuffer;
		std::unordered_set<Ref<Material>> mMaterials;
		std::unordered_map<const Material*, std::vector<Geometry>> mGeometries;
	};

	class COCKTAIL_GRAPHIC_API SceneNode : public Inherit<SceneNode, Transformable, AcyclicGraphNode<SceneNode>, Renderable>
	{
	public:

		/**
		 * \brief 
		 * \param transformationNode 
		 */
		SceneNode(Scene* scene, Ref<TransformationNode> transformationNode);

		/**
		 * \brief 
		 * \param shape 
		 */
		void AddShape(Ref<Shape> shape);

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
		std::vector<Ref<Shape>> mShapes;
		Box<float> mBoundingBox;
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_SCENENODE_HPP
