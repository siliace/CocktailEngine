#include <Cocktail/Graphic/Material/Shading/MaterialProgramLibrary.hpp>

namespace Ck
{
	void MaterialProgramLibrary::Register(const Entry& entry)
	{
		mEntries.push_back(entry);
	}

	const std::vector<MaterialProgramLibrary::Entry>& MaterialProgramLibrary::GetEntries() const
	{
		return mEntries;
	}
}
