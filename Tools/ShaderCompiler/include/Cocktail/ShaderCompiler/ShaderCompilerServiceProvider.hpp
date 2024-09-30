#ifndef COCKTAIL_TOOLS_SHADERCOMPILER_SHADERCOMPILERSERVICEPROVIDER_HPP
#define COCKTAIL_TOOLS_SHADERCOMPILER_SHADERCOMPILERSERVICEPROVIDER_HPP

#include <Cocktail/Core/Application/ServiceProvider.hpp>

namespace Ck::Tools::ShaderCompiler
{
	class ShaderCompilerServiceProvider : public Extends<ShaderCompilerServiceProvider, ServiceProvider>
    {
    public:

        explicit ShaderCompilerServiceProvider(Ref<Application> application);

    protected:

        void DoRegister(const Ref<Application>& application) override;
    };
}

#endif // COCKTAIL_TOOLS_SHADERCOMPILER_SHADERCOMPILERSERVICEPROVIDER_HPP
