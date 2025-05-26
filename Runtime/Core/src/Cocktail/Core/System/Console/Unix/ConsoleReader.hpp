#ifndef COCKTAIL_CORE_SYSTEM_CONSOLE_UNIX_CONSOLEREADER_HPP
#define COCKTAIL_CORE_SYSTEM_CONSOLE_UNIX_CONSOLEREADER_HPP

#include <Cocktail/Core/IO/Input/Reader/Reader.hpp>

namespace Ck::Detail::Unix
{
	/**
	 * \brief 
	 */
	class ConsoleReader : public Reader
	{
	public:

		/**
		 * \brief
		 * \param c
		 * \return
		 */
		bool Read(char& c) override;

		/**
		 * \brief 
		 * \param buffer 
		 * \param length 
		 * \return 
		 */
		std::size_t Read(char* buffer, std::size_t length) override;

		/**
		 * \brief 
		 * \return 
		 */
		bool HasCursor() const override;

		/**
		 * \brief 
		 * \param position 
		 * \return 
		 */
		Uint64 Seek(Uint64 position) override;

		/**
		 * \brief 
		 */
		void Rewind() override;

		/**
		 * \brief 
		 * \return 
		 */
		Uint64 Tell() const override;

		/**
		 * \brief 
		 * \return 
		 */
		std::size_t GetSize() const override;

		/**
		 * \brief 
		 * \return 
		 */
		bool IsEof() const override;
	};
}

#endif // COCKTAIL_CORE_SYSTEM_CONSOLE_UNIX_CONSOLEREADER_HPP
