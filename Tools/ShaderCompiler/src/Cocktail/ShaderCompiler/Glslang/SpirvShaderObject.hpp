#ifndef COCKTAIL_TOOLS_SHADERCOMPILER_GLSLANG_SPIRVSHADEROBJECT_HPP
#define COCKTAIL_TOOLS_SHADERCOMPILER_GLSLANG_SPIRVSHADEROBJECT_HPP

#include <memory>

#include <glslang/Include/ResourceLimits.h>
#include <glslang/Public/ShaderLang.h>

#include <Cocktail/Core/Meta/Inherit.hpp>

#include <Cocktail/ShaderCompiler/ShaderObject.hpp>
#include <Cocktail/ShaderCompiler/ShaderBinaryFormat.hpp>
#include <Cocktail/ShaderCompiler/TargetApi.hpp>
#include <Cocktail/ShaderCompiler/Glslang/GlslShaderIncluder.hpp>

namespace Ck::Tools::ShaderCompiler
{
	class SpirvShaderObject : public Inherit<SpirvShaderObject, Object, ShaderObject>
	{
	public:

		/**
		 * \brief 
		 */
		static const TBuiltInResource* DefaultResources;

		/**
		 * \brief 
		 * \param createInfo
		 * \param targetApi
		 * \param format 
		 */
		SpirvShaderObject(const ShaderObjectCreateInfo& createInfo, TargetApi targetApi, ShaderBinaryFormat format);

		/**
		 * \brief 
		 * \return 
		 */
		bool Compile() override;

		/**
		 * \brief 
		 * \return 
		 */
		Renderer::ShaderType GetStage() const override;

		/**
		 * \brief 
		 * \return 
		 */
		glslang::TShader* GetHandle() const;

	private:

		Renderer::ShaderType mStage;
		Language mLanguage;
		GlslShaderIncluder mIncluder;
		std::unique_ptr<glslang::TShader> mShader;
		std::vector<const char*> mSources;
		std::vector<int> mSourceLengths;
		std::vector<const char*> mSourceNames;
	};
}
#endif // COCKTAIL_TOOLS_SHADERCOMPILER_GLSLANG_SPIRVSHADEROBJECT_HPP
