#include <Cocktail/Graphic/Material/MipMaps/CubeMipMaps.hpp>

namespace Ck
{
	CubeMipMaps::CubeMipMaps(const Extent2D<unsigned int>& baseSize, const PixelFormat& pixelFormat, unsigned int arrayLayerCount, unsigned int levelCount):
		MipMaps(MakeExtent(baseSize, 1u), pixelFormat, arrayLayerCount * 6, levelCount)
	{
		/// Nothing	
	}

	bool CubeMipMaps::IsCube() const
	{
		return true;
	}

	MipMapLevel& CubeMipMaps::GetFaceLevel(Face face, unsigned int layer, unsigned level)
	{
		return GetLevel(layer + static_cast<unsigned int>(face), level);
	}

	const MipMapLevel& CubeMipMaps::GetFaceLevel(Face face, unsigned int layer, unsigned level) const
	{
		return GetLevel(layer + static_cast<unsigned int>(face), level);
	}
}
