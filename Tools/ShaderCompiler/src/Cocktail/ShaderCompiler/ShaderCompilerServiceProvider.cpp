#include <Cocktail/Core/Application/Application.hpp>

#include <Cocktail/ShaderCompiler/CompilerFactory.hpp>
#include <Cocktail/ShaderCompiler/ShaderCompilerServiceProvider.hpp>

namespace Ck::Tools::ShaderCompiler
{
	COCKTAIL_REGISTER_SERVICE_PROVIDER(ShaderCompilerServiceProvider);

	ShaderCompilerServiceProvider::ShaderCompilerServiceProvider(Ref<Application> application):
		Super(std::move(application))
	{
		/// Nothing
	}

	void ShaderCompilerServiceProvider::DoRegister(const Ref<Application>& application)
	{
		application->Singleton<CompilerFactory>();
	}
}
