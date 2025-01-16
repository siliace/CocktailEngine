#ifndef COCKTAIL_CORE_OBJECT_HPP
#define COCKTAIL_CORE_OBJECT_HPP

#include <typeinfo>

#include <Cocktail/Core/Export.hpp>

namespace Ck
{
	class RefCounter;

	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API Object
	{
	public:

		/**
		 * \brief Safely assign a RefCounter to an object if its does not hold one yet
		 * \param object 
		 * \param refCounter 
		 */
		static void AssignRefCounter(Object* object, RefCounter* refCounter);

		/**
		 * \brief 
		 */
		virtual ~Object() = default;

		/**
		 * \brief 
		 * \param other 
		 */
		Object(const Object& other) = delete;

		/**
		 * \brief 
		 * \param other 
		 */
		Object(Object&& other) noexcept = default;

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		Object& operator=(const Object& other) = delete;

		/**
		 * \brief 
		 * \param other 
		 * \return 
		 */
		Object& operator=(Object&& other) noexcept = default;

		/**
		 * \brief 
		 * \param typeInfo 
		 * \return 
		 */
		virtual bool HasTypeInfo(const std::type_info& typeInfo) const;

		/**
		 * \brief 
		 * \return 
		 */
		virtual const std::type_info& GetTypeInfo() const;

	protected:

		/**
		 * \brief 
		 */
		Object();

		/**
		 * \brief 
		 * \return 
		 */
		RefCounter* GetRefCounter() const;

	private:

		RefCounter* mRefCounter;
	};

	template <typename T>
	using IsObjectHelper = std::is_base_of<Object, T>;

	template <typename T>
	static constexpr bool IsObject = IsObjectHelper<T>::value;
}

#endif // COCKTAIL_CORE_OBJECT_HPP
