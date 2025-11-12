#ifndef COCKTAIL_VULKAN_SHADER_REFLECTION_BLOCKMEMBER_HPP
#define COCKTAIL_VULKAN_SHADER_REFLECTION_BLOCKMEMBER_HPP

#include <Cocktail/Core/Array.hpp>
#include <Cocktail/Core/DataType.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief
	 */
	struct BlockMember
	{
		std::size_t GetSize() const
		{
			if (!Members.IsEmpty())
			{
				std::size_t membersSize = 0;
				for (const BlockMember& member : Members)
					membersSize += member.GetSize();

				return membersSize * ArrayLength;
			}

			return DataType.GetSize() * ElementCount * ArrayLength;
		}

		/**
		 * \brief
		 */
		std::size_t Offset = 0;

		/**
		 * \brief
		 */
		DataType DataType;

		/**
		 * \brief
		 */
		unsigned int ElementCount = 0;

		/**
		 * \brief
		 */
		unsigned int ArrayLength = 0;

		/**
		 * \brief
		 */
		AnsiString Name;

		/**
		 * \brief
		 */
		Array<BlockMember> Members;
	};
}

#endif // COCKTAIL_VULKAN_SHADER_REFLECTION_BLOCKMEMBER_HPP
