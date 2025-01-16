#ifndef COCKTAIL_CORE_SYSTEM_WINDOW_SYSTEMCURSORTYPE_HPP
#define COCKTAIL_CORE_SYSTEM_WINDOW_SYSTEMCURSORTYPE_HPP

namespace Ck
{
	/**
	 * \brief Enumeration of possible native system cursor types
	 * Refer to the following table to determine which cursor is available on which platform :
	 * ------------------------------------------------------------------------------
	 * | Type                                         | Linux | Mac OS X | Windows  |
	 * |:--------------------------------------------:|:-----:|:--------:|:--------:|
	 * | Ck::SystemCursorType::Arrow                  | yes   | yes      | yes      |
	 * | Ck::SystemCursorType::ArrowWait              | no    | no       | yes      |
	 * | Ck::SystemCursorType::Wait                   | yes   | no       | yes      |
	 * | Ck::SystemCursorType::Text                   | yes   | yes      | yes      |
	 * | Ck::SystemCursorType::Hand                   | yes   | yes      | yes      |
	 * | Ck::SystemCursorType::SizeHorizontal         | yes   | yes      | yes      |
	 * | Ck::SystemCursorType::SizeVertical           | yes   | yes      | yes      |
	 * | Ck::SystemCursorType::SizeTopLeftBottomRight | no    | yes*     | yes      |
	 * | Ck::SystemCursorType::SizeBottomLeftTopRight | no    | yes*     | yes      |
	 * | Ck::SystemCursorType::SizeLeft               | yes   | yes**    | yes**    |
	 * | Ck::SystemCursorType::SizeRight              | yes   | yes**    | yes**    |
	 * | Ck::SystemCursorType::SizeTop                | yes   | yes**    | yes**    |
	 * | Ck::SystemCursorType::SizeBottom             | yes   | yes**    | yes**    |
	 * | Ck::SystemCursorType::SizeTopLeft            | yes   | yes**    | yes**    |
	 * | Ck::SystemCursorType::SizeTopRight           | yes   | yes**    | yes**    |
	 * | Ck::SystemCursorType::SizeBottomLeft         | yes   | yes**    | yes**    |
	 * | Ck::SystemCursorType::SizeBottomRight        | yes   | yes**    | yes**    |
	 * | Ck::SystemCursorType::SizeAll                | yes   | no       | yes      |
	 * | Ck::SystemCursorType::Cross                  | yes   | yes      | yes      |
	 * | Ck::SystemCursorType::Help                   | yes   | yes*     | yes      |
	 * | Ck::SystemCursorType::NotAllowed             | yes   | yes      | yes      |
	 * |----------------------------------------------------------------------------|
	 * |                                                                            |
	 * | * These cursor types are undocumented so may not				            |
	 * |   be available on all versions, but have been tested on 10.13	            |
	 * |																            |
	 * | ** On Windows and macOS, double-headed arrows are used			            |
	 * ------------------------------------------------------------------------------
	 */
	enum class SystemCursorType
	{
		/**
		 * \brief Arrow cursor (default)
		 */
		Arrow,

		/**
		 * \brief Busy arrow cursor
		 */
		ArrowWait,

		/**
		 * \brief Busy arrow cursor
		 */
		Wait,

		/**
		 * \brief I-beam, cursor when hovering over a field allowing text entry
		 */
		Text,

		/**
		 * \brief Pointing hand cursor
		 */
		Hand,

		/**
		 * \brief Horizontal double arrow cursor
		 */
		SizeHorizontal,

		/**
		 * \brief Vertical double arrow cursor
		 */
		SizeVertical,

		/**
		 * \brief Double arrow cursor going from top-left to bottom-right
		 */
		SizeTopLeftBottomRight,

		/**
		 * \brief Double arrow cursor going from bottom-left to top-right
		 */
		SizeBottomLeftTopRight,

		/**
		 * \brief Left arrow cursor on Linux, same as SizeHorizontal on other platforms
		 */
		SizeLeft,

		/**
		 * \brief Right arrow cursor on Linux, same as SizeHorizontal on other platforms
		 */
		SizeRight,

		/**
		 * \brief Up arrow cursor on Linux, same as SizeVertical on other platforms
		 */
		SizeTop,

		/**
		 * \brief Down arrow cursor on Linux, same as SizeVertical on other platforms
		 */
		SizeBottom,

		/**
		 * \brief Top-left arrow cursor on Linux, same as SizeTopLeftBottomRight on other platforms
		 */
		SizeTopLeft,

		/**
		 * \brief Bottom-right arrow cursor on Linux, same as SizeTopLeftBottomRight on other platforms
		 */
		SizeBottomRight,

		/**
		 * \brief Bottom-left arrow cursor on Linux, same as SizeBottomLeftTopRight on other platforms
		 */
		SizeBottomLeft,

		/**
		 * \brief Top-right arrow cursor on Linux, same as SizeBottomLeftTopRight on other platforms
		 */
		SizeTopRight,

		/**
		 * \brief Combination of SizeHorizontal and SizeVertical
		 */
		SizeAll,

		/**
		 * \brief Cross-hair cursor
		 */
		Cross,

		/**
		 * \brief Help cursor
		 */
		Help,

		/**
		 * \brief Action not allowed cursor
		 */
		NotAllowed
	};
}

#endif // COCKTAIL_CORE_SYSTEM_WINDOW_SYSTEMCURSORTYPE_HPP
