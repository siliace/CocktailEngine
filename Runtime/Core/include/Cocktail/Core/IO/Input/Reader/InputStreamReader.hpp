#ifndef COCKTAIL_CORE_IO_INPUT_READER_INPUTSTREAMREADER_HPP
#define COCKTAIL_CORE_IO_INPUT_READER_INPUTSTREAMREADER_HPP

#include <Cocktail/Core/Export.hpp>
#include <Cocktail/Core/IO/Input/Reader/Reader.hpp>
#include <Cocktail/Core/IO/Input/Stream/InputStream.hpp>

namespace Ck
{
	class COCKTAIL_CORE_API InputStreamReader : public Reader
	{
	public:

		explicit InputStreamReader(InputStream& inputStream);

		bool Read(char& c) override;

		std::size_t Read(char* buffer, std::size_t length) override;

		bool HasCursor() const override;

		Uint64 Seek(Uint64 position) override;

		void Rewind() override;

		Uint64 Tell() const override;

		std::size_t GetSize() const override;

		bool IsEof() const override;

	private:

		InputStream& mInputStream;
	};
}

#endif // COCKTAIL_CORE_IO_INPUT_READER_INPUTSTREAMREADER_HPP
