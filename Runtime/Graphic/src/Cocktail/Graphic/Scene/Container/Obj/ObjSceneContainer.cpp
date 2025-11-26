#include <unordered_map>

#include <Cocktail/Core/Application/App.hpp>
#include <Cocktail/Core/Log/Log.hpp>
#include <Cocktail/Core/Image/ImageLoader.hpp>
#include <Cocktail/Core/System/FileSystem/Storage.hpp>

#include <Cocktail/Graphic/Material/MipMaps/MipMapsLoader.hpp>
#include <Cocktail/Graphic/Scene/SceneLoader.hpp>
#include <Cocktail/Graphic/Scene/Container/Obj/ObjSceneContainer.hpp>

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
			mMaterials.Add(
				ProcessMaterial(objMaterial)
			);
		}

		mMeshes.Add(
			ProcessMesh(attributes, shapes)
		);

		mRoot.MeshIndices = { 0 }; /// Obj format support only one mesh with many sub mesh
	}

	std::shared_ptr<MipMaps> ObjSceneContainer::LoadMipMaps(const Path& path)
	{
        if (path.IsEmpty())
            return nullptr;

		if (auto it = mMipMaps.find(path); it != mMipMaps.end())
			return it->second;

        ImageLoader* imageLoader = App::Resolve<ImageLoader>();
        MipMapsLoader* mipMapLoader = App::Resolve<MipMapsLoader>();

        String extension(path.GetExtension());
        Path fullpath = mImportParameters.BaseDirectory.Join(path);
        if (!Storage::IsFile(fullpath))
        {
            CK_LOG(SceneLoaderLogCategory, LogLevel::Error, CK_TEXT("File %s not found"), path.ToString());
            return nullptr;
        }

		std::shared_ptr<MipMaps> mipMaps;
        if (mipMapLoader->SupportExtensionImport(extension))
        {
			mipMaps = mipMapLoader->LoadFromPath(fullpath, {});
        }
        else if (imageLoader->SupportExtensionImport(extension))
        {
			std::shared_ptr<Image> image = imageLoader->LoadFromPath(fullpath, { ImageImportParameters::Format::RedGreenBlueAlpha });
			mipMaps = MipMaps::FromImage(*image);
        }
		else
		{
			CK_LOG(SceneLoaderLogCategory, LogLevel::Error, CK_TEXT("No loader found for file %s"), path.ToString());
			return nullptr;
		}

		mMipMaps[path] = mipMaps;

		return mipMaps;
    }

	ObjSceneContainer::MaterialInfo ObjSceneContainer::ProcessMaterial(const tinyobj::material_t& objMaterial)
	{
		MaterialInfo materialInfo;
		materialInfo.Name = objMaterial.name.c_str();
		materialInfo.ShadingMode = Material::ShadingMode::Phong;
		materialInfo.Colors.Base = { objMaterial.diffuse[0], objMaterial.diffuse[1], objMaterial.diffuse[2], objMaterial.dissolve };
		materialInfo.Colors.Specular = { objMaterial.specular[0], objMaterial.specular[1], objMaterial.specular[2], 1.f };
		materialInfo.Colors.Emission = { objMaterial.emission[0], objMaterial.emission[1], objMaterial.emission[2], 1.f };

		materialInfo.Textures[Material::TextureType::Ambient] = LoadMipMaps(String::ConvertFrom<Encoders::Ascii>(objMaterial.ambient_texname.c_str()));
		materialInfo.Textures[Material::TextureType::BaseColor] = LoadMipMaps(String::ConvertFrom<Encoders::Ascii>(objMaterial.diffuse_texname.c_str()));
		materialInfo.Textures[Material::TextureType::Specular] = LoadMipMaps(String::ConvertFrom<Encoders::Ascii>(objMaterial.specular_texname.c_str()));
		materialInfo.Textures[Material::TextureType::SpecularHighlight] = LoadMipMaps(String::ConvertFrom<Encoders::Ascii>(objMaterial.specular_highlight_texname.c_str()));
		materialInfo.Textures[Material::TextureType::Normal] = LoadMipMaps(String::ConvertFrom<Encoders::Ascii>(objMaterial.bump_texname.c_str()));
		materialInfo.Textures[Material::TextureType::Displacement] = LoadMipMaps(String::ConvertFrom<Encoders::Ascii>(objMaterial.displacement_texname.c_str()));
		materialInfo.Textures[Material::TextureType::Alpha] = LoadMipMaps(String::ConvertFrom<Encoders::Ascii>(objMaterial.alpha_texname.c_str()));
		materialInfo.Textures[Material::TextureType::Reflection] = LoadMipMaps(String::ConvertFrom<Encoders::Ascii>(objMaterial.reflection_texname.c_str()));
		materialInfo.Textures[Material::TextureType::Emission] = LoadMipMaps(String::ConvertFrom<Encoders::Ascii>(objMaterial.emissive_texname.c_str()));

		if (objMaterial.dissolve != 1.f)
		{
			materialInfo.AlphaMode = Material::AlphaMode::Blend;
		}
		else if (!objMaterial.alpha_texname.empty())
		{
			materialInfo.AlphaMode = Material::AlphaMode::Mask;
			materialInfo.AlphaCutoff = 0.5f; /// MTL format does not provide this value
		}
		else
		{
			materialInfo.AlphaMode = Material::AlphaMode::Opaque;
		}

		return materialInfo;
	}

	void ObjSceneContainer::ProcessIndices(ObjVertexCache& vertexCache, const tinyobj::attrib_t& attributes, const tinyobj::index_t* indices, std::size_t indicesCount) const
	{
		for (std::size_t i = 0; i < indicesCount; i++)
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

				vertex.TexCoord = texCoord;
				if (mImportParameters.FlipUpUV)
					vertex.TexCoord.Y() = 1.f - vertex.TexCoord.Y();
			}

			if (objVertexIndices.normal_index != -1)
			{
				Vector3<float> normal;
				for (std::size_t j = 0; j < normal.GetSize(); j++)
					normal.At(j) = attributes.normals[normal.GetSize() * static_cast<std::size_t>(objVertexIndices.normal_index) + j];
				vertex.Normal = normal;
			}

			vertexCache.AddVertex(vertex);
		}
	}

	bool ObjSceneContainer::ProcessSubMesh(ObjVertexCache& vertexCache, Array<SubMeshInfo>& subMeshes, const tinyobj::attrib_t& attributes, const tinyobj::shape_t& shape) const
	{
		SubMeshInfo subMesh;
		subMesh.Name = shape.name.c_str();

		if (!shape.mesh.indices.empty())
		{
			const std::vector<tinyobj::index_t>& indices = shape.mesh.indices;
			const std::vector<int>& materialIndexes = shape.mesh.material_ids;

			std::size_t subMeshStart = 0;
			for (std::size_t i = 0 ; i < indices.size(); i++)
			{
				std::size_t next = i + 1;
				int currentMaterialIndex = materialIndexes[i / 3];

				const bool isLast = next == indices.size();
				const bool nextMaterialChange = !isLast && materialIndexes[next / 3] != currentMaterialIndex;
				if (isLast || nextMaterialChange)
				{
					subMesh.Count = next - subMeshStart;
					subMesh.FirstVertex = 0;
					subMesh.FirstIndex = vertexCache.GetIndexCount();
					subMesh.MaterialIndex = currentMaterialIndex + 1;
					subMesh.PrimitiveTopology = Renderer::PrimitiveTopology::Triangle;

					ProcessIndices(vertexCache, attributes, &indices[subMeshStart], subMesh.Count);

					subMeshes.Add(subMesh);

					subMeshStart = next;
				}
			}

			return true;
		}
		else
		{
			const std::vector<tinyobj::index_t>* indices = nullptr;
			if (!shape.lines.indices.empty())
			{
				indices = &shape.lines.indices;

				subMesh.MaterialIndex = 0;
				subMesh.PrimitiveTopology = Renderer::PrimitiveTopology::Line;
			}
			else if (!shape.points.indices.empty())
			{
				indices = &shape.points.indices;

				subMesh.MaterialIndex = 0;
				subMesh.PrimitiveTopology = Renderer::PrimitiveTopology::Point;
			}

			if (!indices)
				return false;

			subMesh.Count = indices->size();
			subMesh.FirstVertex = 0;
			subMesh.FirstIndex = vertexCache.GetIndexCount();

			ProcessIndices(vertexCache, attributes, indices->data(), indices->size());

			subMeshes.Add(subMesh);

			return true;
		}
	}

	SceneContainer::MeshInfo ObjSceneContainer::ProcessMesh(const tinyobj::attrib_t& attributes, const std::vector<tinyobj::shape_t>& shapes)
	{
		ObjVertexCache vertexCache(mVertexLayout);

		Array<SubMeshInfo> subMeshes;
		subMeshes.Reserve(shapes.size());
		for (const tinyobj::shape_t& shape : shapes)
		{
			bool processed = ProcessSubMesh(vertexCache, subMeshes, attributes, shape);
			if (!processed)
				CK_LOG(SceneLoaderLogCategory, LogLevel::Error, CK_TEXT("Shape %s is invalid and will be ignored"), shape.name.c_str());
		}

		MeshInfo meshInfo;
		meshInfo.Vertices = vertexCache.CreateVertexArray();
		meshInfo.Indices = vertexCache.CreateIndexArray();
		meshInfo.SubMeshes = std::move(subMeshes);

		return meshInfo;
	}
}
