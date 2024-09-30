#ifndef COCKTAIL_TOOLS_SHADERCOMPILER_GLSLANG_GLSLANGUTILS_HPP
#define COCKTAIL_TOOLS_SHADERCOMPILER_GLSLANG_GLSLANGUTILS_HPP

#include <glslang/Public/ShaderLang.h>

#include <Cocktail/Renderer/Shader/ShaderType.hpp>

#include <Cocktail/ShaderCompiler/Language.hpp>
#include <Cocktail/ShaderCompiler/ShaderBinaryFormat.hpp>
#include <Cocktail/ShaderCompiler/TargetApi.hpp>

namespace Ck::Tools::ShaderCompiler
{
	/**
	 * \brief 
	 * \param stage 
	 * \return 
	 */
	EShLanguage ToGlslangType(Renderer::ShaderType stage);

	/**
	 * \brief 
	 * \param language 
	 * \return 
	 */
	glslang::EShSource ToGlslangType(Language language);

	/**
	 * \brief
	 * \param targetApi
	 * \return
	 */
	glslang::EShTargetClientVersion ToGlslangType(TargetApi targetApi);

	/**
	 * \brief 
	 * \param format 
	 * \return 
	 */
	glslang::EShTargetLanguageVersion ToGlslangType(ShaderBinaryFormat format);
}

#endif // COCKTAIL_TOOLS_SHADERCOMPILER_GLSLANG_GLSLANGUTILS_HPP
