#ifndef COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_FILESYSTEMEXCEPTION_HPP
#define COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_FILESYSTEMEXCEPTION_HPP

#include <CocktailEngine/Core/GenericException.hpp>
#include <CocktailEngine/Core/String.hpp>

namespace Ck
{
    /**
     * \brief Portable conditions reported by generic filesystem operations.
     *
     * These codes describe filesystem failures independently from a native platform
     * error number. Callers should catch \ref FileSystemException by reference and
     * inspect \ref GenericException::GetErrorCode instead of interpreting the
     * diagnostic message.
     */
    enum class FileSystemErrorCode
    {
        NoSuchFileOrDirectory, /**< The requested path does not exist. */
        FileExists, /**< A path conflicts with an existing entry. */
        PermissionDenied, /**< Access to the path was denied. */
        IsADirectory, /**< A file operation received a directory. */
        NotADirectory, /**< A directory operation received a non-directory. */
        DirectoryNotEmpty, /**< An operation requires an empty directory. */
        FilenameTooLong, /**< A path or filename exceeds its limit. */
        TooManySymbolicLinkLevels, /**< Resolving a path exceeded symbolic-link depth. */
        ReadOnlyFileSystem, /**< The target filesystem does not permit writes. */
        NoSpaceOnDevice, /**< The target filesystem has no available space. */
        FileTooLarge, /**< The operation would exceed the file-size limit. */
        TooManyFilesOpen, /**< The process has too many open file handles. */
        TooManyFilesOpenInSystem, /**< The system has too many open file handles. */
        CrossDeviceLink, /**< An operation cannot cross filesystem boundaries. */
        IoError, /**< An input/output operation failed. */
        InvalidArgument, /**< An argument is invalid for the operation. */
        OperationNotPermitted, /**< The operation is not permitted. */
        DeviceOrResourceBusy, /**< A required device or resource is busy. */
        BadFileDescriptor, /**< A file descriptor is invalid. */
        NoSuchDevice, /**< The requested device does not exist. */
        NoSuchDeviceOrAddress, /**< The requested device or address does not exist. */
        InvalidSeek, /**< The requested seek operation is invalid. */
        ResultOutOfRange, /**< The operation produced an out-of-range result. */
        ValueTooLarge, /**< A value exceeds the supported range. */
        Interrupted, /**< The operation was interrupted before completion. */
        ResourceUnavailableTryAgain, /**< A temporary resource shortage requires a retry. */
        OperationCanceled, /**< The operation was canceled. */
        NotSupported, /**< The filesystem does not support the operation. */
        FunctionNotSupported, /**< The requested function is not supported. */
        NotEnoughMemory /**< The operation could not allocate required memory. */
    };

    /**
     * \brief Produces a diagnostic message for a \ref FileSystemErrorCode.
     *
     * Known codes are translated to their enumerator names. An out-of-domain value
     * produces a diagnostic fallback instead of relying on an invalid enum string.
     * \ref FileSystemErrorCode remains the programmatic error contract.
     */
    class FileSystemErrorCodeTranslator : public ErrorCodeTranslator<FileSystemErrorCode>
    {
    public:

        /**
         * \brief Converts a filesystem error code into its diagnostic identifier.
         *
         * \param errorCode Filesystem error code to convert.
         *
         * \return The name of \p errorCode in the engine text encoding, or a
         *         fallback message for an unknown value.
         */
        String TranslateErrorCode(FileSystemErrorCode errorCode) const override
        {
            switch (errorCode)
            {
                case FileSystemErrorCode::NoSuchFileOrDirectory: return CK_TEXT("NoSuchFileOrDirectory");
                case FileSystemErrorCode::FileExists: return CK_TEXT("FileExists");
                case FileSystemErrorCode::PermissionDenied: return CK_TEXT("PermissionDenied");
                case FileSystemErrorCode::IsADirectory: return CK_TEXT("IsADirectory");
                case FileSystemErrorCode::NotADirectory: return CK_TEXT("NotADirectory");
                case FileSystemErrorCode::DirectoryNotEmpty: return CK_TEXT("DirectoryNotEmpty");
                case FileSystemErrorCode::FilenameTooLong: return CK_TEXT("FilenameTooLong");
                case FileSystemErrorCode::TooManySymbolicLinkLevels: return CK_TEXT("TooManySymbolicLinkLevels");
                case FileSystemErrorCode::ReadOnlyFileSystem: return CK_TEXT("ReadOnlyFileSystem");
                case FileSystemErrorCode::NoSpaceOnDevice: return CK_TEXT("NoSpaceOnDevice");
                case FileSystemErrorCode::FileTooLarge: return CK_TEXT("FileTooLarge");
                case FileSystemErrorCode::TooManyFilesOpen: return CK_TEXT("TooManyFilesOpen");
                case FileSystemErrorCode::TooManyFilesOpenInSystem: return CK_TEXT("TooManyFilesOpenInSystem");
                case FileSystemErrorCode::CrossDeviceLink: return CK_TEXT("CrossDeviceLink");
                case FileSystemErrorCode::IoError: return CK_TEXT("IoError");
                case FileSystemErrorCode::InvalidArgument: return CK_TEXT("InvalidArgument");
                case FileSystemErrorCode::OperationNotPermitted: return CK_TEXT("OperationNotPermitted");
                case FileSystemErrorCode::DeviceOrResourceBusy: return CK_TEXT("DeviceOrResourceBusy");
                case FileSystemErrorCode::BadFileDescriptor: return CK_TEXT("BadFileDescriptor");
                case FileSystemErrorCode::NoSuchDevice: return CK_TEXT("NoSuchDevice");
                case FileSystemErrorCode::NoSuchDeviceOrAddress: return CK_TEXT("NoSuchDeviceOrAddress");
                case FileSystemErrorCode::InvalidSeek: return CK_TEXT("InvalidSeek");
                case FileSystemErrorCode::ResultOutOfRange: return CK_TEXT("ResultOutOfRange");
                case FileSystemErrorCode::ValueTooLarge: return CK_TEXT("ValueTooLarge");
                case FileSystemErrorCode::Interrupted: return CK_TEXT("Interrupted");
                case FileSystemErrorCode::ResourceUnavailableTryAgain: return CK_TEXT("ResourceUnavailableTryAgain");
                case FileSystemErrorCode::OperationCanceled: return CK_TEXT("OperationCanceled");
                case FileSystemErrorCode::NotSupported: return CK_TEXT("NotSupported");
                case FileSystemErrorCode::FunctionNotSupported: return CK_TEXT("FunctionNotSupported");
                case FileSystemErrorCode::NotEnoughMemory: return CK_TEXT("NotEnoughMemory");
            }

            return String::Format(CK_TEXT("UnknownFileSystemError(%u)"), static_cast<unsigned int>(errorCode));
        }
    };

    /**
     * \brief Runtime exception for a portable filesystem error.
     *
     * Use \ref GetErrorCode to distinguish the condition that caused the
     * exception. Native driver operations can still report their own error type;
     * this exception represents failures normalized by filesystem services.
     */
    using FileSystemException = GenericException<FileSystemErrorCode, FileSystemErrorCodeTranslator>;
}

#endif // COCKTAILENGINE_CORE_SYSTEM_FILESYSTEM_FILESYSTEMEXCEPTION_HPP
