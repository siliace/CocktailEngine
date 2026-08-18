#include <catch2/catch_all.hpp>

#include <CocktailEngine/Core/Application/App.hpp>
#include <CocktailEngine/Core/Enum.hpp>
#include <CocktailEngine/Core/Image/Image.hpp>
#include <CocktailEngine/Core/Signal/Observable.hpp>
#include <CocktailEngine/Core/System/Concurrency/Thread.hpp>
#include <CocktailEngine/Core/System/Window/WindowFactory.hpp>

// Tests of the Window module written against its interfaces only, so the same cases run
// whatever windowing system the platform provides. Nothing here names a backend, and the
// one thing allowed to stand in for platform knowledge is
// WindowFactory::IsSystemCursorSupported, which exists precisely to be asked.
//
// A window manager answers a request when it feels like it, and is free to refuse. The
// cases whose outcome depends on that cooperation carry the [WindowManager] tag, so a run
// on a platform with an uncooperative or absent one can exclude them with ~[WindowManager]
// and still get everything the engine answers on its own.

using namespace Ck;

namespace Catch
{
    /**
     * \brief Teaches the test framework how to print a String
     *
     * Without this a failed comparison between two titles reports "{?} == {?}", which says
     * that something differs but not what.
     */
    template <>
    struct StringMaker<Ck::String>
    {
        static std::string convert(const Ck::String& value)
        {
            return std::string(reinterpret_cast<const char*>(value.GetData()), value.GetLength());
        }
    };
}

namespace
{
    /**
     * \brief Resolves the window factory, or skips the calling test when there is none
     *
     * A platform without a windowing system registers no factory, which is a reason to
     * skip rather than to fail: there is nothing to test, and nothing is broken.
     */
    WindowFactory* RequireWindowFactory()
    {
        WindowFactory* windowFactory = App::Resolve<WindowFactory>();
        if (!windowFactory)
            SKIP("No WindowFactory is registered, so this platform exposes no windowing system");

        return windowFactory;
    }

    /**
     * \brief Creates a window suitable for a test
     *
     * Decorated and resizable, because that is the configuration the size and position
     * requests below are about, and titled after the case so that a window left on screen
     * by a crashed run says where it came from.
     */
    UniquePtr<Window> MakeTestWindow(WindowFactory* windowFactory, const Extent2D<unsigned int>& size)
    {
        WindowCreateInfo createInfo;
        createInfo.Size = size;
        createInfo.Position = MakeExtent(120, 120);
        createInfo.Title = CK_TEXT("Cocktail window test");
        createInfo.Style = Flags<WindowStyle>::All();

        return windowFactory->CreateWindow(createInfo);
    }

    /**
     * \brief Pumps the window until a condition holds, or until it is clear it will not
     *
     * Waiting on the condition rather than on a fixed delay is what keeps these cases both
     * quick when the window manager answers immediately and reliable when it takes its
     * time. The pumping is also what delivers the events a backend needs to see before it
     * can report the new state.
     *
     * \return True if the condition held before the deadline, false otherwise
     */
    template <typename Predicate>
    bool PumpUntil(Window& window, Predicate&& predicate)
    {
        constexpr unsigned int Attempts = 150;

        for (unsigned int attempt = 0; attempt < Attempts; attempt++)
        {
            window.PollEvents();

            if (predicate())
                return true;

            Thread::SleepFor(Duration::Milliseconds(20));
        }

        return false;
    }

    /**
     * \brief Gives the window manager a chance to answer, without expecting anything
     *
     * For the cases that assert on what did not change, where there is no condition to
     * wait for.
     */
    void Pump(Window& window)
    {
        for (unsigned int attempt = 0; attempt < 10; attempt++)
        {
            window.PollEvents();
            Thread::SleepFor(Duration::Milliseconds(20));
        }
    }

    /**
     * \brief Builds a small opaque image usable as a cursor shape
     */
    Image MakeCursorImage(const Extent2D<unsigned int>& size)
    {
        LargeByteArray pixels(static_cast<std::size_t>(size.Width) * size.Height * 4, 0xFF);

        return Image(size, ImageRawFormat::Type::RedGreenBlueAlpha8, GammaSpace::sRGB, Move(pixels));
    }
}

