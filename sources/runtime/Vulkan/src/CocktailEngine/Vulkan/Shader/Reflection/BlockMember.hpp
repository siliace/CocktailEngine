#ifndef COCKTAILENGINE_VULKAN_SHADER_REFLECTION_BLOCKMEMBER_HPP
#define COCKTAILENGINE_VULKAN_SHADER_REFLECTION_BLOCKMEMBER_HPP

#include <CocktailEngine/Core/Array.hpp>
#include <CocktailEngine/Core/DataType.hpp>

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

			return Type.GetSize() * ElementCount * ArrayLength;
		}

		/**
		 * \brief
		 */
		std::size_t Offset = 0;

		/**
		 * \brief
		 */
		DataType Type;

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
		AsciiString Name;

		/**
		 * \brief
		 */
		Array<BlockMember> Members;
	};
}

#endif // COCKTAILENGINE_VULKAN_SHADER_REFLECTION_BLOCKMEMBER_HPP
