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

    Finder& Finder::Where(Predicate predicate)
    {
        if (predicate)
            mPredicates.Add(Move(predicate));

        return *this;
    }

    Finder& Finder::Exclude(const Path& path)
    {
        mExcluded.Add(path.ToFormat(Path::Format::Generic));

        return *this;
    }

    Finder& Finder::ExcludeDirectory(const Path& path)
    {
        mExcludedDirectories.Add(path.ToFormat(Path::Format::Generic));

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
        {
            if (!IsExcludedDirectory(source))
                content.Append(Get(source, mDepth, driver));
        }

        return content;
    }

    bool Finder::Matches(const Path& path) const
    {
        for (const Predicate& predicate : mPredicates)
        {
            if (!predicate(path))
                return false;
        }

        return true;
    }

    bool Finder::IsExcluded(const Path& path) const
    {
        return mExcluded.Contains(path.ToFormat(Path::Format::Generic));
    }

    bool Finder::IsExcludedDirectory(const Path& path) const
    {
        const Path normalizedPath = path.ToFormat(Path::Format::Generic);
        for (const Path& excludedDirectory : mExcludedDirectories)
        {
            if (normalizedPath.StartsWith(excludedDirectory))
                return true;
        }

        return false;
    }

    Array<Path> Finder::Get(const Path& source, unsigned int depth, FileSystemDriver& driver) const
    {
        Array<Path> content;
        UniquePtr<Directory> directory = driver.OpenDirectory(source);

        for (const Path& child : directory->GetContent())
        {
            if (child == Path::CurrentDirectory || child == Path::ParentDirectory)
                continue;

            const bool isFile = driver.IsFile(child);
            const bool isDirectory = !isFile && driver.IsDirectory(child);

            if (isFile)
            {
                if (mIgnoreFiles || IsExcluded(child) || !Matches(child))
                    continue;

                if (!mFileExtension.IsEmpty() && child.GetExtension() != mFileExtension)
                    continue;

                content.Add(child);
            }
            else if (isDirectory)
            {
                if (IsExcludedDirectory(child))
                    continue;

                if (!mIgnoreDirectories && !IsExcluded(child) && Matches(child))
                    content.Add(child);

                if (depth > 0)
                    content.Append(Get(child, depth - 1, driver));
            }
        }

        return content;
    }
}