TEST_CASE("A window exposes a native handle and reports itself alive", "[Window]")
{
    WindowFactory* windowFactory = RequireWindowFactory();
    UniquePtr<Window> window = MakeTestWindow(windowFactory, MakeExtent(640u, 480u));

    REQUIRE(window != nullptr);
    REQUIRE(window->GetSystemHandle() != nullptr);
    REQUIRE(window->PollEvents());
}

TEST_CASE("A window reports no size bound until it is given one", "[Window]")
{
    WindowFactory* windowFactory = RequireWindowFactory();
    UniquePtr<Window> window = MakeTestWindow(windowFactory, MakeExtent(640u, 480u));

    // A null extent is how the absence of a bound is spelled, so these must not report
    // whatever the windowing system happens to have lying around.
    REQUIRE(window->GetMinSize().Width == 0);
    REQUIRE(window->GetMinSize().Height == 0);
    REQUIRE(window->GetMaxSize().Width == 0);
    REQUIRE(window->GetMaxSize().Height == 0);
}

TEST_CASE("A window reports the size bounds it was given", "[Window]")
{
    WindowFactory* windowFactory = RequireWindowFactory();
    UniquePtr<Window> window = MakeTestWindow(windowFactory, MakeExtent(640u, 480u));

    SECTION("A minimum comes back unchanged")
    {
        window->SetMinSize(MakeExtent(320u, 240u));

        REQUIRE(window->GetMinSize().Width == 320);
        REQUIRE(window->GetMinSize().Height == 240);
    }

    SECTION("A maximum comes back unchanged")
    {
        window->SetMaxSize(MakeExtent(1280u, 1024u));

        REQUIRE(window->GetMaxSize().Width == 1280);
        REQUIRE(window->GetMaxSize().Height == 1024);
    }

    SECTION("Setting one bound leaves the other alone")
    {
        window->SetMinSize(MakeExtent(320u, 240u));
        window->SetMaxSize(MakeExtent(1280u, 1024u));

        REQUIRE(window->GetMinSize().Width == 320);
        REQUIRE(window->GetMinSize().Height == 240);
        REQUIRE(window->GetMaxSize().Width == 1280);
        REQUIRE(window->GetMaxSize().Height == 1024);
    }
}

TEST_CASE("A null extent removes a size bound of a window", "[Window]")
{
    WindowFactory* windowFactory = RequireWindowFactory();
    UniquePtr<Window> window = MakeTestWindow(windowFactory, MakeExtent(640u, 480u));

    SECTION("A minimum can be dropped")
    {
        window->SetMinSize(MakeExtent(320u, 240u));
        window->SetMinSize(MakeExtent(0u, 0u));

        REQUIRE(window->GetMinSize().Width == 0);
        REQUIRE(window->GetMinSize().Height == 0);
    }

    SECTION("A maximum can be dropped")
    {
        window->SetMaxSize(MakeExtent(1280u, 1024u));
        window->SetMaxSize(MakeExtent(0u, 0u));

        REQUIRE(window->GetMaxSize().Width == 0);
        REQUIRE(window->GetMaxSize().Height == 0);
    }

    SECTION("Dropping a maximum does not shrink the window to nothing")
    {
        window->SetMaxSize(MakeExtent(0u, 0u));
        Pump(*window);

        REQUIRE(window->GetSize().Width > 0);
        REQUIRE(window->GetSize().Height > 0);
    }
}

