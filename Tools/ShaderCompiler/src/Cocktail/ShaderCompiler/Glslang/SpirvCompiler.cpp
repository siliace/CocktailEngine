#include <Cocktail/ShaderCompiler/Glslang/SpirvCompiler.hpp>
#include <Cocktail/ShaderCompiler/Glslang/SpirvProgram.hpp>

namespace Ck::Tools::ShaderCompiler
{
	SpirvCompiler::SpirvCompiler(const CompilerCreateInfo& createInfo) :
		mTargetApi(createInfo.TargetApi),
		mOutputFormat(createInfo.OutputFormat)
	{
		/// Nothing
	}

	Ref<ShaderObject> SpirvCompiler::CreateShaderObject(const ShaderObjectCreateInfo& createInfo)
	{
		return SpirvShaderObject::New(createInfo, mTargetApi, mOutputFormat);
	}

	Ref<Program> SpirvCompiler::CreateProgram(Renderer::ShaderProgramType type)
	{
		return SpirvProgram::New(type);
	}

	ShaderBinaryFormat SpirvCompiler::GetOutputFormat() const
	{
		return mOutputFormat;
	}
}
