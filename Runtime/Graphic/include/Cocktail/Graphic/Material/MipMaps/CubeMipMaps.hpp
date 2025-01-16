#ifndef COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_CUBEMIPMAPS_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_CUBEMIPMAPS_HPP

#include <Cocktail/Graphic/Material/MipMaps/MipMaps.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API CubeMipMaps : public Extends<CubeMipMaps, MipMaps>
	{
	public:

		/**
		 * \brief 
		 */
		enum class Face
		{
			PositiveX,
			NegativeX,
			PositiveY,
			NegativeY,
			PositiveZ,
			NegativeZ,
		};


		/**
		 * \brief
		 * \param baseSize
		 * \param pixelFormat
		 * \param arrayLayerCount
		 * \param levelCount
		 */
		CubeMipMaps(const Extent2D<unsigned int>& baseSize, const PixelFormat& pixelFormat, unsigned int arrayLayerCount, unsigned int levelCount);

		/**
		 * \brief 
		 * \return 
		 */
		bool IsCube() const override;

		/**
		 * \brief 
		 * \param face 
		 * \param layer 
		 * \param level 
		 * \return 
		 */
		MipMapLevel& GetFaceLevel(Face face, unsigned int layer, unsigned int level);

		/**
		 * \brief
		 * \param face
		 * \param layer
		 * \param level
		 * \return
		 */
		const MipMapLevel& GetFaceLevel(Face face, unsigned int layer, unsigned int level) const;
	};
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_CUBEMIPMAPS_HPP
