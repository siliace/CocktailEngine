#include <Cocktail/Renderer/Shader/ShaderProgramType.hpp>

namespace Ck::Renderer
{
	bool ProgramSupportShader(ShaderProgramType programType, ShaderType shaderType)
	{
		switch (programType)
		{
		case ShaderProgramType::Graphic:
			return AllGraphicStages & shaderType;

		case ShaderProgramType::Compute:
			return shaderType == ShaderType::Compute;
		}

		return false;
	}
}
