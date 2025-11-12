#include <Cocktail/Core/Exception.hpp>
#include <Cocktail/Core/System/FileSystem/Path.hpp>
#include <Cocktail/Core/Utility/StringUtils.hpp>

namespace Ck
{
    namespace
    {
        void CheckElementSeparator(String& element, Path::Format format, TextChar systemSeparator)
        {
            String::SizeType lastIndex = element.GetLength() - 1;
            const bool hasTrailingSlash = element[lastIndex] == CK_CHAR('/');
            const bool hasTrailingBackSlash = element[lastIndex] == CK_CHAR('\\');

            switch (format)
            {
            case Path::Format::Auto:
                {
                    if (!hasTrailingSlash && !hasTrailingBackSlash)
                        element.Append(systemSeparator);
                }
                break;

            case Path::Format::Generic:
                {
                    if (!hasTrailingSlash && !hasTrailingBackSlash)
                    {
                        element.Append(CK_CHAR('/'));
                    }
                    else
                    {
                        element[lastIndex] = CK_CHAR('/');
                    }
                }
                break;

            case Path::Format::System:
                {
                    if (!hasTrailingSlash && !hasTrailingBackSlash)
                    {
                        element.Append(systemSeparator);
                    }
                    else
                    {
                        element[lastIndex] = systemSeparator;
                    }
                }
                break;
            }
        }
    }

    const Path Path::Empty = Path();

	Path Path::Parse(const String& string, Format format)
	{
        return Parse(string.GetData(), string.GetLength(), format);
	}

	Path Path::Parse(StringView string, Format format)
	{
        return Parse(string.GetData(), string.GetLength(), format);
	}

	Path Path::Parse(const TextChar* path, Format format)
	{
        return Parse(path, StringUtils<TextChar, SizeType>::GetLength(path), format);
	}

	Path Path::Parse(const TextChar* path, SizeType length, Format format)
    {
        String root;
        static const StringView NetRoot = CK_TEXT(R"(\\)");
        if (length >= 2 && StringUtils<TextChar, SizeType>::StartsWith(path, length, NetRoot.GetData(), NetRoot.GetLength()))
        {
            SizeType serverEnd = StringUtils<TextChar, SizeType>::FindFirst(path, length, CK_CHAR('\\'), 2).GetOr(length);
            SizeType shareEnd = StringUtils<TextChar, SizeType>::FindFirst(path, length, CK_CHAR('\\'), serverEnd + 1).GetOr(length);

            root = String(path, shareEnd);
        }
        else if (length >= 2 && std::isalpha(path[0]) && path[1] == CK_CHAR(':'))
        {
            SizeType firstSlash = StringUtils<TextChar, SizeType>::FindFirst(path, length, CK_CHAR('/')).GetOr(length);
            SizeType firstAntiSlash = StringUtils<TextChar, SizeType>::FindFirst(path, length, CK_CHAR('\\')).GetOr(length);
            SizeType firstSeparator = std::min(firstSlash, firstAntiSlash);

            root = String(path, firstSeparator < length ? firstSeparator + 1 : 2);
        }
        else if (path[0] == CK_CHAR('/'))
        {
            root = CK_TEXT("/");
        }

        String element;
        Array<String> elements;
        SizeType start = root.GetLength();
        for (SizeType i = start; i < length; i++)
        {
            const bool isEnd = i + 1 == length;
            const bool isSeparator = IsSeparator(path[i]);

            element.Append(path[i]);
            if ((isEnd || isSeparator) && !element.IsEmpty())
            {
                if (!(element.GetLength() == 1 && IsSeparator(element[0])))
                {
                    if (!isEnd)
                        CheckElementSeparator(element, format, SystemSeparator);

                    elements.Add(element);
                }
                element.Clear();
            }
        }

        return Path(std::move(root), std::move(elements), format);
    }

    bool Path::IsSeparator(TextChar character)
    {
        return character == CK_CHAR('\\') || character == CK_CHAR('/');
    }

    Path::Path(StringView string, Format format) :
        Path(string.GetData(), string.GetLength(), format)
    {
        /// Nothing
    }

