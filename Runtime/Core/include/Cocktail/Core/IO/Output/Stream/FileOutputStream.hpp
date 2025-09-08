#ifndef COCKTAIL_CORE_IO_FILEOUTPUTSTREAM_HPP
#define COCKTAIL_CORE_IO_FILEOUTPUTSTREAM_HPP

#include <string>

#include <Cocktail/Core/Export.hpp>
#include <Cocktail/Core/IO/Output/Stream/OutputStream.hpp>
#include <Cocktail/Core/System/FileSystem/File.hpp>

namespace Ck
{
	/**
	 * \brief Base interface for custom output streams
	 */
    class COCKTAIL_CORE_API FileOutputStream : public OutputStream
    {
    public:

        /**
         * \brief Constructor
         * Create a new instance of FileOutputStream
         *
         * \param file The file write into
         */
        explicit FileOutputStream(File& file);
	
        /**
         * \brief Write data to the file
    	*
		 * \param data A buffer containing data to write in the stream
		 * \param length The number of bytes to write to the stream
         */
        std::size_t Write(const void* data, std::size_t length) override;
        
        /**
         * \brief Flush every buffered write operations to this underlying file
         */
        void Flush() override;
		
	private:

        File* mFile;
    };
}

#endif // COCKTAIL_CORE_IO_FILEOUTPUTSTREAM_HPP