TEST_CASE("A window brought outside its size bounds is pulled back in", "[Window][WindowManager]")
{
    WindowFactory* windowFactory = RequireWindowFactory();

    SECTION("A minimum grows a smaller window, with a maximum already set")
    {
        UniquePtr<Window> window = MakeTestWindow(windowFactory, MakeExtent(200u, 200u));
        PumpUntil(*window, [&] {
            return window->GetSize() == MakeExtent(200u, 200u);
        });

        // The maximum first, so that the minimum is applied with both bounds in place:
        // that is the arrangement in which one bound can be lost to the other.
        window->SetMaxSize(MakeExtent(800u, 800u));
        window->SetMinSize(MakeExtent(400u, 400u));

        // Asserted before pumping on purpose. Setting a bound also publishes it to the
        // window manager, which enforces it on its own, so waiting here would pass whether
        // or not the window resized itself. What distinguishes the two is that the window
        // acts within the call while the window manager needs its own turn first.
        Extent2D<unsigned int> windowSize = window->GetSize();
        REQUIRE(windowSize.Width >= 400);
        REQUIRE(windowSize.Height >= 400);
    }

    SECTION("A maximum shrinks a larger window, with a minimum already set")
    {
        UniquePtr<Window> window = MakeTestWindow(windowFactory, MakeExtent(900u, 700u));
        PumpUntil(*window, [&] {
            return window->GetSize().Width == 900 && window->GetSize().Height == 700;
        });

        window->SetMinSize(MakeExtent(100u, 100u));
        window->SetMaxSize(MakeExtent(600u, 500u));

        Extent2D<unsigned int> windowSize = window->GetSize();
        REQUIRE(windowSize.Width <= 600);
        REQUIRE(windowSize.Height <= 500);
    }
}

TEST_CASE("A window round-trips its client size", "[Window][WindowManager]")
{
    WindowFactory* windowFactory = RequireWindowFactory();
    UniquePtr<Window> window = MakeTestWindow(windowFactory, MakeExtent(640u, 480u));

    SECTION("It reports the size it was created with")
    {
        const bool reported = PumpUntil(*window, [&] {
            return window->GetSize().Width == 640 && window->GetSize().Height == 480;
        });

        REQUIRE(reported);
    }

    SECTION("It reports the size it was asked to take")
    {
        window->SetSize(MakeExtent(800u, 600u));

        const bool resized = PumpUntil(*window, [&] {
            return window->GetSize().Width == 800 && window->GetSize().Height == 600;
        });

        REQUIRE(resized);
    }
}

TEST_CASE("A window round-trips its position", "[Window][WindowManager]")
{
    WindowFactory* windowFactory = RequireWindowFactory();
    UniquePtr<Window> window = MakeTestWindow(windowFactory, MakeExtent(400u, 300u));

    window->SetPosition(MakeExtent(320, 240));

    const bool moved = PumpUntil(*window, [&] {
        return window->GetPosition().Width == 320 && window->GetPosition().Height == 240;
    });

    REQUIRE(moved);
}

TEST_CASE("A window round-trips its title", "[Window]")
{
    WindowFactory* windowFactory = RequireWindowFactory();
    UniquePtr<Window> window = MakeTestWindow(windowFactory, MakeExtent(400u, 300u));

    const String title = CK_TEXT("A title the window has to give back");
    window->SetTitle(title);

    REQUIRE(window->GetTitle() == title);
}

TEST_CASE("A window tracks the cursor it displays", "[Window]")
{
    WindowFactory* windowFactory = RequireWindowFactory();
    UniquePtr<Window> window = MakeTestWindow(windowFactory, MakeExtent(400u, 300u));

    SECTION("It has none until it is given one")
    {
        REQUIRE(window->GetCursor() == nullptr);
    }

    SECTION("It keeps the one it is given")
    {
        SystemCursor* cursor = windowFactory->LoadSystemCursor(SystemCursorType::Arrow);
        REQUIRE(cursor != nullptr);

        window->SetCursor(cursor);

        REQUIRE(window->GetCursor() == cursor);
    }

    SECTION("It accepts having its cursor cleared")
    {
        window->SetCursor(windowFactory->LoadSystemCursor(SystemCursorType::Arrow));
        window->SetCursor(nullptr);

        REQUIRE(window->GetCursor() == nullptr);
    }
}

TEST_CASE("A window tracks the visibility of its cursor", "[Window]")
{
    WindowFactory* windowFactory = RequireWindowFactory();
    UniquePtr<Window> window = MakeTestWindow(windowFactory, MakeExtent(400u, 300u));

    SECTION("Hiding and showing it is reported")
    {
        window->SetCursorVisible(false);
        REQUIRE_FALSE(window->IsCursorVisible());

        window->SetCursorVisible(true);
        REQUIRE(window->IsCursorVisible());
    }

    SECTION("Hiding it without a cursor set is allowed")
    {
        REQUIRE(window->GetCursor() == nullptr);

        window->SetCursorVisible(false);
        REQUIRE_FALSE(window->IsCursorVisible());

        window->SetCursorVisible(true);
        REQUIRE(window->IsCursorVisible());
    }
}

