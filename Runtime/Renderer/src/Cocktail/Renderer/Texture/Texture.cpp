#include <Cocktail/Renderer/Texture/Texture.hpp>

namespace Ck::Renderer
{
	unsigned int ComputeTextureLevelCount(const Extent3D<unsigned int>& baseSize)
	{
		unsigned int maxDimension = std::max({ baseSize.Width, baseSize.Height, baseSize.Depth });
		return 1 + std::floor(std::log2(maxDimension));
	}

	Extent3D<unsigned int> ComputeTextureLevelSize(const Extent3D<unsigned int>& size, unsigned int level)
	{
		Extent3D<unsigned int> levelSize = size;
		for (unsigned int i = 0; i < level; i++)
			levelSize /= 2u;

		levelSize.Width = std::max(levelSize.Width, 1u);
		levelSize.Height = std::max(levelSize.Height, 1u);
		levelSize.Depth = std::max(levelSize.Depth, 1u);

		return levelSize;
	}
}
