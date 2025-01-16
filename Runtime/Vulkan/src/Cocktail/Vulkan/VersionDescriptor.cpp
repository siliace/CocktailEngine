#include <Cocktail/Vulkan/VersionDescriptor.hpp>
#include <Cocktail/Vulkan/Volk.hpp>

namespace Ck::Vulkan
{
	VersionDescriptor::VersionDescriptor(Uint32 version) :
		Major(VK_API_VERSION_MAJOR(version)),
		Minor(VK_API_VERSION_MINOR(version)),
		Patch(VK_API_VERSION_PATCH(version))
	{
		/// Nothing
	}

	VersionDescriptor::VersionDescriptor(Uint8 major, Uint8 minor, Uint8 patch) :
		Major(major),
		Minor(minor),
		Patch(patch)
	{
		/// Nothing
	}

	Uint32 VersionDescriptor::ToInteger() const
	{
		return VK_MAKE_API_VERSION(0, Major, Minor, Patch);
	}
}
