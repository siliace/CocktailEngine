#ifndef COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_MIPMAPS_HPP
#define COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_MIPMAPS_HPP

#include <Cocktail/Core/Image/Image.hpp>

#include <Cocktail/Graphic/Material/MipMaps/MipMapLevel.hpp>

namespace Ck
{
	struct MipMapsImportParameters
	{
		/**
		 * \brief Specifies to the loader to keep only the first level, leaving the MipMaps loaded incomplete
		 */
		bool OneLevel = false;
	};

	struct MipMapsExportParameters
	{
	};

	/**
	 * \brief 
	 */
	class COCKTAIL_GRAPHIC_API MipMaps
	{
	public:

		/**
		 * \brief 
		 * \param image
		 * \return 
		 */
		static std::shared_ptr<MipMaps> FromImage(const Image& image);

		/**
		 * \brief Compute the maximum number of mip maps level a texture with a given base size can support
		 * \param baseSize 
		 * \return 
		 */
		static unsigned int ComputeLevelCount(const Extent3D<unsigned int>& baseSize);

		/**
		 * \brief 
		 * \param size 
		 * \param level 
		 * \return 
		 */
		static Extent3D<unsigned int> ComputeLevelSize(const Extent3D<unsigned int>& size, unsigned int level);

		/**
		 * \brief 
		 * \param baseSize 
		 * \param pixelFormat 
		 * \param arrayLayerCount 
		 * \param mipMapCount 
		 */
		MipMaps(const Extent3D<unsigned int>& baseSize, const PixelFormat& pixelFormat, unsigned int arrayLayerCount, unsigned int mipMapCount);

		/**
		 * \brief 
		 */
		virtual ~MipMaps() = default;

		/**
		 * \brief Tell whether every level of every layer is provided with pixels
		 * Incomplete mipmaps will result in mipmap generations before rendering
		 * \return True if complete, false otherwise
		 */
		bool IsComplete() const;

		/**
		 * \brief 
		 * \return 
		 */
		virtual bool IsCube() const;

		/**
		 * \brief 
		 * \return 
		 */
		bool IsArray() const;

		/**
		 * \brief
		 * \param slice
		 * \param level
		 * \return
		 */
		MipMapLevel& GetLevel(unsigned int slice, unsigned int level);

		/**
		 * \brief 
		 * \param slice 
		 * \param level 
		 * \return 
		 */
		const MipMapLevel& GetLevel(unsigned int slice, unsigned int level) const;

		/**
		 * \brief 
		 * \return 
		 */
		const Extent3D<unsigned int>& GetBaseSize() const;

		/**
		 * \brief 
		 * \return 
		 */
		const PixelFormat& GetPixelFormat() const;

		/**
		 * \brief 
		 * \return 
		 */
		unsigned int GetArrayLayerCount() const;

		/**
		 * \brief 
		 * \return 
		 */
		unsigned int GetMipMapCount() const;
		
	private:

		Extent3D<unsigned int> mBaseSize;
		PixelFormat mPixelFormat;
		unsigned int mArrayLayerCount;
		unsigned int mMipMapCount;
		std::vector<MipMapLevel> mLevels;
		ByteArray mPixels;
	};
}

#endif // COCKTAIL_GRAPHIC_MATERIAL_MIPMAPS_MIPMAPS_HPP
