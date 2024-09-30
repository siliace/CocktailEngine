#ifndef COCKTAIL_TOOLS_SHADERCOMPILER_SHADEROBJECT_HPP
#define COCKTAIL_TOOLS_SHADERCOMPILER_SHADEROBJECT_HPP

#include <filesystem>
#include <string>

#include <Cocktail/Core/Meta/Implements.hpp>

#include <Cocktail/Renderer/Shader/ShaderType.hpp>

#include <Cocktail/ShaderCompiler/Language.hpp>

namespace Ck::Tools::ShaderCompiler
{
	struct ShaderSource
	{
		/**
		 * \brief 
		 */
		std::string_view Code;

		/**
		 * \brief 
		 */
		std::string_view Name;
	};

	/**
	 * \brief 
	 */
	struct ShaderObjectCreateInfo
	{
		/**
		 * \brief 
		 */
		Renderer::ShaderType Stage = Renderer::ShaderType::Vertex;

		/**
		 * \brief 
		 */
		Language Language = Language::Glsl;

		/**
		 * \brief 
		 */
		unsigned int SourceCount = 0;

		/**
		 * \brief 
		 */
		const ShaderSource* Sources = nullptr;

		/**
		 * \brief 
		 */
		std::filesystem::path Location;
	};

	/**
	 * \brief 
	 */
	class ShaderObject : public Implements<ShaderObject, Interface>
	{
	public:

		/**
		 * \brief
		 * \return
		 */
		virtual bool Compile() = 0;

		/**
		 * \brief 
		 * \return 
		 */
		virtual Renderer::ShaderType GetStage() const = 0;
	};
}

#endif // COCKTAIL_TOOLS_SHADERCOMPILER_SHADEROBJECT_HPP
