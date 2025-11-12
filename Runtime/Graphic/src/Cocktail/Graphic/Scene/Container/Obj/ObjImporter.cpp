#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/IO/Input/Reader/BufferedReader.hpp>
#include <Cocktail/Core/IO/Input/Reader/InputStreamReader.hpp>
#include <Cocktail/Core/IO/Input/Stream/MemoryInputStream.hpp>
#include <Cocktail/Core/Log/Log.hpp>
#include <Cocktail/Core/Utility/FileUtils.hpp>
#include <Cocktail/Core/Utility/StringConvertion.hpp>

#include <Cocktail/Graphic/Scene/Container/Obj/ObjImporter.hpp>
#include <Cocktail/Graphic/Scene/Container/Obj/ObjSceneContainer.hpp>

namespace Ck
{
	COCKTAIL_DECLARE_EXCEPTION(ObjParseError);

	std::shared_ptr<SceneContainer> ObjImporter::LoadFromPath(const Path& path, const SceneImportParameters& parameters)
	{
		SceneImportParameters importParameters = parameters;
		importParameters.BaseDirectory = path.GetParent();

		tinyobj::ObjReaderConfig readerConfig;
		readerConfig.vertex_color = false;
		readerConfig.mtl_search_path = AnsiString::Convert(path.GetParent().ToString()).GetData();

		AnsiString p = AnsiString::Convert(path.ToString());
		CK_LOG(SceneLoaderLogCategory, LogLevel::Info, CK_TEXT("Loading scene from %s"), path.ToString());
		tinyobj::ObjReader reader;
		if (!reader.ParseFromFile(p.GetData(), readerConfig))
			throw ObjParseError(String::ConvertFrom<AsciiEncoder>(reader.Error().c_str()));

		return std::make_shared<ObjSceneContainer>(importParameters, reader.GetAttrib(), reader.GetShapes(), reader.GetMaterials());
	}

	std::shared_ptr<SceneContainer> ObjImporter::LoadFromStream(InputStream& inputStream, const SceneImportParameters& parameters)
	{
		tinyobj::ObjReaderConfig readerConfig;
		readerConfig.vertex_color = false;
		readerConfig.mtl_search_path = AnsiString::Convert(parameters.BaseDirectory.ToString()).GetData();

		AnsiString objText, mtlText;
		{
			Array<String> tokens;
			InputStreamReader inputStreamReader(inputStream);
			BufferedReader reader(inputStreamReader);
			while (!reader.IsEof())
			{
				String line = reader.ReadLine();
				if (line.IsEmpty())
					continue;

				line.Split(tokens, ' ');
				if (tokens[0] == CK_TEXT("mtllib"))
				{
					Path mtlFilePath = parameters.BaseDirectory;
					mtlFilePath = mtlFilePath.Join(tokens[1]);

					mtlText = AnsiString::Convert(FileUtils::ReadFile(mtlFilePath).ToString());
				}

				objText.Append(AnsiString::Convert(line));
			}
		}

		tinyobj::ObjReader reader;
		if (!reader.ParseFromString(objText.GetData(), mtlText.GetData(), readerConfig))
			throw ObjParseError(String::ConvertFrom<AsciiEncoder>(reader.Error().c_str()));

		return std::make_shared<ObjSceneContainer>(parameters, reader.GetAttrib(), reader.GetShapes(), reader.GetMaterials());
	}

	bool ObjImporter::SupportExtension(StringView extension) const
	{
		return extension == CK_TEXT(".obj");
	}

	bool ObjImporter::SupportParameters(const SceneImportParameters& parameters) const
	{
		return true;
	}
}