TEST_CASE("A window factory serves the system cursors it claims to support", "[Window]")
{
    WindowFactory* windowFactory = RequireWindowFactory();

    SECTION("The arrow is available everywhere")
    {
        REQUIRE(windowFactory->IsSystemCursorSupported(SystemCursorType::Arrow));
        REQUIRE(windowFactory->LoadSystemCursor(SystemCursorType::Arrow) != nullptr);
    }

    SECTION("Every supported type can be loaded and knows itself")
    {
        for (SystemCursorType cursorType : Enum<SystemCursorType>::Values)
        {
            if (!windowFactory->IsSystemCursorSupported(cursorType))
                continue;

            SystemCursor* cursor = windowFactory->LoadSystemCursor(cursorType);

            REQUIRE(cursor != nullptr);
            REQUIRE(cursor->GetType() == cursorType);
            REQUIRE(cursor->GetSystemHandle() != nullptr);
        }
    }

    SECTION("Loading the same type twice yields the same cursor")
    {
        SystemCursor* first = windowFactory->LoadSystemCursor(SystemCursorType::Arrow);
        SystemCursor* second = windowFactory->LoadSystemCursor(SystemCursorType::Arrow);

        REQUIRE(first == second);
    }
}

TEST_CASE("An image cursor carries the hot spot it was built with", "[Window]")
{
    WindowFactory* windowFactory = RequireWindowFactory();

    const Extent2D<unsigned int> hotspot = MakeExtent(5u, 3u);
    UniquePtr<ImageCursor> cursor = windowFactory->CreateCursor(MakeCursorImage(MakeExtent(16u, 16u)), hotspot);

    REQUIRE(cursor != nullptr);
    REQUIRE(cursor->GetSystemHandle() != nullptr);
    REQUIRE(cursor->GetHotSpot().Width == hotspot.Width);
    REQUIRE(cursor->GetHotSpot().Height == hotspot.Height);
}

TEST_CASE("A window reports a resize to its listeners", "[Window]")
{
    WindowFactory* windowFactory = RequireWindowFactory();
    UniquePtr<Window> window = MakeTestWindow(windowFactory, MakeExtent(640u, 480u));

    // Consumers subscribe through their own Observable, not through the window, so the
    // test owns its connection the way the application does.
    Observable listener;

    unsigned int resizeCount = 0;
    Extent2D<unsigned int> lastReportedSize;
    listener.Connect(window->OnResizedEvent(), [&](const WindowResizedEvent& event) {
        resizeCount++;
        lastReportedSize = event.Size;
    });

    PumpUntil(*window, [&] {
        return window->GetSize().Width == 640 && window->GetSize().Height == 480;
    });

    resizeCount = 0;
    window->SetSize(MakeExtent(800u, 600u));

    const bool reported = PumpUntil(*window, [&] {
        return resizeCount > 0 && lastReportedSize.Width == 800 && lastReportedSize.Height == 600;
    });

    REQUIRE(reported);

    // The event has to agree with what the window answers once it has settled, which is
    // the whole point of reporting it.
    REQUIRE(lastReportedSize.Width == window->GetSize().Width);
    REQUIRE(lastReportedSize.Height == window->GetSize().Height);
}

TEST_CASE("A window names the window it reports about", "[Window]")
{
    WindowFactory* windowFactory = RequireWindowFactory();
    UniquePtr<Window> window = MakeTestWindow(windowFactory, MakeExtent(400u, 300u));

    Observable listener;

    Window* reportedSource = nullptr;
    listener.Connect(window->OnResizedEvent(), [&](const WindowResizedEvent& event) {
        reportedSource = event.Source;
    });

    window->SetSize(MakeExtent(520u, 400u));

    const bool reported = PumpUntil(*window, [&] {
        return reportedSource != nullptr;
    });

    REQUIRE(reported);
    REQUIRE(reportedSource == window.Get());
}