    Path::Path(const String& string, Format format) :
        Path(string.GetData(), string.GetLength(), format)
    {
        /// Nothing
    }

    Path::Path(const TextChar* string, Format format) :
        Path(string, StringUtils<TextChar, SizeType>::GetLength(string), format)
    {
        /// Nothing
    }

    Path::Path(const TextChar* string, SizeType length, Format format) :
        mFormat(format)
    {
        if (string && length)
        {
            Path parsed = Parse(string, length, mFormat);

            mRoot = std::move(parsed.mRoot);
            mElements = std::move(parsed.mElements);
            mJoined.SetDirty();
        }
    }

    Path& Path::operator=(StringView string)
    {
        *this = Parse(string.GetData(), string.GetLength(), mFormat);
        return *this;
    }

    Path& Path::operator=(const String& string)
    {
        *this = Parse(string.GetData(), string.GetLength(), mFormat);
        return *this;
    }

    Path& Path::Join(StringView other)
    {
        return Join(other.GetData(), other.GetLength());
    }

    Path& Path::Join(const Path& other)
    {
        if (other.IsAbsolute())
        {
            *this = other;
        }
        else
        {
            mJoined.SetDirty();

            mElements.TryLast().Then([&](String& last) {
                CheckElementSeparator(last, mFormat, SystemSeparator);
            });

            for (auto i = 0; i < other.mElements.GetSize(); i++)
            {
                mElements.Add(other.mElements[i]);
                if (i + 1 < other.mElements.GetSize())
                    CheckElementSeparator(mElements.Last(), mFormat, SystemSeparator);
            }
        }

        return *this;
    }

    Path& Path::Join(const String& other)
    {
        return Join(other.GetData(), other.GetLength());
    }

    Path& Path::Join(const TextChar* other)
    {
        return Join(other, StringUtils<TextChar, SizeType>::GetLength(other));
    }

    Path& Path::Join(const TextChar* other, SizeType length)
    {
        return Join(Parse(other, length));
    }

    bool Path::IsEmpty() const
    {
        return mRoot.IsEmpty() && mElements.IsEmpty();
    }

    bool Path::IsAbsolute() const
    {
        return !mRoot.IsEmpty();
    }

    const String& Path::GetRoot() const
    {
        return mRoot;
    }

    Path Path::GetParent() const
    {
        if (mElements.IsEmpty())
            return *this;

        Array<String> elements = mElements.Slice(0, mElements.GetSize() - 1);
        elements.TryLast().Then([](String& last){
           if (last.EndsWith(CK_CHAR('/')) || last.EndsWith('\\'))
               last = last.SubString(0, last.GetLength() - 1);
        });

        return Path(mRoot, std::move(elements), mFormat);
    }

    Path Path::GetFilename() const
    {
        return mElements.TryLast().Map([](const String& last){
            return last.FindLast('.').Map([&](SizeType index) {
                return last.SubString(0, index);
            }).GetOr(String::Empty);
        }).GetOr(String::Empty);
    }

    String Path::GetExtension() const
    {
        return mElements.TryLast().Map([](const String& last){
            return last.FindLast('.').Map([&](SizeType index) {
                return last.SubString(index);
            }).GetOr(String::Empty);
        }).GetOr(String::Empty);
    }

    String Path::ToString() const
    {
        return mJoined.Get([this]() {
            String string;
            if (IsAbsolute())
            {
                string = mRoot;
                if (mRoot != CK_TEXT("/") && !mRoot.EndsWith(CK_CHAR('\\')))
                    string.Append(CK_CHAR('\\'));
            }

            for (SizeType i = 0; i < mElements.GetSize(); i++)
                string.Append(mElements[i]);

            return string;
        });
    }

    bool Path::operator==(const Path& rhs) const
    {
        return std::tie(mRoot, mElements) == std::tie(rhs.mRoot, rhs.mElements);
    }

    bool Path::operator!=(const Path& rhs) const
    {
        return !(*this == rhs);
    }

    Path::Path(String root, Array<String> elements, Format format):
        mRoot(std::move(root)),
        mElements(std::move(elements)),
        mFormat(format)
    {
        /// Nothing
    }
}
