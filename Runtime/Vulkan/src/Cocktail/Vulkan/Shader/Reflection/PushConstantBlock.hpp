#ifndef COCKTAIL_VULKAN_SHADER_REFLECTION_PUSHCONSTANTBLOCKINFO_HPP
#define COCKTAIL_VULKAN_SHADER_REFLECTION_PUSHCONSTANTBLOCKINFO_HPP

#include <Cocktail/Vulkan/Shader/Reflection/BlockMember.hpp>

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

#endif // COCKTAIL_VULKAN_SHADER_REFLECTION_PUSHCONSTANTBLOCKINFO_HPP
