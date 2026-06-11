#ifndef COCKTAIL_GRAPHIC_SCENE_CONTAINER_SCENECONTAINER_HPP
#define COCKTAIL_GRAPHIC_SCENE_CONTAINER_SCENECONTAINER_HPP

#include <Cocktail/Graphic/Scene/Scene.hpp>
#include <Cocktail/Graphic/Scene/Light/Light.hpp>

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
				float HorizontalMagnification;
				float VerticalMagnification;
			};

			String Name;
			PerspectiveInfo Perspective;
			OrthographicInfo Orthographic;
			bool IsPerspective = false;
			Vector2<float> DepthBounds;
		};

	    struct LightInfo
	    {
	        struct SpotInfo
	        {
	            Angle<float> InnerConeAngle;
	            Angle<float> OuterConeAngle;
	        };

	        String Name;
	        LinearColor Color;
	        float Intensity = 100.f;
	        float Range = 0.f;
	        Light::Type Type;
	        SpotInfo Spot;
	    };

		/**
		 * \brief 
		 * \return 
		 */
		const Array<CameraInfo>& GetCameras() const;

	    /**
         * \brief
         * \return
         */
	    const Array<LightInfo>& GetLights() const;

        /**
         * \brief Add the content of the SceneContainer into an existing scene
         *
         * \param scene
         *
         * \return
         */
		SceneNode* AddToScene(Scene& scene);

	protected:

		struct MaterialInfo
		{
			AsciiString Name;
			Material::Parameters Parameters;
			Material::ShadingMode ShadingMode;
			EnumMap<Material::TextureType, SharedPtr<MipMaps>> Textures;
			bool DoubleSided = false;
			Material::AlphaMode AlphaMode = Material::AlphaMode::Opaque;
			float AlphaCutoff = 0.f;
		};

		struct SubMeshInfo
		{
			AsciiString Name;
			unsigned int Count;
			unsigned int FirstVertex;
			unsigned int FirstIndex;
			Renderer::PrimitiveTopology PrimitiveTopology = Renderer::PrimitiveTopology::Triangle;
			unsigned int MaterialIndex = 0;
		};

		struct MeshInfo
		{
			AsciiString Name;
			SharedPtr<VertexArray> Vertices;
			SharedPtr<IndexArray> Indices;
			Array<SubMeshInfo> SubMeshes;
		};

		struct NodeInfo
		{
			Array<NodeInfo> Children;
			Array<unsigned int> MeshIndices;
			Transformation LocalTransformation = Transformation::Identity();
			CameraInfo* Camera = nullptr;
		    LightInfo* Light = nullptr;
		};

		SceneContainer();

		NodeInfo mRoot;
		Array<CameraInfo> mCameras;
		Array<MeshInfo> mMeshes;
		Array<SharedPtr<MipMaps>> mMipMaps;
		Array<MaterialInfo> mMaterials;
	    Array<LightInfo> mLights;

	private:

		/**
		 * \brief Create a material to be served a default one for shape that have none (like point or line shapes)
		 *
		 * \param baseColor The base color of the material to create
		 *
		 * \return The created material
		 */
		static MaterialInfo CreateDefaultMaterial(LinearColor baseColor);

		/**
		 * \brief
		 *
		 * \param scene 
		 * \param parent 
		 * \param nodeInfo
		 * \param meshes 
		 * \param materials
		 *
		 * \return 
		 */
		SceneNode* ProcessNode(Scene& scene, SceneNode* parent, NodeInfo& nodeInfo, const Array<SharedPtr<Mesh>>& meshes, const Array<SharedPtr<Material>>& materials);
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_CONTAINER_SCENECONTAINER_HPP