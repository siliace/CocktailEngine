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
		unsigned int GetSize() const
		{
			unsigned int size = 0;
			for (const BlockMember& member : Members)
				size += member.GetSize();

			return size;
		}

		/**
		 * \brief
		 */
		std::string Name;

		/**
		 * \brief 
		 */
		unsigned int BaseOffset = 0;

		/**
		 * \brief
		 */
		std::vector<BlockMember> Members;
	};
}

#endif // COCKTAIL_VULKAN_SHADER_REFLECTION_PUSHCONSTANTBLOCKINFO_HPP
