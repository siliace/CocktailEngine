#ifndef COCKTAIL_GRAPHIC_SCENE_CONTAINER_SCENECONTAINER_HPP
#define COCKTAIL_GRAPHIC_SCENE_CONTAINER_SCENECONTAINER_HPP

#include <Cocktail/Graphic/Scene/Scene.hpp>

namespace Ck
{
	class COCKTAIL_GRAPHIC_API SceneContainer : public Extends<SceneContainer, Object>
	{
	public:

		/**
		 * \brief Add the content of the SceneContainer into an existing scene
		 * \param scene 
		 * \return 
		 */
		Ref<SceneNode> AddToScene(Scene& scene);

	protected:

		struct MaterialInfo
		{
			std::string Name;
			Material::UniformColors Colors;
			Material::ShadingMode ShadingMode;
			EnumMap<Material::TextureType, Ref<MipMaps>> Textures;
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
			Ref<VertexArray> Vertices;
			Ref<IndexArray> Indices;
			std::vector<SubMeshInfo> SubMeshes;
		};

		struct NodeInfo
		{
			std::vector<NodeInfo> Children;
			std::vector<unsigned int> MeshIndices;
			Transformation LocalTransformation = Transformation::Identity();
		};

		NodeInfo mRoot;
		std::vector<MeshInfo> mMeshes;
		std::vector<Ref<MipMaps>> mMipMaps;
		std::vector<MaterialInfo> mMaterials;

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
		Ref<SceneNode> ProcessNode(Scene& scene, Ref<SceneNode> parent, NodeInfo& nodeInfo, const std::vector<Ref<Mesh>>& meshes, const std::vector<Ref<Material>>& materials);
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_CONTAINER_SCENECONTAINER_HPP