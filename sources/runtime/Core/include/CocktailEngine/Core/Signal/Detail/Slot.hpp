#ifndef COCKTAILENGINE_CORE_SIGNAL_DETAIL_SLOT_HPP
#define COCKTAILENGINE_CORE_SIGNAL_DETAIL_SLOT_HPP

#include <functional>

#include <CocktailEngine/Core/Memory/UniquePtr.hpp>
#include <CocktailEngine/Core/Signal/Detail/SlotState.hpp>

namespace Ck::Detail
{
    /**
     * \brief Abstract callback adapter owned by a signal registry.
     *
     * A slot combines callback invocation with the connection state inherited from
     * \ref SlotState. It invokes its concrete callback only while it is connected and
     * enabled.
     *
     * \tparam Args Types delivered to the callback.
     */
    template <typename... Args>
    class Slot : public SlotState
    {
    public:

        /**
         * \brief Invokes the callback when the slot is connected and enabled.
         *
         * \param args Callback arguments packaged by the emitting signal.
         */
        void Invoke(std::tuple<Args...>&& args)
        {
            if (SlotState::IsConnected() && !SlotState::IsDisabled())
                DoInvoke(Move(args));
        }

    protected:

        /**
         * \brief Creates a slot associated with a stable signal registry and invocation group.
         *
         * \param container Registry that owns the slot.
         * \param groupId Group used to order callback invocation.
         */
        Slot(const WeakPtr<SlotContainer>& container, unsigned int groupId) :
            SlotState(container, groupId)
        {
            /// Nothing
        }

        /**
         * \brief Invokes the concrete callback implementation.
         *
         * \param args Callback arguments packaged by the emitting signal.
         */
        virtual void DoInvoke(std::tuple<Args...>&& args) = 0;
    };

    /**
     * \brief Slot that owns an arbitrary callable.
     *
     * \tparam Callable Type of the stored callable.
     * \tparam Args Types delivered to the callback.
     */
    template <typename Callable, typename... Args>
    class CallableSlot final : public Slot<Args...>
    {
    public:

        /**
         * \brief Stores a callback and associates it with a stable signal registry.
         *
         * The callable is copied when an lvalue is supplied and moved when an rvalue is
         * supplied.
         *
         * \tparam T Type of the supplied callable.
         *
         * \param callable Callback to store.
         * \param container Registry that owns the slot.
         * \param groupId Group used to order callback invocation.
         */
        template <typename T>
        CallableSlot(T&& callable, const WeakPtr<SlotContainer>& container, unsigned int groupId) :
            Slot<Args...>(container, groupId),
            mCallable(Forward<T>(callable))
        {
            /// Nothing
        }

    protected:

        /**
         * \brief Applies the packaged arguments to the stored callable.
         *
         * \param args Callback arguments packaged by the emitting signal.
         */
        void DoInvoke(std::tuple<Args...>&& args) override
        {
            std::apply(mCallable, Forward<std::tuple<Args...>>(args));
        }

    private:

        Callable mCallable;
    };

    /**
     * \brief Slot that invokes a non-const member function on a non-owned object.
     *
     * \tparam T Type of the target object.
     * \tparam Args Types delivered to the member function.
     */
    template <typename T, typename... Args>
    class ObjectSlot : public Slot<Args...>
    {
    public:

        using ObjectType = T;
        using ReferenceType = T&;
        using FunctionType = void (T::*)(Args...);

        /**
         * \brief Stores a non-owning object reference and member function.
         *
         * \p object must remain alive until this slot is disconnected.
         *
         * \param object Object receiving the callback.
         * \param function Member function to invoke.
         * \param container Registry that owns the slot.
         * \param groupId Group used to order callback invocation.
         */
        ObjectSlot(ReferenceType object, FunctionType function, const WeakPtr<SlotContainer>& container, unsigned int groupId) :
            Slot<Args...>(container, groupId),
            mObject(object),
            mFunction(function)
        {
            /// Nothing
        }

    protected:

        /**
         * \brief Invokes the stored member function with the packaged arguments.
         *
         * \param args Callback arguments packaged by the emitting signal.
         */
        void DoInvoke(std::tuple<Args...>&& args) override
        {
            std::apply(
                [&](Args&&... innerArgs) {
                    std::invoke(mFunction, mObject, Forward<Args>(innerArgs)...);
                },
                Forward<std::tuple<Args...>>(args));
        }

    private:

        ReferenceType mObject;
        FunctionType mFunction;
    };

    /**
     * \brief Slot that invokes a const member function on a non-owned object.
     *
     * \tparam T Type of the target object.
     * \tparam Args Types delivered to the member function.
     */
    template <typename T, typename... Args>
    class ConstantObjectSlot : public Slot<Args...>
    {
    public:

        using ObjectType = T;
        using ReferenceType = const T&;
        using FunctionType = void (T::*)(Args...) const;

        /**
         * \brief Stores a non-owning object reference and const member function.
         *
         * \p object must remain alive until this slot is disconnected.
         *
         * \param object Object receiving the callback.
         * \param function Const member function to invoke.
         * \param container Registry that owns the slot.
         * \param groupId Group used to order callback invocation.
         */
        ConstantObjectSlot(ReferenceType object, FunctionType function, const WeakPtr<SlotContainer>& container, unsigned int groupId) :
            Slot<Args...>(container, groupId),
            mObject(object),
            mFunction(function)
        {
            /// Nothing
        }

    protected:

        /**
         * \brief Invokes the stored const member function with the packaged arguments.
         *
         * \param args Callback arguments packaged by the emitting signal.
         */
        void DoInvoke(std::tuple<Args...>&& args) override
        {
            std::apply(
                [&](Args&&... innerArgs) {
                    std::invoke(mFunction, mObject, Forward<Args>(innerArgs)...);
                },
                Forward<std::tuple<Args...>>(args));
        }

    private:

        ReferenceType mObject;
        FunctionType mFunction;
    };

    /**
     * \brief Slot decorator that disconnects its callback after one invocation.
     *
     * The connection is consumed before user code executes, so recursive emissions and
     * exceptions cannot invoke the callback a second time.
     *
     * \tparam Args Types delivered to the callback.
     */
    template <typename... Args>
    class OneShotSlot final : public Slot<Args...>
    {
    public:

        /**
         * \brief Wraps a slot that will be invoked at most once.
         *
         * \param inner Slot to invoke once.
         * \param container Registry that owns the wrapper.
         * \param groupId Group used to order callback invocation.
         */
        OneShotSlot(UniquePtr<Slot<Args...>> inner, const WeakPtr<SlotContainer>& container, unsigned int groupId) :
            Slot<Args...>(container, groupId),
            mInner(Move(inner))
        {
            /// Nothing
        }

    protected:

        /**
         * \brief Disconnects this slot, then invokes the wrapped callback.
         *
         * \param args Callback arguments packaged by the emitting signal.
         */
        void DoInvoke(std::tuple<Args...>&& args) override
        {
            SlotState::Disconnect();
            mInner->Invoke(Move(args));
        }

    private:

        UniquePtr<Slot<Args...>> mInner;
    };
}

#endif // COCKTAILENGINE_CORE_SIGNAL_DETAIL_SLOT_HPP
