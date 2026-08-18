#ifndef COCKTAILENGINE_CORE_SIGNAL_DETAIL_SIGNALBASE_HPP
#define COCKTAILENGINE_CORE_SIGNAL_DETAIL_SIGNALBASE_HPP

#include <tuple>
#include <type_traits>

#include <CocktailEngine/Core/Memory/UniquePtr.hpp>
#include <CocktailEngine/Core/Signal/Connection.hpp>
#include <CocktailEngine/Core/Signal/Detail/SignalState.hpp>

namespace Ck::Detail
{
    /**
     * \brief Thread-policy-based signal facade shared by the public signal aliases.
     *
     * The slot registry is held by a stable shared state. Moving this facade transfers that
     * state without rebinding existing slots, so connections created before a move continue
     * to target the destination signal. The facade itself must not be moved, destroyed, or
     * otherwise accessed concurrently with any of its member functions.
     *
     * \tparam TLockable Lock implementation used to protect the signal registry.
     * \tparam Args Callback argument types. Arguments passed by value must be copyable,
     * because every slot receives its own argument tuple.
     */
    template <Lockable TLockable, typename... Args>
    class SignalBase
    {
    private:

        using State = SignalState<TLockable, Args...>;

    public:

        /**
         * \brief Creates a signal without connected slots.
         */
        SignalBase() :
            mState(MakeShared<State>())
        {
            /// Nothing
        }

        /**
         * \brief Signals cannot be copied because connections identify individual slots.
         */
        SignalBase(const SignalBase&) = delete;

        /**
         * \brief Transfers the stable slot registry from another signal.
         *
         * Existing connections keep targeting the transferred registry. The moved-from signal
         * is empty and recreates an independent registry when it is next connected.
         *
         * \param other Signal that transfers ownership of its registry.
         */
        SignalBase(SignalBase&& other) noexcept :
            mState(Move(other.mState))
        {
            /// Nothing
        }

        /**
         * \brief Signals cannot be copied because connections identify individual slots.
         */
        SignalBase& operator=(const SignalBase&) = delete;

        /**
         * \brief Replaces this signal's registry with one transferred from another signal.
         *
         * Existing connections to slots owned by this signal become disconnected. Connections
         * to slots transferred from \p other keep targeting this signal. The moved-from signal
         * recreates an independent registry when it is next connected.
         *
         * \param other Signal that transfers ownership of its registry.
         *
         * \return This signal.
         */
        SignalBase& operator=(SignalBase&& other) noexcept
        {
            if (this != &other)
            {
                if (const SharedPtr<State> state = GetState())
                    state->Disconnect();

                mState = Move(other.mState);
            }

            return *this;
        }

        /**
         * \brief Connects an owned callable with compatible arguments.
         *
         * The callable is copied when an lvalue is supplied and moved when an rvalue is
         * supplied. Keep the returned connection alive for as long as the callback must
         * remain registered.
         *
         * \param callable Callback to invoke.
         * \param groupId Group used to order callback invocation.
         *
         * \return A RAII connection that manages the callback.
         */
        template <typename Callable>
        [[nodiscard]] Connection Connect(Callable&& callable, unsigned int groupId = 0)
        {
            using SlotType = CallableSlot<std::decay_t<Callable>, Args...>;

            const SharedPtr<State> state = EnsureState();
            SharedPtr<Slot<Args...>> slot = CreateSlot<SlotType>(state, groupId, Forward<Callable>(callable));
            return Connection(slot);
        }

        /**
         * \brief Connects a non-const member function.
         *
         * The signal does not own or track \p object. It must remain alive until the
         * connection is disconnected.
         *
         * \tparam T Object type.
         *
         * \param object Object receiving the callback.
         * \param function Member function to invoke.
         * \param groupId Group used to order callback invocation.
         *
         * \return A RAII connection that manages the callback.
         */
        template <typename T>
        [[nodiscard]] Connection Connect(T& object, void (T::*function)(Args...), unsigned int groupId = 0)
        {
            using SlotType = ObjectSlot<T, Args...>;

            const SharedPtr<State> state = EnsureState();
            SharedPtr<Slot<Args...>> slot = CreateSlot<SlotType>(state, groupId, object, Move(function));
            return Connection(slot);
        }

        /**
         * \brief Connects a const member function.
         *
         * The signal does not own or track \p object. It must remain alive until the
         * connection is disconnected.
         *
         * \tparam T Object type.
         *
         * \param object Object receiving the callback.
         * \param function Const member function to invoke.
         * \param groupId Group used to order callback invocation.
         *
         * \return A RAII connection that manages the callback.
         */
        template <typename T>
        [[nodiscard]] Connection Connect(const T& object, void (T::*function)(Args...) const, unsigned int groupId = 0)
        {
            using SlotType = ConstantObjectSlot<T, Args...>;

            const SharedPtr<State> state = EnsureState();
            SharedPtr<Slot<Args...>> slot = CreateSlot<SlotType>(state, groupId, object, Move(function));
            return Connection(slot);
        }

