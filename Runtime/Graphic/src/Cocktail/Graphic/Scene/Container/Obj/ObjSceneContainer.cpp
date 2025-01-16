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
	SceneContainer::MaterialInfo ObjSceneContainer::CreateDefaultMaterial(LinearColor baseColor)
	{
		MaterialInfo material;
		material.Name = "ObjDefault";
		material.ShadingMode = Material::ShadingMode::Unlit;
		material.Colors.Base = baseColor;
		material.DoubleSided = true;
		material.Opaque = true;

		return material;
	}

	ObjSceneContainer::ObjSceneContainer(const SceneImportParameters& importParameters, const tinyobj::attrib_t& attributes, const std::vector<tinyobj::shape_t>& shapes, const std::vector<tinyobj::material_t>& materials) :
		mImportParameters(importParameters)
	{
		VertexLayout::Builder vertexLayoutBuilder;
		vertexLayoutBuilder.AddAttribute(VertexAttributeSemantic::Position, DataType::Float32, 3);
		vertexLayoutBuilder.AddAttribute(VertexAttributeSemantic::TexCoord, DataType::Float32, 2);
		vertexLayoutBuilder.AddAttribute(VertexAttributeSemantic::Normal, DataType::Float32, 3);
		mVertexLayout = vertexLayoutBuilder.Get();

		mMaterials.push_back(CreateDefaultMaterial(LinearColor(0.f, 0.f, 0.f)));
		for (const tinyobj::material_t& objMaterial : materials)
		{
			mMaterials.push_back(
				ProcessMaterial(objMaterial)
			);
		}

		mMeshes.push_back(
			ProcessMesh(attributes, shapes)
		);

		mRoot.MeshIndices = { 0 }; /// Obj format support only one mesh with many sub mesh
	}

	Ref<MipMaps> ObjSceneContainer::LoadMipMaps(const std::filesystem::path& path)
	{
        if (path.empty())
            return nullptr;

		if (auto it = mMipMaps.find(path); it != mMipMaps.end())
			return it->second;

        ImageLoader* imageLoader = App::Resolve<ImageLoader>();
        MipMapsLoader* mipMapLoader = App::Resolve<MipMapsLoader>();

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
		materialInfo.Colors.Base = { objMaterial.diffuse[0], objMaterial.diffuse[1], objMaterial.diffuse[2], 1.f };
		materialInfo.Colors.Specular = { objMaterial.specular[0], objMaterial.specular[1], objMaterial.specular[2], 1.f };
		materialInfo.Colors.Emission = { objMaterial.emission[0], objMaterial.emission[1], objMaterial.emission[2], 1.f };
		materialInfo.Textures[Material::TextureType::Ambient] = LoadMipMaps(objMaterial.ambient_texname);
		materialInfo.Textures[Material::TextureType::BaseColor] = LoadMipMaps(objMaterial.diffuse_texname);
		materialInfo.Textures[Material::TextureType::Specular] = LoadMipMaps(objMaterial.specular_texname);
		materialInfo.Textures[Material::TextureType::SpecularHighlight] = LoadMipMaps(objMaterial.specular_highlight_texname);
		materialInfo.Textures[Material::TextureType::Bump] = LoadMipMaps(objMaterial.bump_texname);
		materialInfo.Textures[Material::TextureType::Displacement] = LoadMipMaps(objMaterial.displacement_texname);
		materialInfo.Textures[Material::TextureType::Alpha] = LoadMipMaps(objMaterial.alpha_texname);
		materialInfo.Textures[Material::TextureType::Reflection] = LoadMipMaps(objMaterial.reflection_texname);
		materialInfo.Textures[Material::TextureType::Emission] = LoadMipMaps(objMaterial.emissive_texname);
		materialInfo.Opaque = objMaterial.dissolve == 1.f && objMaterial.alpha_texname.empty();

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

	bool ObjSceneContainer::ProcessSubMesh(ObjVertexCache& vertexCache, std::vector<SubMeshInfo>& subMeshes, const tinyobj::attrib_t& attributes, const tinyobj::shape_t& shape)
	{
		SubMeshInfo subMesh;
		subMesh.Name = shape.name;

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

					subMeshes.push_back(subMesh);

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

			subMeshes.push_back(subMesh);

			return true;
		}
	}

	SceneContainer::MeshInfo ObjSceneContainer::ProcessMesh(const tinyobj::attrib_t& attributes, const std::vector<tinyobj::shape_t>& shapes)
	{
		ObjVertexCache vertexCache(mVertexLayout);

		std::vector<SubMeshInfo> subMeshes;
		subMeshes.reserve(shapes.size());
		for (const tinyobj::shape_t& shape : shapes)
		{
			bool processed = ProcessSubMesh(vertexCache, subMeshes, attributes, shape);
			if (!processed)
				Log::Error("Shape {} is invalid and will be ignored", shape.name);
		}

		MeshInfo meshInfo;
		meshInfo.Vertices = vertexCache.CreateVertexArray();
		meshInfo.Indices = vertexCache.CreateIndexArray();
		meshInfo.SubMeshes = std::move(subMeshes);

		return meshInfo;
	}
}
