#include <Cocktail/Core/System/Clipboard/Xlib/ClipboardService.hpp>

namespace Ck::Detail::Xlib
{
    ClipboardService::ClipboardService(AtomManager* atomManager, Display* display) :
        mDisplay(display)
    {
        // Get the atoms we need to make use of the clipboard
        mClipboardAtom = atomManager->GetAtom(CK_TEXT("CLIPBOARD"), false);
        mUtf8Atom = atomManager->GetAtom(CK_TEXT("UTF8_STRING"), false);
        mPropertyAtom = atomManager->GetAtom(CK_TEXT("XSEL_DATA"), false);

        // Create a hidden window that will broker our clipboard interactions with X
        mWindow = XCreateSimpleWindow(mDisplay, DefaultRootWindow(display), 0, 0, 1, 1, 0, 0, 0);

        // Register the events we are interested in
        XSelectInput(mDisplay, mWindow, SelectionNotify | SelectionClear | SelectionRequest);
    }

    std::size_t ClipboardService::Get(void* data, std::size_t length, std::size_t offset)
    {
        if (!data || length == 0)
            return 0;

        XConvertSelection(mDisplay, mClipboardAtom, mUtf8Atom, mPropertyAtom, mWindow, CurrentTime);

        XEvent event;
        for (unsigned int attempts = 0; attempts < 1000; attempts++)
        {
            XNextEvent(mDisplay, &event);

            if (event.type != SelectionNotify)
                continue;

            if (event.xselection.requestor != mWindow)
                continue;

            if (event.xselection.selection != mClipboardAtom)
                continue;

            if (event.xselection.property == 0)
                return 0;

            Atom actualType;
            int actualFormat;
            unsigned long nitems;
            unsigned long bytesAfter;
            unsigned char* buffer = nullptr;

            std::size_t totalCopied = 0;
            long readOffset = 0;

            do
            {
                if (buffer)
                {
                    XFree(buffer);
                    buffer = nullptr;
                }

                XGetWindowProperty(mDisplay, mWindow, mPropertyAtom, readOffset,
                                   1024, // lecture par chunk
                                   False, AnyPropertyType, &actualType, &actualFormat, &nitems, &bytesAfter, &buffer);

                if (!buffer)
                    break;

                // Conversion en taille réelle en bytes
                std::size_t byteSize = nitems * (actualFormat / 8);

                // Gestion offset utilisateur
                if (offset >= byteSize)
                {
                    offset -= byteSize;
                }
                else
                {
                    std::size_t available = byteSize - offset;
                    std::size_t toCopy = std::min(length - totalCopied, available);

                    Memory::Copy(static_cast<char*>(data) + totalCopied, buffer + offset, toCopy);

                    totalCopied += toCopy;
                    offset = 0;

                    if (totalCopied >= length)
                        break;
                }

                readOffset += nitems;

            } while (bytesAfter > 0);

            if (buffer)
                XFree(buffer);

            return totalCopied;
        }

        return 0;
    }

    std::size_t ClipboardService::GetSize() const
    {
        return 0;
    }

    void ClipboardService::Set(const void* data, std::size_t length)
    {
    }

    void ClipboardService::Clear()
    {
    }
}
