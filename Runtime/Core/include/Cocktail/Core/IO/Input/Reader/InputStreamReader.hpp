#ifndef COCKTAIL_CORE_IO_INPUT_READER_INPUTSTREAMREADER_HPP
#define COCKTAIL_CORE_IO_INPUT_READER_INPUTSTREAMREADER_HPP

#include <Cocktail/Core/Export.hpp>
#include <Cocktail/Core/IO/Input/Reader/Reader.hpp>
#include <Cocktail/Core/IO/Input/Stream/InputStream.hpp>
#include <Cocktail/Core/Utility/StringConvertion.hpp>

namespace Ck
{
	class COCKTAIL_CORE_API InputStreamReader : public Reader
	{
	public:

		explicit InputStreamReader(InputStream& inputStream, EncodingMode encodingMode = EncodingMode::Utf8);

		bool Read(TextChar& c) override;

		std::size_t Read(TextChar* buffer, std::size_t length) override;

		bool HasCursor() const override;

		Uint64 Seek(Uint64 position) override;

		void Rewind() override;

		Uint64 Tell() const override;

		std::size_t GetSize() const override;

		bool IsEof() const override;

		EncodingMode GetEncodingMode() const override
		{
			return mEncodingMode;
		}

	private:

		InputStream& mInputStream;
		EncodingMode mEncodingMode;
	};
}

#endif // COCKTAIL_CORE_IO_INPUT_READER_INPUTSTREAMREADER_HPP
