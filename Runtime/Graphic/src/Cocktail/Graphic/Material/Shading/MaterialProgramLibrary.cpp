#include <Cocktail/Graphic/Material/Shading/MaterialProgramLibrary.hpp>

namespace Ck
{
	void MaterialProgramLibrary::Register(const Entry& entry)
	{
		mEntries.Add(entry);
	}

	const Array<MaterialProgramLibrary::Entry>& MaterialProgramLibrary::GetEntries() const
	{
		return mEntries;
	}
}
