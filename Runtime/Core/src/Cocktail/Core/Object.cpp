#include <stdexcept>

#include <Cocktail/Core/Object.hpp>

namespace Ck
{
	void Object::AssignRefCounter(Object* object, RefCounter* refCounter)
	{
		if (object->mRefCounter)
			throw std::logic_error("Cannot assign a ref counter to an object already holding one");

		object->mRefCounter = refCounter;
	}

	bool Object::HasTypeInfo(const std::type_info& typeInfo) const
	{
		return typeid(Object) == typeInfo;
	}

	const std::type_info& Object::GetTypeInfo() const
	{
		return typeid(Object);
	}

	Object::Object() :
		mRefCounter(nullptr)
	{
		/// Nothing
	}

	RefCounter* Object::GetRefCounter() const
	{
		return mRefCounter;
	}
}
