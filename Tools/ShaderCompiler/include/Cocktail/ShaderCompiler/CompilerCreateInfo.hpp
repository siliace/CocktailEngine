#ifndef COCKTAIL_TOOLS_SHADERCOMPILER_COMPILERCREATEINFO_HPP
#define COCKTAIL_TOOLS_SHADERCOMPILER_COMPILERCREATEINFO_HPP

#include <Cocktail/ShaderCompiler/ShaderBinaryFormat.hpp>
#include <Cocktail/ShaderCompiler/TargetApi.hpp>

namespace Ck::Tools::ShaderCompiler
{
	/**
	 * \brief
	 */
	struct CompilerCreateInfo
	{
		/**
		 * \brief
		 */
		TargetApi TargetApi = TargetApi::Vulkan_1_0;

		/**
		 * \brief
		 */
		ShaderBinaryFormat OutputFormat = ShaderBinaryFormat::Spv_1_0;
	};
}

#endif // COCKTAIL_TOOLS_SHADERCOMPILER_COMPILERCREATEINFO_HPP
