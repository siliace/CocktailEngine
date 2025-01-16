#ifndef COCKTAIL_CORE_IO_INPUT_STREAM_FILEINPUTSTREAM_HPP
#define COCKTAIL_CORE_IO_INPUT_STREAM_FILEINPUTSTREAM_HPP

#include <Cocktail/Core/IO/Input/Stream/InputStream.hpp>
#include <Cocktail/Core/System/FileSystem/File.hpp>

namespace Ck
{
	/**
	 * \brief 
	 */
	class COCKTAIL_CORE_API FileInputStream : public InputStream
	{
	public:

		/**
		 * \brief Constructor
		 * \param file The file to read from
		 */
		explicit FileInputStream(const Ref<File>& file);

		/**
		 * \brief 
		 * \return 
		 */
		bool HasCursor() const override;

		/**
		 * \brief Read data from the stream
		 * Reading the stream might not totally fill the full \p length of the \p buffer if the stream does not contains enough data.
		 * After reading, the stream's reading position must be advanced by the amount of bytes actually read.
		 * \param buffer Buffer where copy data read from the stream
		 * \param length The number of bytes to read from the stream
		 * \return The number of bytes actually read
		 */
		std::size_t Read(void* buffer, std::size_t length) override;

		/**
		 * \brief Change the current reading position
		 * \param position The position to seek to from the beginning of the stream
		 * \return The position actually sought
		 */
		Uint64 Seek(Uint64 position) override;

		/**
		 * \brief Get the current reading position in the stream
		 * \return The current position
		 */
		Uint64 Tell() const override;

		/**
		 * \brief Moves the file position indicator to the beginning of the given file stream
		 * The function is equivalent to <pre>FileInputStream::Seek(0);</pre>
		 * \see InputStream::Seek
		 */
		void Rewind() override;
		
		/**
		 * \brief Get the size (in bytes) of the stream
		 * \return The total amount of bytes available in the stream
		 */
		std::size_t GetSize() const override;

		/**
		 * \brief 
		 * \return 
		 */
		bool IsEof() const override;

	private:

		Ref<File> mFile;
	};
}

#endif // COCKTAIL_CORE_IO_INPUT_STREAM_FILEINPUTSTREAM_HPP
