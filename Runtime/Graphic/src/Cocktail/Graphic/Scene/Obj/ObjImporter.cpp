#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <Cocktail/Core/Log/Log.hpp>

#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/Application/App.hpp>
#include <Cocktail/Core/Image/ImageLoader.hpp>
#include <Cocktail/Core/IO/Input/Reader/BufferedReader.hpp>
#include <Cocktail/Core/IO/Input/Reader/InputStreamReader.hpp>
#include <Cocktail/Core/IO/Input/Stream/MemoryInputStream.hpp>
#include <Cocktail/Core/System/FileSystem/Storage.hpp>
#include <Cocktail/Core/Utility/FileUtils.hpp>
#include <Cocktail/Core/Utility/StringUtils.hpp>

#include <Cocktail/Graphic/Material/MipMaps/MipMapsLoader.hpp>
#include <Cocktail/Graphic/Scene/Obj/ObjImporter.hpp>

namespace Ck
{
	COCKTAIL_DECLARE_EXCEPTION_BASE(ObjParseError, "Failed to parse obj file", std::runtime_error);

	class ObjSceneContainer : public Extends<ObjSceneContainer, SceneContainer>
	{
	public:

		ObjSceneContainer(const SceneImportParameters& importParameters, const tinyobj::attrib_t& attributes, const std::vector<tinyobj::shape_t>& shapes, const std::vector<tinyobj::material_t>& materials) :
			mImportParameters(importParameters)
		{

			Ref<VertexLayout> vertexLayout = VertexLayout::New();
			vertexLayout->CreateAttribute(VertexAttributeSemantic::Position, DataType::Float32, 3);
			vertexLayout->CreateAttribute(VertexAttributeSemantic::TexCoord, DataType::Float32, 2);
			vertexLayout->CreateAttribute(VertexAttributeSemantic::Normal, DataType::Float32, 3);

			for (const tinyobj::material_t& objMaterial : materials)
			{
				MaterialInfo materialInfo;
				materialInfo.Name = objMaterial.name;
				materialInfo.ShadingMode = Material::ShadingMode::Phong;
				materialInfo.Colors.Base = { objMaterial.diffuse[0] * 255.f, objMaterial.diffuse[1] * 255.f, objMaterial.diffuse[2] * 255.f, 255.f };
				materialInfo.Colors.Specular = { objMaterial.specular[0] * 255.f, objMaterial.specular[1] * 255.f, objMaterial.specular[2] * 255.f, 255.f };
				materialInfo.Colors.Emission = { objMaterial.emission[0] * 255.f, objMaterial.emission[1] * 255.f, objMaterial.emission[2] * 255.f, 255.f };
				materialInfo.Textures[Material::TextureType::Ambient] = LoadMipMaps(objMaterial.ambient_texname);
				materialInfo.Textures[Material::TextureType::BaseColor] = LoadMipMaps(objMaterial.diffuse_texname);
				materialInfo.Textures[Material::TextureType::Specular] = LoadMipMaps(objMaterial.specular_texname);
				materialInfo.Textures[Material::TextureType::SpecularHighlight] = LoadMipMaps(objMaterial.specular_highlight_texname);
				materialInfo.Textures[Material::TextureType::Bump] = LoadMipMaps(objMaterial.bump_texname);
				materialInfo.Textures[Material::TextureType::Displacement] = LoadMipMaps(objMaterial.displacement_texname);
				materialInfo.Textures[Material::TextureType::Alpha] = LoadMipMaps(objMaterial.alpha_texname);
				materialInfo.Textures[Material::TextureType::Reflection] = LoadMipMaps(objMaterial.reflection_texname);
				materialInfo.Textures[Material::TextureType::Emission] = LoadMipMaps(objMaterial.emissive_texname);

				mMaterials.push_back(materialInfo);
			}

			struct ObjVertex
			{
				Vector3<float> Position;
				Vector2<float> TexCoord;
				Vector3<float> Normal;
			};

			struct ObjVertexHasher
			{
				std::size_t operator()(const ObjVertex& objVertex) const noexcept
				{
					std::size_t positionHash = 0;
					for (auto i = 0; i < objVertex.Position.GetSize(); i++)
						HashCombine(positionHash, objVertex.Position.At(i));

					std::size_t texCoordHash = 0;
					for (auto i = 0; i < objVertex.TexCoord.GetSize(); i++)
						HashCombine(texCoordHash, objVertex.TexCoord.At(i));

					std::size_t normalHash = 0;
					for (auto i = 0; i < objVertex.Normal.GetSize(); i++)
						HashCombine(normalHash, objVertex.Normal.At(i));

					return HashMerge(positionHash, HashMerge(texCoordHash, normalHash));
				}
			};

			struct ObjVertexComparator
			{
				bool operator()(const ObjVertex& lhs, const ObjVertex& rhs) const noexcept
				{
					return std::tie(lhs.Position, lhs.TexCoord, lhs.Normal) == std::tie(rhs.Position, rhs.TexCoord, rhs.Normal);
				}
			};

			for (const tinyobj::shape_t& objShape : shapes)
			{
				const tinyobj::mesh_t& objMesh = objShape.mesh;

				unsigned int currentIndex = 0;
				std::vector<unsigned int> vertexIndices;
				vertexIndices.reserve(objMesh.indices.size());

				std::unordered_map<ObjVertex, unsigned int, ObjVertexHasher, ObjVertexComparator> vertexCache;
				vertexCache.reserve(objMesh.indices.size());

				for (std::size_t i = 0; i < objMesh.indices.size(); i++)
				{
					ObjVertex vertex;
					const tinyobj::index_t& objVertexIndices = objMesh.indices[i];

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
					}

					if (objVertexIndices.normal_index != -1)
					{
						Vector3<float> normal;
						for (std::size_t j = 0; j < normal.GetSize(); j++)
							normal.At(j) = attributes.normals[normal.GetSize() * static_cast<std::size_t>(objVertexIndices.normal_index) + j];

						vertex.Normal = normal;
					}

					auto [it, inserted]  = vertexCache.insert({ vertex, currentIndex });
					if (inserted)
						++currentIndex;

					vertexIndices.push_back(it->second);
				}

				Ref<VertexArray> vertices = VertexArray::New(vertexLayout, vertexCache.size());
				for (const auto& [vertex, index] : vertexCache)
				{
					VertexRef vertexRef = vertices->At(index);
					vertexRef.Set(VertexAttributeSemantic::Position, vertex.Position);
					vertexRef.Set(VertexAttributeSemantic::TexCoord, vertex.TexCoord);
					vertexRef.Set(VertexAttributeSemantic::Normal, vertex.Normal);
				}

				const bool useShortIndices = vertexCache.size() < std::numeric_limits<Uint16>::max();
				const Renderer::IndexType indexType = useShortIndices ? Renderer::IndexType::Short : Renderer::IndexType::Integer;
				Ref<IndexArray> indices = IndexArray::New(indexType, vertexIndices.size());
				for (std::size_t i = 0; i < vertexIndices.size(); i++)
				{
					IndexRef indexRef = indices->At(i);
					unsigned int value = vertexIndices[i];
					if (useShortIndices)
					{
						indexRef.Set<Uint16>(static_cast<Uint16>(value));

					}
					else
					{
						indexRef.Set<Uint32>(value);
					}
				}

				MeshInfo meshInfo;
				meshInfo.Vertices = std::move(vertices);
				meshInfo.Indices = std::move(indices);
				meshInfo.PrimitiveTopology = Renderer::PrimitiveTopology::Triangle;
				meshInfo.MaterialIndex = objMesh.material_ids.front();
				mMeshes.push_back(meshInfo);
			}