        /**
         * \brief Connects an owned callable that consumes its connection before invoking.
         *
         * The slot is disconnected even if the callback throws. Exceptions are propagated
         * after the signal restores its internal state.
         *
         * \param callable Callback to invoke once.
         * \param groupId Group used to order callback invocation.
         *
         * \return A RAII connection that manages the callback.
         */
        template <typename Callable>
        [[nodiscard]] Connection ConnectOnce(Callable&& callable, unsigned int groupId = 0)
        {
            using SlotType = OneShotSlot<Args...>;
            using InnerSlotType = CallableSlot<std::decay_t<Callable>, Args...>;

            const SharedPtr<State> state = EnsureState();
            WeakPtr<SlotContainer> container(state);
            UniquePtr<InnerSlotType> inner = MakeUnique<InnerSlotType>(Forward<Callable>(callable), container, groupId);
            SharedPtr<Slot<Args...>> slot = CreateSlot<SlotType>(state, groupId, Move(inner));

            return Connection(slot);
        }

        /**
         * \brief Connects a non-const member function that consumes its connection before invoking.
         *
         * The signal does not own or track \p object. It must remain alive until the
         * connection is disconnected.
         *
         * \tparam T Object type.
         *
         * \param object Object receiving the callback.
         * \param function Member function to invoke once.
         * \param groupId Group used to order callback invocation.
         *
         * \return A RAII connection that manages the callback.
         */
        template <typename T>
        [[nodiscard]] Connection ConnectOnce(T& object, void (T::*function)(Args...), unsigned int groupId = 0)
        {
            using SlotType = OneShotSlot<Args...>;
            using InnerSlotType = ObjectSlot<T, Args...>;

            const SharedPtr<State> state = EnsureState();
            WeakPtr<SlotContainer> container(state);
            UniquePtr<InnerSlotType> inner = MakeUnique<InnerSlotType>(object, Move(function), container, groupId);
            SharedPtr<Slot<Args...>> slot = CreateSlot<SlotType>(state, groupId, Move(inner));

            return Connection(slot);
        }

        /**
         * \brief Connects a const member function that consumes its connection before invoking.
         *
         * The signal does not own or track \p object. It must remain alive until the
         * connection is disconnected.
         *
         * \tparam T Object type.
         *
         * \param object Object receiving the callback.
         * \param function Const member function to invoke once.
         * \param groupId Group used to order callback invocation.
         *
         * \return A RAII connection that manages the callback.
         */
        template <typename T>
        [[nodiscard]] Connection ConnectOnce(const T& object, void (T::*function)(Args...) const, unsigned int groupId = 0)
        {
            using SlotType = OneShotSlot<Args...>;
            using InnerSlotType = ConstantObjectSlot<T, Args...>;

            const SharedPtr<State> state = EnsureState();
            WeakPtr<SlotContainer> container(state);
            UniquePtr<InnerSlotType> inner = MakeUnique<InnerSlotType>(object, Move(function), container, groupId);
            SharedPtr<Slot<Args...>> slot = CreateSlot<SlotType>(state, groupId, Move(inner));

            return Connection(slot);
        }

        /**
         * \brief Invokes every active slot.
         *
         * \param args Arguments supplied to every callback.
         */
        void Emit(Args... args)
        {
            if (const SharedPtr<State> state = GetState())
                state->Emit(args...);
        }

        /**
         * \brief Tells whether the signal has at least one active connection.
         *
         * \return True when an active slot is connected, otherwise false.
         */
        bool IsBound() const
        {
            if (const SharedPtr<State> state = GetState())
                return state->IsBound();

            return false;
        }

        /**
         * \brief Disconnects every slot from the signal.
         */
        void Disconnect()
        {
            if (const SharedPtr<State> state = GetState())
                state->Disconnect();
        }

        /**
         * \brief Disconnects every slot in a group.
         *
         * \param groupId Group identifier to disconnect.
         */
        void Disconnect(unsigned int groupId)
        {
            if (const SharedPtr<State> state = GetState())
                state->Disconnect(groupId);
        }

    private:

        /**
         * \brief Returns the current registry, creating one for a moved-from signal if needed.
         *
         * \return The current registry.
         */
        SharedPtr<State> EnsureState()
        {
            if (!mState)
                mState = MakeShared<State>();

            return mState;
        }

        /**
         * \brief Returns a strong reference to the current registry.
         *
         * \return The current registry, or an empty pointer for a moved-from signal.
         */
        SharedPtr<State> GetState() const
        {
            return mState;
        }

        /**
         * \brief Creates and registers a new slot in a stable registry.
         *
         * \tparam T Concrete slot type.
         * \tparam SlotArgs Constructor argument types.
         *
         * \param state Registry that will own the slot.
         * \param groupId Group used to order callback invocation.
         * \param args Constructor arguments.
         *
         * \return The created slot.
         */
        template <typename T, typename... SlotArgs>
        SharedPtr<Slot<Args...>> CreateSlot(const SharedPtr<State>& state, unsigned int groupId, SlotArgs&&... args)
        {
            WeakPtr<SlotContainer> container(state);
            SharedPtr<Slot<Args...>> slot = MakeShared<T>(Forward<SlotArgs>(args)..., container, groupId);
            state->Register(slot, groupId);

            return slot;
        }

        SharedPtr<State> mState;
    };
}

#endif // COCKTAILENGINE_CORE_SIGNAL_DETAIL_SIGNALBASE_HPP
