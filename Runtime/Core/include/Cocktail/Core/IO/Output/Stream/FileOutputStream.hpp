#ifndef COCKTAIL_CORE_IO_FILEOUTPUTSTREAM_HPP
#define COCKTAIL_CORE_IO_FILEOUTPUTSTREAM_HPP

#include <string>

#include <Cocktail/Core/Export.hpp>
#include <Cocktail/Core/IO/Output/Stream/OutputStream.hpp>
#include <Cocktail/Core/System/FileSystem/File.hpp>

namespace Ck
{
    /**
     * \brief
     */
    class COCKTAIL_CORE_API FileOutputStream : public OutputStream
    {
    public:
		
        /**
         * \brief Constructor
         * \param file The file to read from
         */
        explicit FileOutputStream(const Ref<File>& file);
	
        /**
         * \brief
         * \param
         * \param
         */
        std::size_t Write(const void* data, std::size_t length) override;
        
        /**
         * \brief
         */
        void Flush() override;
		
	private:

        Ref<File> mFile;
    };
}

#endif // COCKTAIL_CORE_IO_FILEOUTPUTSTREAM_HPP