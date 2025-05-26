#ifndef COCKTAIL_CORE_IO_INPUT_READER_BUFFEREDREADER_HPP
#define COCKTAIL_CORE_IO_INPUT_READER_BUFFEREDREADER_HPP

#include <memory>
#include <string>

#include <Cocktail/Core/Export.hpp>
#include <Cocktail/Core/IO/Input/Reader/Reader.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API BufferedReader : public Reader
	{
	public:

		/**
		 * \brief 
		 */
		static constexpr std::size_t DefaultBufferSize = 8192;

		/**
		 * \brief 
		 * \param reader
		 * \param bufferSize 
		 */
		explicit BufferedReader(Reader& reader, std::size_t bufferSize = DefaultBufferSize);

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
		std::string ReadLine();

		/**
		 * \brief 
		 * \return 
		 */
		std::string ReadAll();
		
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

	private:

		/**
		 * \brief 
		 * \return 
		 */
		std::size_t Advance();

		/**
		 * \brief 
		 * \param limit 
		 * \return 
		 */
		std::size_t FindLineEnd(std::size_t limit) const;
		
		/**
		 * \brief The reader to read from
		 */
		Reader& mIn;

		/**
		 * \brief The index of current read position. Must be <= mLimit
		 */
		std::size_t mPos;
		std::size_t mLimit;
		std::size_t mBufferSize;
		std::string mLineBuffer;
		std::unique_ptr<char[]> mBuffer;
	};
}

#endif // COCKTAIL_CORE_IO_INPUT_READER_BUFFEREDREADER_HPP
