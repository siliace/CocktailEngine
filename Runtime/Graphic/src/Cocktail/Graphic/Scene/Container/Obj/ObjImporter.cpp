#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/IO/Input/Reader/BufferedReader.hpp>
#include <Cocktail/Core/IO/Input/Reader/InputStreamReader.hpp>
#include <Cocktail/Core/IO/Input/Stream/MemoryInputStream.hpp>
#include <Cocktail/Core/Utility/FileUtils.hpp>
#include <Cocktail/Core/Utility/StringUtils.hpp>

#include <Cocktail/Graphic/Scene/Container/Obj/ObjImporter.hpp>
#include <Cocktail/Graphic/Scene/Container/Obj/ObjSceneContainer.hpp>

namespace Ck
{
	COCKTAIL_DECLARE_EXCEPTION_BASE(ObjParseError, "Failed to parse obj file", std::runtime_error);

	Ref<SceneContainer> ObjImporter::LoadFromPath(const std::filesystem::path& path, const SceneImportParameters& parameters)
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

	Ref<SceneContainer> ObjImporter::LoadFromStream(InputStream& inputStream, const SceneImportParameters& parameters)
	{
		tinyobj::ObjReaderConfig readerConfig;
		readerConfig.vertex_color = false;
		readerConfig.mtl_search_path = parameters.BaseDirectory.string();

		std::string objText, mtlText;
		{
			std::vector<std::string> tokens;
			InputStreamReader inputStreamReader(inputStream);
			BufferedReader reader(inputStreamReader);
			while (!reader.IsEof())
			{
				std::string line = reader.ReadLine();
				if (line.empty())
					continue;

				StringUtils::Split(tokens, line, ' ');
				if (tokens[0] == "mtllib")
					mtlText = FileUtils::ReadFile(parameters.BaseDirectory / tokens[1]).ToString();

				objText.append(line);
			}
		}

		tinyobj::ObjReader reader;
		if (!reader.ParseFromString(objText, mtlText, readerConfig))
			throw ObjParseError(reader.Error());

		return ObjSceneContainer::New(parameters, reader.GetAttrib(), reader.GetShapes(), reader.GetMaterials());
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