			for (const MeshInfo& meshInfo : mMeshes)
				mRoot.Meshes.push_back(&meshInfo);
		}

	private:

		Ref<MipMaps> LoadMipMaps(const std::filesystem::path& path)
		{
			if (path.empty())
				return nullptr;

			return mMipMaps.GetOrCreate(path, [&]() -> Ref<MipMaps> {
				Ref<ImageLoader> imageLoader = App::Resolve<ImageLoader>();
				Ref<MipMapsLoader> mipMapLoader = App::Resolve<MipMapsLoader>();

				std::string extension = path.extension().string();
				std::filesystem::path fullpath = mImportParameters.BaseDirectory / path;
				if (!Storage::IsFile(fullpath))
				{
					Log::Error("File {} not found", path.string());
					return nullptr;
				}

				if (mipMapLoader->SupportExtensionImport(extension))
					return mipMapLoader->LoadFromPath(fullpath, {});

				if (imageLoader->SupportExtensionImport(extension))
				{
					Ref<Image> image = imageLoader->LoadFromPath(fullpath, { ImageImportParameters::Format::RedGreenBlueAlpha });
					return MipMaps::FromImage(*image, true);
				}

				Log::Error("No loader found for file {}", path.string());
				return nullptr;
			});
		}

		SceneImportParameters mImportParameters;
		CachePool<std::filesystem::path, Ref<MipMaps>> mMipMaps;
	};

	Ref<SceneContainer> ObjImporter::LoadFromPath(const std::filesystem::path& path, const SceneImportParameters& parameters) const
	{
		SceneImportParameters importParameters = parameters;
		importParameters.BaseDirectory = path.parent_path();

		tinyobj::ObjReaderConfig readerConfig;
		readerConfig.vertex_color = false;
		readerConfig.mtl_search_path = path.parent_path().string();

		tinyobj::ObjReader reader;
		if (!reader.ParseFromFile(path.string(), readerConfig))
			throw ObjParseError(reader.Error());

		return ObjSceneContainer::New(importParameters, reader.GetAttrib(), reader.GetShapes(), reader.GetMaterials());
	}

	Ref<SceneContainer> ObjImporter::LoadFromStream(const Ref<InputStream>& inputStream, const SceneImportParameters& parameters) const
	{
		tinyobj::ObjReaderConfig readerConfig;
		readerConfig.vertex_color = false;
		readerConfig.mtl_search_path = parameters.BaseDirectory.string();

		std::string objText, mtlText;
		{
			std::vector<std::string> tokens;
			Ref<BufferedReader> reader = BufferedReader::New(InputStreamReader::New(inputStream));
			while (!reader->IsEof())
			{
				std::string line = reader->ReadLine();
				if (line.empty())
					continue;

				StringUtils::Split(tokens, line, ' ');
				if (tokens[0] == "mtllib")
					mtlText = FileUtils::ReadFile(parameters.BaseDirectory / tokens[1]);

				objText.append(line);
			}
		}

		tinyobj::ObjReader reader;
		if (!reader.ParseFromString(objText, mtlText, readerConfig))
			throw ObjParseError(reader.Error());

		return ObjSceneContainer::New(parameters, reader.GetAttrib(), reader.GetShapes(), reader.GetMaterials());
	}

	Ref<SceneContainer> ObjImporter::LoadFromMemory(const void* buffer, std::size_t length, const SceneImportParameters& parameters) const
	{
		return LoadFromStream(
			MemoryInputStream::New(buffer, length), parameters
		);
	}

	bool ObjImporter::SupportExtension(std::string_view extension) const
	{
		return extension == ".obj";
	}

	bool ObjImporter::SupportParameters(const SceneImportParameters& parameters) const
	{
		return true;
	}
}
