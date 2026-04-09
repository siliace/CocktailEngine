#include <Cocktail/Core/System/FileSystem/Embedded/EmbdeddDirectoryIterator.hpp>

namespace Ck
{
    EmbeddedDirectoryIterator::EmbeddedDirectoryIterator(cmrc::embedded_filesystem fileSystem, Path directoryPath) :
        mFileSystem(fileSystem),
        mDirectoryPath(std::move(directoryPath))
    {
        mDirectoryPath.ToFormatInPlace(Path::Format::Generic);

        Utf8String utf8Path = Utf8String::Convert(mDirectoryPath.ToString());
        mIterable = mFileSystem.iterate_directory(reinterpret_cast<const char*>(utf8Path.GetData()));
    }

    void EmbeddedDirectoryIterator::Next()
    {
        if (mIterable == mIterable.end())
            return;

        ++mIterable;
        if (mIterable != mIterable.end())
        {
            AsciiString filename = (*mIterable).filename().c_str();

            mCurrentEntry = mDirectoryPath;
            mCurrentEntry.Join(String::Convert(filename));
        }
    }

    const Path& EmbeddedDirectoryIterator::GetPath() const
    {
        return !IsEnd() ? mCurrentEntry : Path::Empty;
    }

    PathInfo EmbeddedDirectoryIterator::GetPathInfo() const
    {
        return {};
    }

    bool EmbeddedDirectoryIterator::IsEnd() const
    {
        return mIterable != mIterable.end();
    }
}
