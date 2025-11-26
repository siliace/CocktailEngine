#include <Cocktail/Core/Utility/StorageUtils.hpp>

namespace Ck
{
    void StorageUtils::MakeDirectories(const URI& uri, StorageService* storage)
    {
        FileUtils::MakeDirectories(uri.GetPath(), storage->ResolveDriver(uri.GetScheme()));
    }

    ByteArray StorageUtils::ReadFile(const URI& uri, StorageService* storage)
    {
        return FileUtils::ReadFile(uri.GetPath(), storage->ResolveDriver(uri.GetScheme()));
    }
    void StorageUtils::WriteFile(const URI& uri, ByteArrayView content, bool append, StorageService* storage)
    {
        return FileUtils::WriteFile(uri.GetPath(), content, append, storage->ResolveDriver(uri.GetScheme()));
    }
}
