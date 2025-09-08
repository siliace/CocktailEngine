#include <Cocktail/Core/System/FileSystem/URI.hpp>
#include <Cocktail/Core/Utility/StringUtils.hpp>

namespace Ck
{
    const StringView URI::SchemeSeparator = CK_TEXT("://");

	URI URI::Parse(const String& string)
	{
		return Parse(string.GetData(), string.GetLength());
	}

	URI URI::Parse(StringView string)
	{
		return Parse(string.GetData(), string.GetLength());
	}

	URI URI::Parse(const TextChar* path)
	{
		return Parse(path, StringUtils<TextChar, String::SizeType>::GetLength(path));
	}

    URI URI::Parse(const TextChar* string, String::SizeType length)
    {
        String::SizeType separatorIndex = StringUtils<TextChar, String::SizeType>::FindFirst(
            string, length, SchemeSeparator.GetData(), SchemeSeparator.GetLength()
        ).GetOr(length);

        String scheme;
        Path path;

        if (separatorIndex < length)
        {
            scheme = String(string, separatorIndex);
            ++separatorIndex;

            String::SizeType pathStart = separatorIndex + SchemeSeparator.GetLength() - 1;
            String::SizeType pathLength = length - separatorIndex - SchemeSeparator.GetLength() + 1;
            path = Path::Parse(string + pathStart, pathLength);
        }
        else
        {
            path = Path::Parse(string, length);
        }

        return URI(std::move(scheme), std::move(path));
    }

    URI::URI(Path path) :
        URI(String::Empty, std::move(path))
    {
        /// Nothing
    }

    URI::URI(StringView string) :
        URI(string.GetData(), string.GetLength())
    {
        /// Nothing
    }

    URI::URI(const String& string) :
        URI(string.GetData(), string.GetLength())
    {
        /// Nothing
    }

    URI::URI(const TextChar* string) :
        URI(string, StringUtils<TextChar, String::SizeType>::GetLength(string))
    {
        /// Nothing
    }

    URI::URI(const TextChar* string, String::SizeType length)
    {
        *this = Parse(string, length);
    }

    URI::URI(String scheme, Path path) :
        mScheme(std::move(scheme)),
        mPath(std::move(path))
    {
        /// Nothing
    }

    String URI::ToString() const
    {
        if (mScheme.IsEmpty())
            return mPath.ToString();

        return mScheme + CK_TEXT(":") + mPath.ToString();
    }

    const String& URI::GetScheme() const
    {
        return mScheme;
    }

    const Path& URI::GetPath() const
    {
        return mPath;
    }

    bool URI::operator==(const URI& other) const
    {
        return std::tie(mScheme, mPath) == std::tie(other.mScheme, other.mScheme);
    }

    bool URI::operator!=(const URI& other) const
    {
        return !(*this == other);
    }
}
