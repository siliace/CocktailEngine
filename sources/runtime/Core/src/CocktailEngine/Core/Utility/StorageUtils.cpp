#include <CocktailEngine/Core/Utility/StorageUtils.hpp>

namespace Ck
{
    void StorageUtils::MakeDirectories(const URI& uri, StorageService* storage)
    {
        FileUtils::MakeDirectories(uri.GetPath(), storage->ResolveDriver(uri));
    }

    ByteArray StorageUtils::ReadFile(const URI& uri, StorageService* storage)
    {
        return FileUtils::ReadFile(uri.GetPath(), storage->ResolveDriver(uri));
    }

    void StorageUtils::WriteFile(const URI& uri, ByteArrayView content, StorageService* storage)
    {
        FileUtils::WriteFile(uri.GetPath(), content, storage->ResolveDriver(uri));
    }

    void StorageUtils::AppendFile(const URI& uri, ByteArrayView content, StorageService* storage)
    {
        FileUtils::AppendFile(uri.GetPath(), content, storage->ResolveDriver(uri));
    }
}
