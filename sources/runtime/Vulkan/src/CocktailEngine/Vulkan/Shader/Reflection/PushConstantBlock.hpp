#ifndef COCKTAILENGINE_VULKAN_SHADER_REFLECTION_PUSHCONSTANTBLOCKINFO_HPP
#define COCKTAILENGINE_VULKAN_SHADER_REFLECTION_PUSHCONSTANTBLOCKINFO_HPP

#include <CocktailEngine/Vulkan/Shader/Reflection/BlockMember.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief
	 */
	struct PushConstantBlockInfo
	{
		/**
		 * \brief
		 */
		AsciiString Name;

		/**
		 * \brief 
		 */
		unsigned int BaseOffset = 0;

		/**
		 * \brief 
		 */
		unsigned int Size = 0;

		/**
		 * \brief
		 */
		Array<BlockMember> Members;
	};
}

#endif // COCKTAILENGINE_VULKAN_SHADER_REFLECTION_PUSHCONSTANTBLOCKINFO_HPP
