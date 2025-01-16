#ifndef COCKTAIL_CORE_SYSTEM_CLIPBOARD_CLIPBOARDSERVICE_HPP
#define COCKTAIL_CORE_SYSTEM_CLIPBOARD_CLIPBOARDSERVICE_HPP

#include <utility>

namespace Ck
{
	/**
	 * \brief Service interacting with the system's clipboard
	 */
	class ClipboardService
	{
	public:

		/**
		 * \brief Destructor
		 */
		virtual ~ClipboardService() = default;

		/**
		 * \brief Get data from the system's clipboard to a memory buffer
		 * \param data The buffer where copy clipboard's content
		 * \param length The length of \p buffer
		 * \param offset The offset (in bytes) of the first byte to copy from the clipboard
		 * \return The amount of bytes copied from the clipboard to \p buffer
		 */
		virtual std::size_t Get(void* data, std::size_t length, std::size_t offset = 0) = 0;

		/**
		 * \brief Get the size (in bytes) of the content of the system's clipboard
		 * \return The size of the clipboard content
		 */
		virtual std::size_t GetSize() const = 0;

		/**
		 * \brief Set data to the system's clipboard from a memory buffer
		 * \param data The buffer from where copy clipboard's content
		 * \param length The length of \p buffer
		 */
		virtual void Set(const void* data, std::size_t length) = 0;

		/**
		 * \brief Empty the content of the clipboard
		 */
		virtual void Clear() = 0;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_CLIPBOARD_CLIPBOARDSERVICE_HPP
