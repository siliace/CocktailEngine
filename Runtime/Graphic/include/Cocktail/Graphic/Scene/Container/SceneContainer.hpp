#ifndef COCKTAIL_GRAPHIC_SCENE_CONTAINER_SCENECONTAINER_HPP
#define COCKTAIL_GRAPHIC_SCENE_CONTAINER_SCENECONTAINER_HPP

#include <Cocktail/Graphic/Scene/Scene.hpp>

namespace Ck
{
	class COCKTAIL_GRAPHIC_API SceneContainer
	{
	public:

		/**
		 * \brief Add the content of the SceneContainer into an existing scene
		 * \param scene 
		 * \return 
		 */
		std::shared_ptr<SceneNode> AddToScene(Scene& scene);

	protected:

		struct MaterialInfo
		{
			std::string Name;
			Material::UniformColors Colors;
			Material::ShadingMode ShadingMode;
			EnumMap<Material::TextureType, std::shared_ptr<MipMaps>> Textures;
			bool DoubleSided = false;
			bool Opaque = false;
		};

		struct SubMeshInfo
		{
			std::string Name;
			unsigned int Count;
			unsigned int FirstVertex;
			unsigned int FirstIndex;
			Renderer::PrimitiveTopology PrimitiveTopology = Renderer::PrimitiveTopology::Triangle;
			unsigned int MaterialIndex = 0;
		};

		struct MeshInfo
		{
			std::string Name;
			std::shared_ptr<VertexArray> Vertices;
			std::shared_ptr<IndexArray> Indices;
			Array<SubMeshInfo> SubMeshes;
		};

		struct NodeInfo
		{
			Array<NodeInfo> Children;
			Array<unsigned int> MeshIndices;
			Transformation LocalTransformation = Transformation::Identity();
		};

		NodeInfo mRoot;
		Array<MeshInfo> mMeshes;
		Array<std::shared_ptr<MipMaps>> mMipMaps;
		Array<MaterialInfo> mMaterials;

	private:

		/**
		 * \brief 
		 * \param scene 
		 * \param parent 
		 * \param nodeInfo
		 * \param meshes 
		 * \param materials 
		 * \return 
		 */
		std::shared_ptr<SceneNode> ProcessNode(Scene& scene, std::shared_ptr<SceneNode> parent, NodeInfo& nodeInfo, const Array<std::shared_ptr<Mesh>>& meshes, const Array<std::shared_ptr<Material>>& materials);
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_CONTAINER_SCENECONTAINER_HPP