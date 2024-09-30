#include <Cocktail/Core/Application/Application.hpp>

#include <Cocktail/Main/ExitCode.hpp>

#include <Cocktail/ShaderCompiler/CompilerFactory.hpp>
#include <Cocktail/ShaderCompiler/Project.hpp>

#include "Cocktail/Core/Log/ConsoleLogChannel.hpp"
#include "Cocktail/Core/Log/Log.hpp"
#include "Cocktail/Core/System/FileSystem/Storage.hpp"
#include "Cocktail/Core/Utility/FileUtils.hpp"

using namespace Ck;
using namespace Ck::Main;
using namespace Ck::Tools::ShaderCompiler;

std::string MakeBinaryExtension(Renderer::ShaderType shaderType, ShaderBinaryFormat format)
{
    std::string extension;
    switch (shaderType)
    {
    case Renderer::ShaderType::Vertex:
        extension += ".vert";
	    break;

    case Renderer::ShaderType::TessellationControl:
        extension += ".tessC";
	    break;

    case Renderer::ShaderType::TessellationEvaluation:
        extension += ".tessE";
	    break;

    case Renderer::ShaderType::Geometry:
        extension += ".geom";
	    break;

    case Renderer::ShaderType::Fragment:
        extension += ".frag";
	    break;

    case Renderer::ShaderType::Compute:
        extension += ".comp";
	    break;
    }

    switch (format)
    {
    case ShaderBinaryFormat::Spv_1_0:
    case ShaderBinaryFormat::Spv_1_1:
    case ShaderBinaryFormat::Spv_1_2:
    case ShaderBinaryFormat::Spv_1_3:
    case ShaderBinaryFormat::Spv_1_4:
    case ShaderBinaryFormat::Spv_1_5:
    case ShaderBinaryFormat::Spv_1_6:
        extension += ".spv";
	    break;

    case ShaderBinaryFormat::DXIL_SM_6:
        extension += ".cso";
        break;
    }

    return extension;
}

ExitCode ApplicationMain(Ref<Application> application)
{
	std::vector<std::string> args = application->GetArgv();

    std::filesystem::path projectPath = args.back();
    if (!Storage::IsFile(projectPath))
    {
        Log::Error("Project file {} does not exists", projectPath.string());
        return ExitCode::GeneralError;
    }

    Ref<Compiler> compiler = application->Resolve<CompilerFactory>()->CreateCompiler({});

    Project project = Project::LoadFromPath(projectPath);
	EnumMap<Renderer::ShaderType, std::vector<Uint32>> result = project.Build(*compiler);

    std::string projectName = projectPath.stem().string();
    for (Renderer::ShaderType stage : Enum<Renderer::ShaderType>::Values)
    {
        const auto& binary = result[stage];
        if (binary.empty())
            continue;

	    std::filesystem::path outputPath = projectPath.parent_path() / projectName;
        outputPath.replace_extension(MakeBinaryExtension(stage, compiler->GetOutputFormat()));

	    Ref<File> file = Storage::OpenFile(outputPath, FileOpenFlagBits::Truncate | FileOpenFlagBits::Write);
        file->Write(binary.data(), binary.size() * sizeof(Uint32));
    }

	return ExitCode::Success;
}
