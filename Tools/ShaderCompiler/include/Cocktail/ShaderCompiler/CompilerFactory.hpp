#ifndef COCKTAIL_TOOLS_COMPILERFACTORY_HPP
#define COCKTAIL_TOOLS_COMPILERFACTORY_HPP

#include <Cocktail/Core/Meta/Extends.hpp>

#include <Cocktail/ShaderCompiler/Compiler.hpp>
#include <Cocktail/ShaderCompiler/CompilerCreateInfo.hpp>

namespace Ck::Tools::ShaderCompiler
{
	class CompilerFactory : public Extends<CompilerFactory, Object>
	{
	public:

		/**
		 * \brief 
		 * \param createInfo 
		 * \return 
		 */
		Ref<Compiler> CreateCompiler(const CompilerCreateInfo& createInfo) const;
	};
}

#endif // COCKTAIL_TOOLS_COMPILERFACTORY_HPP
