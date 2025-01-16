#ifndef COCKTAIL_CORE_SYSTEM_CLIPBOARD_CLIPBOARD_HPP
#define COCKTAIL_CORE_SYSTEM_CLIPBOARD_CLIPBOARD_HPP

#include <Cocktail/Core/Application/ServiceFacade.hpp>
#include <Cocktail/Core/System/Clipboard/ClipboardService.hpp>

namespace Ck
{
	/**
	 * \brief Facade to ClipboardService
	 */
	class COCKTAIL_CORE_API Clipboard : public ServiceFacade<ClipboardService>
	{
	public:

		/**
		 * \brief Get data from the system's clipboard to a memory buffer
		 * \param data The buffer where copy clipboard's content
		 * \param length The length of \p buffer
		 * \param offset The offset (in bytes) of the first byte to copy from the clipboard
		 * \return The amount of bytes copied from the clipboard to \p buffer
		 */
		static std::size_t Get(void* data, std::size_t length, std::size_t offset = 0);

		/**
		 * \brief Set data to the system's clipboard from a memory buffer
		 * \param data The buffer from where copy clipboard's content
		 * \param length The length of \p buffer
		 */
		static void Set(const void* data, std::size_t length);

		/**
		 * \brief Empty the content of the clipboard
		 */
		static void Clear();
	};
}

#endif // COCKTAIL_CORE_SYSTEM_CLIPBOARD_CLIPBOARD_HPP
