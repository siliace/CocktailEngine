#include <Cocktail/Core/Log/Log.hpp>

#include <Cocktail/ShaderCompiler/ShaderBinaryFormat.hpp>
#include <Cocktail/ShaderCompiler/TargetApi.hpp>
#include <Cocktail/ShaderCompiler/Glslang/GlslangUtils.hpp>
#include <Cocktail/ShaderCompiler/Glslang/SpirvShaderObject.hpp>


namespace Ck::Tools::ShaderCompiler
{
	const TBuiltInResource* SpirvShaderObject::DefaultResources = nullptr;

	SpirvShaderObject::SpirvShaderObject(const ShaderObjectCreateInfo& createInfo, TargetApi targetApi, ShaderBinaryFormat format) :
		mStage(createInfo.Stage),
		mLanguage(createInfo.Language),
		mIncluder(createInfo.Location)
	{
		const bool targetOpenGL = targetApi == TargetApi::OpenGL_4_5;
		for (unsigned int i = 0; i < createInfo.SourceCount; i++)
		{
			const ShaderSource& source = createInfo.Sources[i];

			mSources.push_back(source.Code.data());
			mSourceLengths.push_back(source.Code.length());
			mSourceNames.push_back(source.Name.data());
		}

		mShader = std::make_unique<glslang::TShader>(ToGlslangType(mStage));
		mShader->setAutoMapBindings(true);
		mShader->setAutoMapLocations(true);
		if (!targetOpenGL)
		{
			mShader->setEnvInput(ToGlslangType(mLanguage), mShader->getStage(), glslang::EShClientVulkan, 450);
			mShader->setEnvClient(glslang::EShClientVulkan, ToGlslangType(targetApi));
		}
		else
		{
			mShader->setEnvInput(ToGlslangType(mLanguage), mShader->getStage(), glslang::EShClientOpenGL, 100);
			mShader->setEnvClient(glslang::EShClientOpenGL, glslang::EShTargetOpenGL_450);
		}
		mShader->setEnvTarget(glslang::EshTargetSpv, ToGlslangType(format));
		mShader->setStringsWithLengthsAndNames(mSources.data(), mSourceLengths.data(), mSourceNames.data(), static_cast<int>(mSources.size()));
	}

	bool SpirvShaderObject::Compile()
	{
		bool success = mShader->parse(DefaultResources, 450, false, EShMsgSpvRules | EShMsgVulkanRules, mIncluder);
		if (!success)
			Log::Error("Failed to compile shader:\n{}", mShader->getInfoLog());

		return success;
	}

	Renderer::ShaderType SpirvShaderObject::GetStage() const
	{
		return mStage;
	}

	glslang::TShader* SpirvShaderObject::GetHandle() const
	{
		return mShader.get();
	}
}
