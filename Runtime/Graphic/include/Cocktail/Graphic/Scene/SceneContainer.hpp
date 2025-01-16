#ifndef COCKTAIL_GRAPHIC_SCENE_SCENECONTAINER_HPP
#define COCKTAIL_GRAPHIC_SCENE_SCENECONTAINER_HPP

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

		struct MeshInfo
		{
			Ref<VertexArray> Vertices;
			Ref<IndexArray> Indices;
			Renderer::PrimitiveTopology PrimitiveTopology = Renderer::PrimitiveTopology::Triangle;
			unsigned int MaterialIndex = 0;
		};

		struct NodeInfo
		{
			std::vector<NodeInfo> Children;
			std::vector<const MeshInfo*> Meshes;
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
		 * \param materials 
		 * \return 
		 */
		Ref<SceneNode> ProcessNode(Scene& scene, Ref<SceneNode> parent, NodeInfo& nodeInfo, const std::vector<Ref<Material>>& materials);
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_SCENECONTAINER_HPP