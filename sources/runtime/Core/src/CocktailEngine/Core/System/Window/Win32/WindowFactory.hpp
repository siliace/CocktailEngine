#ifndef COCKTAILENGINE_CORE_SYSTEM_WINDOW_WIN32_WINDOWFACTORY_HPP
#define COCKTAILENGINE_CORE_SYSTEM_WINDOW_WIN32_WINDOWFACTORY_HPP

#include <CocktailEngine/Core/System/Win32/Windows.hpp>
#include <CocktailEngine/Core/System/Window/Win32/WindowClass.hpp>
#include <CocktailEngine/Core/System/Window/WindowFactory.hpp>
#include <CocktailEngine/Core/Utility/EnumMap.hpp>

namespace Ck::Detail::Win32
{
    /**
     * \brief Win32 implementation of the WindowFactory, registered as a singleton by the Windows service provider
     *
     * Owns the two things every window of the process shares: the registered window class
     * carrying the window procedure, and the cache of system cursors. Both outlive the
     * windows created from them, which is why the factory has to be kept alive as long as
     * any window is.
     */
    class WindowFactory final : public Ck::WindowFactory
    {
    public:

        /**
         * \brief Registers the shared window class and loads the supported system cursors
         *
         * \param instanceHandle Handle of the module the window class is registered for
         */
        explicit WindowFactory(HINSTANCE instanceHandle);

        UniquePtr<Ck::ImageCursor> CreateCursor(const Image& image, const Extent2D<unsigned int>& hotspot) override;

        UniquePtr<Ck::Window> CreateWindow(const WindowCreateInfo& createInfo) override;

        Ck::SystemCursor* LoadSystemCursor(SystemCursorType type) override;

        /**
         * \brief Tell whether a SystemCursorType is supported
         *
         * Win32 provides a shape for every type the engine knows about, either natively or
         * by reusing a close enough one.
         *
         * \param systemCursorType The cursor type to test
         *
         * \return True, for every cursor type
         */
        bool IsSystemCursorSupported(SystemCursorType systemCursorType) const override;

    private:

        HINSTANCE mInstanceHandle;

        /// Window class shared by every window created here, carrying the window procedure.
        WindowClass mWindowClass;

        /// System cursors loaded once and handed out as non-owning views by LoadSystemCursor.
        EnumMap<SystemCursorType, UniquePtr<Ck::SystemCursor>> mSystemCursors;
    };
}

#endif // COCKTAILENGINE_CORE_SYSTEM_WINDOW_WIN32_WINDOWFACTORY_HPP
