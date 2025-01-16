#ifndef COCKTAIL_VULKAN_VERSIONDESCRIPTOR_HPP
#define COCKTAIL_VULKAN_VERSIONDESCRIPTOR_HPP

#include <Cocktail/Vulkan/Export.hpp>

namespace Ck::Vulkan
{
	/**
	 * \brief Structure describing the version of an extension or a layer
	 */
	struct COCKTAIL_VULKAN_API VersionDescriptor
	{
		/**
		 * \brief Default destructor
		 */
		VersionDescriptor() = default;

		/**
		 * \brief Constructor
		 * \param version The described version as binary mask
		 */
		explicit VersionDescriptor(Uint32 version);

		/**
		 * \brief Constructor
		 * \param major The major version
		 * \param minor The minor version
		 * \param patch The patch version
		 */
		VersionDescriptor(Uint8 major, Uint8 minor, Uint8 patch);

		/**
		 * \brief 
		 * \return 
		 */
		Uint32 ToInteger() const;

		/**
		 * \brief Specifies the major part of the described version
		 */
		Uint8 Major = 0;

		/**
		 * \brief Specifies the minor part of the described version
		 */
		Uint8 Minor = 0;

		/**
		 * \brief Specifies the patch part of the described version
		 */
		Uint8 Patch = 0;
	};
}

#endif // COCKTAIL_VULKAN_VERSIONDESCRIPTOR_HPP
