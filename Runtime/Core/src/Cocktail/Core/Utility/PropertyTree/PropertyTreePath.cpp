#include <Cocktail/Core/Utility/PropertyTree/PropertyTreePath.hpp>

namespace Ck
{
    PropertyTreePath::PropertyTreePath():
        mSeparator(CK_TEXT('.'))
    {
        /// Nothing
    }

    PropertyTreePath::PropertyTreePath(String path, TextChar separator):
        mSeparator(separator),
        mPath(std::move(path))
    {
        /// Nothing
    }

    PropertyTreePath::PropertyTreePath(const TextChar* path, TextChar separator):
        mSeparator(separator),
        mPath(path)
    {
        /// Nothing
    }

    bool PropertyTreePath::IsEmpty() const
    {
        return mPath.IsEmpty();
    }

    bool PropertyTreePath::IsSingle() const
    {
        return mPath.FindFirst(mSeparator).IsEmpty();
    }

    PropertyTreePath& PropertyTreePath::Append(const PropertyTreePath& other)
    {
        assert(mSeparator == other.mSeparator || other.IsEmpty() || other.IsSingle());
        if (!other.IsEmpty())
        {
            if (!IsEmpty())
                mPath.Append(mSeparator);

            mPath.Append(other.mPath);
        }

        return *this;
    }

    TextChar PropertyTreePath::GetSeparator() const
    {
        return mSeparator;
    }

    const String& PropertyTreePath::ToString() const
    {
        return mPath;
    }
}
