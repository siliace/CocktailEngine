#include <CocktailEngine/Core/System/FileSystem/Local/Unix/LocalDirectory.hpp>

namespace Ck::Detail::Unix
{
    LocalDirectory::LocalDirectory(const Path& path) :
        mPath(path)
    {
        mHandle = opendir(reinterpret_cast<const AnsiChar*>(path.ToString().GetData()));
    }

    LocalDirectory::~LocalDirectory()
    {
        closedir(mHandle);
    }

    Array<Path> LocalDirectory::GetContent() const
    {
        dirent64* result;
        Array<Path> content;

        while ((result = readdir64(mHandle)))
        {
            const TextChar* child = reinterpret_cast<const TextChar*>(result->d_name);

            if (StringUtils<TextChar, unsigned int>::Equal(child, CK_TEXT(".")))
                continue;

            if (StringUtils<TextChar, unsigned int>::Equal(child, CK_TEXT("..")))
                continue;

            content.Emplace(Path::Merge(mPath, child));
        }

        return content;
    }

    const Path& LocalDirectory::GetPath() const
    {
        return mPath;
    }

    void* LocalDirectory::GetSystemHandle() const
    {
        return (void*) mHandle;
    }
}
