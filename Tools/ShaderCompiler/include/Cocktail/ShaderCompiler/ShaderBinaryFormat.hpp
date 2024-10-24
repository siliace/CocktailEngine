#ifndef COCKTAIL_TOOLS_SHADERCOMPILER_SHADERBINARYFORMAT_HPP
#define COCKTAIL_TOOLS_SHADERCOMPILER_SHADERBINARYFORMAT_HPP

namespace Ck::Tools::ShaderCompiler
{
	enum class ShaderBinaryFormat
	{
		Spv_1_0,
		Spv_1_1,
		Spv_1_2,
		Spv_1_3,
		Spv_1_4,
		Spv_1_5,
		Spv_1_6,
		DXIL_SM_6,
	};
}

#endif // COCKTAIL_TOOLS_SHADERCOMPILER_SHADERBINARYFORMAT_HPP
