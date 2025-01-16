#ifndef COCKTAIL_GRAPHIC_SCENE_OBJ_OBJVERTEX_HPP
#define COCKTAIL_GRAPHIC_SCENE_OBJ_OBJVERTEX_HPP

#include <Cocktail/Core/Cocktail.hpp>
#include <Cocktail/Core/Math/Vector/Vector3.hpp>

namespace Ck
{
	struct ObjVertex
	{
		struct Comparator
		{
			bool operator()(const ObjVertex& lhs, const ObjVertex& rhs) const noexcept
			{
				return std::tie(lhs.Position, lhs.TexCoord, lhs.Normal) == std::tie(rhs.Position, rhs.TexCoord, rhs.Normal);
			}
		};

		struct Hasher
		{
			std::size_t operator()(const ObjVertex& objVertex) const noexcept
			{
				std::size_t positionHash = 0;
				for (auto i = 0; i < objVertex.Position.GetSize(); i++)
					HashCombine(positionHash, objVertex.Position.At(i));

				std::size_t texCoordHash = 0;
				for (auto i = 0; i < objVertex.TexCoord.GetSize(); i++)
					HashCombine(texCoordHash, objVertex.TexCoord.At(i));

				std::size_t normalHash = 0;
				for (auto i = 0; i < objVertex.Normal.GetSize(); i++)
					HashCombine(normalHash, objVertex.Normal.At(i));

				return HashMerge(positionHash, HashMerge(texCoordHash, normalHash));
			}
		};

		Vector3<float> Position;
		Vector2<float> TexCoord;
		Vector3<float> Normal;
	};
}

#endif // COCKTAIL_GRAPHIC_SCENE_OBJ_OBJVERTEX_HPP