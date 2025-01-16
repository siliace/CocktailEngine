#ifndef COCKTAIL_CORE_SYSTEM_CLIPBOARD_XLIB_CLIPBOARDSERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_CLIPBOARD_XLIB_CLIPBOARDSERVICE_HPP

#include <Cocktail/Core/System/Clipboard/ClipboardService.hpp>

namespace Ck::Detail::Xlib
{
    class ClipboardService : public Ck::ClipboardService
    {
    public:

		/**
		 * \brief Get data from the system's clipboard to a memory buffer
		 * \param data The buffer where copy clipboard's content
		 * \param length The length of \p buffer
		 * \param offset The offset (in bytes) of the first byte to copy from the clipboard
		 * \return The amount of bytes copied from the clipboard to \p buffer
		 */
		std::size_t Get(void* data, std::size_t length, std::size_t offset = 0) override;

		/**
		 * \brief Get the size (in bytes) of the content of the system's clipboard
		 * \return The size of the clipboard content
		 */
		std::size_t GetSize() const override;

		/**
		 * \brief Set data to the system's clipboard from a memory buffer
		 * \param data The buffer from where copy clipboard's content
		 * \param length The length of \p buffer
		 */
		void Set(const void* data, std::size_t length) override;

		/**
		 * \brief Empty the content of the clipboard
		 */
		void Clear() override;
    };
}

#endif // COCKTAIL_CORE_SYSTEM_CLIPBOARD_XLIB_CLIPBOARDSERVICE_HPP
