#include <SPIRV/GlslangToSpv.h>
#include <SPIRV/SpvTools.h>

#include <Cocktail/ShaderCompiler/Glslang/SpirvProgram.hpp>
#include <Cocktail/ShaderCompiler/Glslang/GlslangUtils.hpp>

namespace Ck::Tools::ShaderCompiler
{
	SpirvProgram::SpirvProgram(Renderer::ShaderProgramType type) :
		mType(type)
	{
		mProgram = std::make_unique<glslang::TProgram>();
	}

	void SpirvProgram::AddShaderObject(Ref<ShaderObject> shaderObject)
	{
		Ref<SpirvShaderObject> spirvShaderObject = SpirvShaderObject::Cast(shaderObject);
		if (!Renderer::ProgramSupportShader(mType, spirvShaderObject->GetStage()))
			throw std::runtime_error("Incompatible stage with program");

		mProgram->addShader(spirvShaderObject->GetHandle());

		mShaderObjects.push_back(spirvShaderObject);
	}

	bool SpirvProgram::Link()
	{
		bool success = mProgram->link(EShMsgSpvRules | EShMsgVulkanRules);
		if (!success)
			return false;

		mProgram->mapIO();

		for (Renderer::ShaderType shaderType : Enum<Renderer::ShaderType>::Values)
		{
			if (!Renderer::ProgramSupportShader(mType, shaderType))
				continue;

			glslang::TIntermediate* intermediate = mProgram->getIntermediate(ToGlslangType(shaderType));
			if (!intermediate)
				continue;

			glslang::SpvOptions options;
			options.validate = true;
			glslang::GlslangToSpv(*intermediate, mStageBinaries[shaderType], &options);
		}

		return true;
	}

	const std::vector<Uint32>& SpirvProgram::GetStageBinary(Renderer::ShaderType stage) const
	{
		if (!Renderer::ProgramSupportShader(mType, stage))
			throw std::runtime_error("Incompatible stage with program");

		return mStageBinaries[stage];
	}

	Renderer::ShaderProgramType SpirvProgram::GetType() const
	{
		return mType;
	}
}
