#include <unordered_map>

#include <Cocktail/Core/Application/App.hpp>
#include <Cocktail/Core/Log/Log.hpp>
#include <Cocktail/Core/Image/ImageLoader.hpp>
#include <Cocktail/Core/System/FileSystem/Storage.hpp>

#include <Cocktail/Graphic/Material/MipMaps/MipMapsLoader.hpp>
#include <Cocktail/Graphic/Scene/SceneLoader.hpp>
#include <Cocktail/Graphic/Scene/Obj/ObjSceneContainer.hpp>

namespace Ck
{
	ObjSceneContainer::ObjSceneContainer(const SceneImportParameters& importParameters, const tinyobj::attrib_t& attributes, const std::vector<tinyobj::shape_t>& shapes, const std::vector<tinyobj::material_t>& materials) :
		mImportParameters(importParameters)
	{
		VertexLayout::Builder vertexLayoutBuilder;
		vertexLayoutBuilder.AddAttribute(VertexAttributeSemantic::Position, DataType::Float32, 3);
		vertexLayoutBuilder.AddAttribute(VertexAttributeSemantic::TexCoord, DataType::Float32, 2);
		vertexLayoutBuilder.AddAttribute(VertexAttributeSemantic::Normal, DataType::Float32, 3);
		mVertexLayout = vertexLayoutBuilder.Get();

		for (const tinyobj::material_t& objMaterial : materials)
		{
			mMaterials.push_back(
				ProcessMaterial(objMaterial)
			);
		}

		std::vector<Vector3<float>> meshTranslations;
		for (const tinyobj::shape_t& objShape : shapes)
		{
			mMeshes.push_back(
				ProcessShape(attributes, objShape)
			);
		}

		for (const MeshInfo& meshInfo : mMeshes)
			mRoot.Meshes.push_back(&meshInfo);
	}

	Ref<MipMaps> ObjSceneContainer::LoadMipMaps(const std::filesystem::path& path)
	{
        if (path.empty())
            return nullptr;

		if (auto it = mMipMaps.find(path); it != mMipMaps.end())
			return it->second;

        Ref<ImageLoader> imageLoader = App::Resolve<ImageLoader>();
        Ref<MipMapsLoader> mipMapLoader = App::Resolve<MipMapsLoader>();

        std::string extension = path.extension().string();
        std::filesystem::path fullpath = mImportParameters.BaseDirectory / path;
        if (!Storage::IsFile(fullpath))
        {
            Log::Error("File {} not found", path.string());
            return nullptr;
        }

		Ref<MipMaps> mipMaps;
        if (mipMapLoader->SupportExtensionImport(extension))
        {
			mipMaps = mipMapLoader->LoadFromPath(fullpath, {});
        }
        else if (imageLoader->SupportExtensionImport(extension))
        {
            Ref<Image> image = imageLoader->LoadFromPath(fullpath, { ImageImportParameters::Format::RedGreenBlueAlpha });
			mipMaps = MipMaps::FromImage(*image);
        }
		else
		{
			Log::Error("No loader found for file {}", path.string());
			return nullptr;
		}

		mMipMaps[path] = mipMaps;

		return mipMaps;
    }

	ObjSceneContainer::MaterialInfo ObjSceneContainer::ProcessMaterial(const tinyobj::material_t& objMaterial)
	{
		MaterialInfo materialInfo;
		materialInfo.Name = objMaterial.name;
		materialInfo.ShadingMode = Material::ShadingMode::Phong;
		materialInfo.Colors.Base = { objMaterial.diffuse[0] * 255.f, objMaterial.diffuse[1] * 255.f, objMaterial.diffuse[2] * 255.f, 255.f };
		materialInfo.Colors.Specular = { objMaterial.specular[0] * 255.f, objMaterial.specular[1] * 255.f, objMaterial.specular[2] * 255.f };
		materialInfo.Colors.Emission = { objMaterial.emission[0] * 255.f, objMaterial.emission[1] * 255.f, objMaterial.emission[2] * 255.f };
		materialInfo.Textures[Material::TextureType::Ambient] = LoadMipMaps(objMaterial.ambient_texname);
		materialInfo.Textures[Material::TextureType::BaseColor] = LoadMipMaps(objMaterial.diffuse_texname);
		materialInfo.Textures[Material::TextureType::Specular] = LoadMipMaps(objMaterial.specular_texname);
		materialInfo.Textures[Material::TextureType::SpecularHighlight] = LoadMipMaps(objMaterial.specular_highlight_texname);
		materialInfo.Textures[Material::TextureType::Bump] = LoadMipMaps(objMaterial.bump_texname);
		materialInfo.Textures[Material::TextureType::Displacement] = LoadMipMaps(objMaterial.displacement_texname);
		materialInfo.Textures[Material::TextureType::Alpha] = LoadMipMaps(objMaterial.alpha_texname);
		materialInfo.Textures[Material::TextureType::Reflection] = LoadMipMaps(objMaterial.reflection_texname);
		materialInfo.Textures[Material::TextureType::Emission] = LoadMipMaps(objMaterial.emissive_texname);
		materialInfo.Opaque = objMaterial.dissolve == 1.f;

		return materialInfo;
	}

	Ref<ObjVertexCache> ObjSceneContainer::CreateVertexCache(const tinyobj::attrib_t& attributes, const std::vector<tinyobj::index_t>& indices) const
	{
		Ref<ObjVertexCache> vertexCache = ObjVertexCache::New(mVertexLayout, indices.size());

		for (std::size_t i = 0; i < indices.size(); i++)
		{
			ObjVertex vertex;
			const tinyobj::index_t& objVertexIndices = indices[i];
	
			Vector3<float> position;
			for (std::size_t j = 0; j < position.GetSize(); j++)
				position.At(j) = attributes.vertices[position.GetSize() * static_cast<std::size_t>(objVertexIndices.vertex_index) + j];
	
			vertex.Position = position;
	
			if (objVertexIndices.texcoord_index != -1)
			{
				Vector2<float> texCoord;
				for (std::size_t j = 0; j < texCoord.GetSize(); j++)
					texCoord.At(j) = attributes.texcoords[texCoord.GetSize() * static_cast<std::size_t>(objVertexIndices.texcoord_index) + j];

				if (mImportParameters.FlipUp)
					vertex.TexCoord.Y() = 1.f - vertex.TexCoord.Y();

				vertex.TexCoord = texCoord;
			}

			if (objVertexIndices.normal_index != -1)
			{
				Vector3<float> normal;
				for (std::size_t j = 0; j < normal.GetSize(); j++)
					normal.At(j) = attributes.normals[normal.GetSize() * static_cast<std::size_t>(objVertexIndices.normal_index) + j];
				vertex.Normal = normal;
			}
			
			vertexCache->AddVertex(vertex);
		}

		return vertexCache;
	}

	ObjSceneContainer::MeshInfo ObjSceneContainer::ProcessShape(const tinyobj::attrib_t& attributes, const tinyobj::shape_t& objShape)
	{
		MeshInfo meshInfo;
		Ref<ObjVertexCache> vertexCache;
		if (const tinyobj::mesh_t& objMesh = objShape.mesh; !objMesh.indices.empty())
		{
			vertexCache = CreateVertexCache(attributes, objMesh.indices);
			meshInfo.PrimitiveTopology = Renderer::PrimitiveTopology::Triangle;
			meshInfo.MaterialIndex = objMesh.material_ids.front(); /// Todo: test if mesh use only one material
		}

		meshInfo.Vertices = vertexCache->CreateVertexArray();
		meshInfo.Indices = vertexCache->CreateIndexArray();

		return meshInfo;
	}
}