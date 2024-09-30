#ifndef COCKTAIL_RENDERER_COMMAND_SYNCHRONIZATION_FENCE_HPP
#define COCKTAIL_RENDERER_COMMAND_SYNCHRONIZATION_FENCE_HPP

#include <Cocktail/Core/Signal/Observable.hpp>
#include <Cocktail/Core/Signal/Signal.hpp>

#include <Cocktail/Renderer/RenderDeviceObject.hpp>

namespace Ck::Renderer
{
	/**
	 * \brief Primitive to synchronize the GPU and the CPU
	 */
	class Fence : public Implements<Fence, RenderDeviceObject>, public Observable
	{
	public:

		/**
		 * \brief Wait for the fence to be signaled indefinitely
		 */
		virtual void Wait() = 0;

		/**
		 * \brief Wait for the fence to be signaled for a given amount of time
		 * \param timeout The timeout (in nanoseconds) to wait before the wait ends
		 * \return True if the fence has been signaled within the time to wait
		 */
		virtual bool Wait(Uint64 timeout) = 0;

		/**
		 * \brief Tell whether the fence is signaled
		 * \return True if the fence is signaled, false otherwise
		 */
		virtual bool IsSignaled() const = 0;

		/**
		 * \brief Reset the state of the fence to unsignaled
		 */
		virtual void Reset() = 0;

		/**
		 * \brief Get the signal emitted when the fence is signaled
		 * \return The signal
		 */
		virtual Signal<>& OnSignaled() = 0;
	};
}

#endif // COCKTAIL_RENDERER_COMMAND_SYNCHRONIZATION_FENCE_HPP
