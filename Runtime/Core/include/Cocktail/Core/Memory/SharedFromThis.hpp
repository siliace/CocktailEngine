#ifndef COCKTAIL_CORE_MEMORY_SHAREDFROMTHIS_HPP
#define COCKTAIL_CORE_MEMORY_SHAREDFROMTHIS_HPP

#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/Memory/WeakPtr.hpp>

namespace Ck
{
    /**
     * \brief Exception thrown when attempting to obtain a self SharedPtr from an object that is not owned by a SharedPtr
     */
    COCKTAIL_DECLARE_EXCEPTION(InvalidSelfPointer);

    /**
     * \brief Provides access to shared and weak references to the current object
     *
     * When an object deriving from SharedFromThis is owned by a SharedPtr, it can
     * safely create additional SharedPtr and WeakPtr instances referring to
     * itself.
     *
     * \tparam T Managed object type
     */
    template <typename T>
    class SharedFromThis : public Detail::SharedFromThisBase
    {
    public:

        /**
         * \brief Returns a SharedPtr referring to this object
         *
         * Throws an exception if the object is not currently owned by a SharedPtr.
         *
         * \return SharedPtr referring to this object
         *
         * \throw InvalidSelfPointer If the internal weak reference has expired
         */
        SharedPtr<T> AsShared()
        {
            if (SharedPtr<T> result = mSelf.ToStrong())
                return result;

            throw InvalidSelfPointer();
        }

        /**
         * \brief Returns a WeakPtr referring to this object
         *
         * \return WeakPtr referring to this object
         */
        WeakPtr<T> AsWeak()
        {
            return mSelf;
        }

    protected:

        /**
         * \brief Initializes the internal weak reference used for SharedFromThis support
         *
         * This method is called by SharedPtr when it first takes ownership of an object
         * deriving from SharedFromThis. It stores a weak reference to the same control
         * block so that the object can later safely create SharedPtr or WeakPtr to itself.
         *
         * The operation is intended to be called exactly once for a given object lifetime.
         * Reinitializing the weak reference is not expected and may lead to undefined behavior.
         *
         * \param pointer Pointer to the managed object instance
         * \param refCounter Control block associated with the owning SharedPtr
         */
        void InitWeakThis(void* pointer, Detail::RefCounter* refCounter) override
        {
            if (mSelf.IsExpired())
            {
                refCounter->WeakAcquire();
                mSelf = WeakPtr<T>(static_cast<T*>(pointer), refCounter);
            }
        }

    private:

        WeakPtr<T> mSelf;
    };
}

#endif // COCKTAILENGINE_SHAREDFROMTHIS_HPP
