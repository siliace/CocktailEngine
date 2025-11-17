#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/IO/Input/Reader/BufferedReader.hpp>
#include <Cocktail/Core/IO/Input/Reader/InputStreamReader.hpp>
#include <Cocktail/Core/IO/Input/Reader/LineReader.hpp>
#include <Cocktail/Core/IO/Input/Stream/MemoryInputStream.hpp>
#include <Cocktail/Core/Log/Log.hpp>
#include <Cocktail/Core/Utility/FileUtils.hpp>

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
		readerConfig.mtl_search_path = reinterpret_cast<const char*>(Utf8String::Convert(importParameters.BaseDirectory.ToString()).GetData());

		Utf8String p = Utf8String::Convert(path.ToString());
		CK_LOG(SceneLoaderLogCategory, LogLevel::Info, CK_TEXT("Loading scene from %s"), path.ToString());
		tinyobj::ObjReader reader;
		if (!reader.ParseFromFile(reinterpret_cast<const char*>(p.GetData()), readerConfig))
			throw ObjParseError(String::ConvertFrom<Encoders::Ascii>(reader.Error().c_str()));

		return std::make_shared<ObjSceneContainer>(importParameters, reader.GetAttrib(), reader.GetShapes(), reader.GetMaterials());
	}

	std::shared_ptr<SceneContainer> ObjImporter::LoadFromStream(InputStream<>& inputStream, const SceneImportParameters& parameters)
	{
		tinyobj::ObjReaderConfig readerConfig;
		readerConfig.vertex_color = false;
		readerConfig.mtl_search_path = reinterpret_cast<const char*>(Utf8String::Convert(parameters.BaseDirectory.ToString()).GetData());

		AsciiString objText, mtlText;
		{
			Array<AsciiString> tokens;
			InputStreamReader<Encoders::Ascii> inputStreamReader(inputStream);
			BufferedReader bufferedReader(inputStreamReader);

		    AsciiString line;
			LineReader lineReader(bufferedReader);
			while (lineReader.ReadLine(line))
			{
				if (line.IsEmpty())
					continue;

				line.Split(tokens, ' ');
				if (tokens[0] == "mtllib")
				{
					Path mtlFilePath = Path::Merge(parameters.BaseDirectory, String::Convert(tokens[1]));
					mtlText = Encoders::GetString<Encoders::Utf8, AsciiString>(FileUtils::ReadFile(mtlFilePath));
				}

				objText.Append(line).Append('\n');
			}
		}

		tinyobj::ObjReader reader;
		if (!reader.ParseFromString(objText.GetData(), mtlText.GetData(), readerConfig))
			throw ObjParseError(String::ConvertFrom<Encoders::Ascii>(reader.Error().c_str()));

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
