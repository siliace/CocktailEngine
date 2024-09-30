#include <Cocktail/Core/Log/Log.hpp>
#include <Cocktail/Core/System/FileSystem/Storage.hpp>
#include <Cocktail/Core/Utility/FileUtils.hpp>
#include <Cocktail/Core/Utility/PropertyTree/Xml/XmlProperties.hpp>

#include <Cocktail/ShaderCompiler/Project.hpp>

namespace Ck::Tools::ShaderCompiler
{
	Project::SourceFile Project::SourceFile::LoadFromPath(std::filesystem::path path, Renderer::ShaderType stage, Language language)
	{
		std::string fileContent = FileUtils::ReadFile(path);
		return SourceFile(path.stem().string(), std::move(fileContent), path.parent_path(), stage, language);
	}

	Project::SourceFile::SourceFile(std::string name, std::string content, std::filesystem::path location,
		Renderer::ShaderType stage, Language language):
		mName(std::move(name)),
		mContent(std::move(content)),
		mLocation(std::move(location)),
		mStage(stage),
		mLanguage(language)
	{
		/// Nothing
	}

	Ref<ShaderObject> Project::SourceFile::Compile(Compiler& compiler) const
	{
		ShaderSource sources[] = {
			{ mContent, mName }
		};

		ShaderObjectCreateInfo shaderObjectCreateInfo;
		shaderObjectCreateInfo.Stage = mStage;
		shaderObjectCreateInfo.Language = mLanguage;
		shaderObjectCreateInfo.SourceCount = 1;
		shaderObjectCreateInfo.Sources = sources;
		shaderObjectCreateInfo.Location = mLocation;
		Ref<ShaderObject> shaderObject = compiler.CreateShaderObject(shaderObjectCreateInfo);

		shaderObject->Compile();

		return shaderObject;
	}

	Project Project::LoadFromPath(const std::filesystem::path& path)
	{
		XmlProperties properties(path);
		std::filesystem::path projectDirectory = path.parent_path();

		Renderer::ShaderProgramType programType = properties.Get<Renderer::ShaderProgramType>("ShaderProject.type");
		Project project(programType);

		const XmlProperties::Sequence& compileElements = properties.GetSequence("ShaderProject.Compile");
		for (std::size_t i = 0; i < compileElements.GetSize(); i++)
		{
			const XmlProperties::Element& compileElement = compileElements.ElementAt(i);
			std::filesystem::path sourcePath = projectDirectory / compileElement.Get<std::string>("path");
			if (!Storage::IsFile(sourcePath))
			{
				Log::Error("Path attribute ({}) from Compile element does not exists", sourcePath.string());
				continue;
			}

			Renderer::ShaderType type = compileElement.Get<Renderer::ShaderType>("type");
			Language language = compileElement.Get<Language>("language");

			project.AddSourceFile(
				SourceFile::LoadFromPath(std::move(sourcePath), type, language)
			);
		}

		return project;
	}

	Project::Project(Renderer::ShaderProgramType programType):
		mProgramType(programType)
	{
		/// Nothing
	}

	void Project::AddSourceFile(const SourceFile& sourceFile)
	{
		mSourceFiles.push_back(sourceFile);
	}

	EnumMap<Renderer::ShaderType, std::vector<Uint32>> Project::Build(Compiler& compiler)
	{
		Ref<Program> program = compiler.CreateProgram(mProgramType);

		for (const SourceFile& sourceFile : mSourceFiles)
			program->AddShaderObject(sourceFile.Compile(compiler));

		program->Link();

		EnumMap<Renderer::ShaderType, std::vector<Uint32>> result;
		for (Renderer::ShaderType stage : Enum<Renderer::ShaderType>::Values)
		{
			if (!Renderer::ProgramSupportShader(mProgramType, stage))
				continue;

			result[stage] = program->GetStageBinary(stage);
		}

		return result;
	}
}
