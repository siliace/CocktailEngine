#include <CocktailEngine/Core/Utility/Finder.hpp>

namespace Ck
{
    Finder& Finder::From(const Path& path)
    {
        mSource.Add(path);

        return *this;
    }

    Finder& Finder::WithExtension(String fileExtension)
    {
        if (!fileExtension.IsEmpty() && fileExtension.First() != CK_CHAR('.'))
            fileExtension.Prepend(CK_CHAR('.'));

        mFileExtension = Move(fileExtension);

        return *this;
    }

    Finder& Finder::IgnoreFiles(bool ignoreFiles)
    {
        mIgnoreFiles = ignoreFiles;

        return *this;
    }

    Finder& Finder::IgnoreDirectories(bool ignoreDirectories)
    {
        mIgnoreDirectories = ignoreDirectories;

        return *this;
    }

    Finder& Finder::Recursively(bool recursive, unsigned int depth)
    {
        mDepth = recursive ? depth : 0;

        return *this;
    }

    Array<Path> Finder::Get(FileSystemDriver& driver) const
    {
        Array<Path> content;
        for (const Path& source : mSource)
            content.Append(Get(source, mDepth, driver));

        return content;
    }

    Array<Path> Finder::Get(const Path& source, unsigned int depth, FileSystemDriver& driver) const
    {
        Array<Path> content;
        UniquePtr<Directory> directory = driver.OpenDirectory(source);

        for (const Path& child : directory->GetContent())
        {
            if (child == CK_TEXT(".") || child == CK_TEXT(".."))
                continue;

            if (driver.IsFile(child) && !mIgnoreFiles)
            {
                if (!mFileExtension.IsEmpty() && child.GetExtension() != mFileExtension)
                    continue;

                content.Add(child);
            }
            else if (driver.IsDirectory(child))
            {
                if (!mIgnoreDirectories)
                    content.Add(child);

                if (depth > 0)
                    content.Append(Get(child, depth - 1, driver));
            }
        }

        return content;
    }
}
