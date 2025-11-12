#ifndef COCKTAIL_GRAPHIC_SCENE_CONTAINER_SCENECONTAINER_HPP
#define COCKTAIL_GRAPHIC_SCENE_CONTAINER_SCENECONTAINER_HPP

#include <Cocktail/Graphic/Scene/Scene.hpp>

namespace Ck
{
	class COCKTAIL_GRAPHIC_API SceneContainer
	{
	public:

		struct CameraInfo
		{
			struct PerspectiveInfo
			{
				Angle<float> FieldOfView;
				float AspectRatio = 0.f;
			};

			struct OrthographicInfo
			{
				
			};

			String Name;
			PerspectiveInfo Perspective;
			OrthographicInfo Orthographic;
			bool IsPerspective = false;
			Vector2<float> DepthBounds;
		};

		/**
		 * \brief 
		 * \return 
		 */
		const Array<CameraInfo>& GetCameras() const;

		/**
		 * \brief Add the content of the SceneContainer into an existing scene
		 * \param scene 
		 * \return 
		 */
		std::shared_ptr<SceneNode> AddToScene(Scene& scene);

	protected:

		struct MaterialInfo
		{
			AnsiString Name;
			Material::UniformColors Colors;
			Material::ShadingMode ShadingMode;
			EnumMap<Material::TextureType, std::shared_ptr<MipMaps>> Textures;
			bool DoubleSided = false;
			Material::AlphaMode AlphaMode = Material::AlphaMode::Opaque;
			float AlphaCutoff = 0.f;
		};

		struct SubMeshInfo
		{
			AnsiString Name;
			unsigned int Count;
			unsigned int FirstVertex;
			unsigned int FirstIndex;
			Renderer::PrimitiveTopology PrimitiveTopology = Renderer::PrimitiveTopology::Triangle;
			unsigned int MaterialIndex = 0;
		};

		struct MeshInfo
		{
			AnsiString Name;
			std::shared_ptr<VertexArray> Vertices;
			std::shared_ptr<IndexArray> Indices;
			Array<SubMeshInfo> SubMeshes;
		};

		struct NodeInfo
		{
			Array<NodeInfo> Children;
			Array<unsigned int> MeshIndices;
			Transformation LocalTransformation = Transformation::Identity();
			CameraInfo* Camera = nullptr;
		};

		SceneContainer();

		NodeInfo mRoot;
		Array<CameraInfo> mCameras;
		Array<MeshInfo> mMeshes;
		Array<std::shared_ptr<MipMaps>> mMipMaps;
		Array<MaterialInfo> mMaterials;

	private:

		/**
		 * \brief Create a material to be served a default one for shape that have none (like point or line shapes)
		 * \param baseColor The base color of the material to create
		 * \return The created material
		 */
		static MaterialInfo CreateDefaultMaterial(LinearColor baseColor);

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