#include <glslang/Public/ShaderLang.h>
#include <SPIRV/GlslangToSpv.h>

#include <Cocktail/Core/Utility/EnumMap.hpp>

#include <Cocktail/ShaderCompiler/Compiler.hpp>
#include <Cocktail/ShaderCompiler/Glslang/GlslangServiceProvider.hpp>

#include "GlslangUtils.hpp"
#include "SpirvProgram.hpp"
#include "SpirvShaderObject.hpp"
#include "Cocktail/ShaderCompiler/CompilerFactory.hpp"
#include "glslang/Public/ResourceLimits.h"

COCKTAIL_REGISTER_SERVICE_PROVIDER(Ck::Tools::ShaderCompiler::GlslangServiceProvider);

namespace Ck::Tools::ShaderCompiler
{
	GlslangServiceProvider::GlslangServiceProvider(const Ref<Application>& application):
		Super(application)
	{
		/// Nothing	
	}

	GlslangServiceProvider::~GlslangServiceProvider()
	{
		glslang::FinalizeProcess();
	}

	void GlslangServiceProvider::DoBoot(const Ref<Application>& application)
	{
		glslang::InitializeProcess();
		SpirvShaderObject::DefaultResources = GetDefaultResources();
	}
}
