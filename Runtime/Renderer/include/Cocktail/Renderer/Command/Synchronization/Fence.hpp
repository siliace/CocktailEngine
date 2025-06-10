#ifndef COCKTAIL_RENDERER_COMMAND_SYNCHRONIZATION_FENCE_HPP
#define COCKTAIL_RENDERER_COMMAND_SYNCHRONIZATION_FENCE_HPP

#include <Cocktail/Core/Signal/Observable.hpp>
#include <Cocktail/Core/Signal/Signal.hpp>
#include <Cocktail/Core/Utility/Time/Duration.hpp>

#include <Cocktail/Renderer/RenderDeviceObject.hpp>

namespace Ck::Renderer
{
	/**
     * \brief Interface representing an opaque handle to a GPU fence object
	 * A Fence is a synchronization primitive between the CPU and the GPU.
	 * It meant to be submitted with CommandList, signaled by the GPU when CommandLists execution is over and waited by the CPU.
	 * Once a Fence has been signaled by the GPU, I should be reset before being submitted once more.
	 */
	class Fence : public RenderDeviceObject, public Observable
	{
	public:

		/**
		 * \brief Wait for the fence to be signaled indefinitely
		 */
		virtual void Wait() = 0;

		/**
		 * \brief Wait for the fence to be signaled for a given amount of time
		 * \param timeout The timeout to wait before the wait ends
		 * \return True if the fence has been signaled within the time to wait
		 */
		virtual bool Wait(const Duration& timeout) = 0;

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
