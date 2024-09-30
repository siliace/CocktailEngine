#ifndef COCKTAIL_TOOLS_SHADERCOMPILER_GLSLANG_GLSLANGSERVICEPROVIDER_HPP
#define COCKTAIL_TOOLS_SHADERCOMPILER_GLSLANG_GLSLANGSERVICEPROVIDER_HPP

#include <Cocktail/Core/Application/Application.hpp>
#include <Cocktail/Core/Application/ServiceProvider.hpp>

namespace Ck::Tools::ShaderCompiler
{
	class GlslangServiceProvider final : public Extends<GlslangServiceProvider, ServiceProvider>
	{
	public:

		/**
		 * \brief 
		 * \param application 
		 */
		explicit GlslangServiceProvider(const Ref<Application>& application);

		/**
		 * \brief 
		 */
		~GlslangServiceProvider() override;

	protected:

		/**
		 * \brief
		 * \param application
		 */
		void DoBoot(const Ref<Application>& application) override;
	};
}

#endif // COCKTAIL_TOOLS_SHADERCOMPILER_GLSLANGSERVICEPROVIDER_HPP
