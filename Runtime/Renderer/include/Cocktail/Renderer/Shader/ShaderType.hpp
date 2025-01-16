#ifndef COCKTAIL_RENDERER_SHADER_SHADERTYPE_HPP
#define COCKTAIL_RENDERER_SHADER_SHADERTYPE_HPP

#include <Cocktail/Core/Flags.hpp>

namespace Ck::Renderer
{
	/**
	 * \brief 
	 */
	enum class ShaderType
	{
		/**
		 * \brief Vertex shader type
		 */
		Vertex = Bit(0),

		/**
		 * \brief Tessellation control shader type
		 */
		TessellationControl = Bit(1),

		/**
		 * \brief Tessellation evaluation shader type
		 */
		TessellationEvaluation = Bit(2),

		/**
		 * \brief Geometry shader type
		 */
		Geometry = Bit(3),

		/**
		 * \brief Fragment shader type
		 */
		Fragment = Bit(4),

		/**
		 * \brief Compute shader type
		 */
		Compute = Bit(5),
	};

	using ShaderStageFlags = Flags<ShaderType>;

	/**
	 * \brief Helper constant of all shader tessellation stages
	 */
	static const ShaderStageFlags AllTessellationStages(ShaderType::TessellationControl | ShaderType::TessellationEvaluation);

	/**
	 * \brief Helper constant of all shader graphic stages
	 */
	static const ShaderStageFlags AllGraphicStages(ShaderType::Vertex | AllTessellationStages | ShaderType::Geometry | ShaderType::Fragment);
}

#endif // COCKTAIL_RENDERER_SHADER_SHADERTYPE_HPP
