project(Core)

cocktail_source_directory(CORE_FILES    
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Array.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Cocktail.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Color.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/DataType.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Enum.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Exception.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Export.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Extent2D.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Extent3D.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Flags.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/String.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Color.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/DataType.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Exception.cpp
)

cocktail_source_directory(CORE_FILES    
    FOLDER "Application"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Application/App.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Application/Application.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Application/ServiceContainer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Application/ServiceFacade.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Application/ServiceProvider.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Application/App.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Application/Application.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Application/ServiceContainer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Application/ServiceProvider.cpp
)

cocktail_source_directory(CORE_FILES    
    FOLDER "Application/Detail"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Application/Detail/CallableServiceBinding.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Application/Detail/InstanceServiceBinding.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Application/Detail/ServiceBinding.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Application/Detail/ServiceBindingBase.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Application/Detail/ServiceFacadeBase.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Application/Detail/SingletonServiceBinding.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Application/Detail/ServiceFacadeBase.cpp
)

cocktail_source_directory(CORE_FILES    
    FOLDER "Asset"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Asset/AssetExporter.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Asset/AssetImporter.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Asset/AssetLoader.hpp
)

cocktail_source_directory(CORE_FILES    
    FOLDER "Image"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Image/Image.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Image/ImageLoader.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Image/ImageServiceProvider.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Image/PixelFormat.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Image/Image.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Image/ImageLoader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Image/ImageServiceProvider.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Image/PixelFormat.cpp
)

cocktail_source_directory(CORE_FILES    
    FOLDER "Image/StbLoader"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Image/StbLoader/StbImageExporter.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Image/StbLoader/StbImageExporter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Image/StbLoader/StbImageImporter.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Image/StbLoader/StbImageImporter.cpp
)

cocktail_source_directory(CORE_FILES
    FOLDER "IO"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/IO/EncodingMode.hpp
)

cocktail_source_directory(CORE_FILES    
    FOLDER "IO/Input/Reader"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/IO/Input/Reader/Reader.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/IO/Input/Reader/InputStreamReader.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/IO/Input/Reader/BufferedReader.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/IO/Input/Reader/InputStreamReader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/IO/Input/Reader/BufferedReader.cpp
)

cocktail_source_directory(CORE_FILES    
    FOLDER "IO/Input/Stream"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/IO/Input/Stream/FileInputStream.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/IO/Input/Stream/MemoryInputStream.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/IO/Input/Stream/InputStream.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/IO/Input/Stream/FileInputStream.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/IO/Input/Stream/MemoryInputStream.cpp
)

cocktail_source_directory(CORE_FILES    
    FOLDER "IO/Output/Stream"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/IO/Output/Stream/FileOutputStream.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/IO/Output/Stream/MemoryOutputStream.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/IO/Output/Stream/OutputStream.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/IO/Output/Stream/FileOutputStream.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/IO/Output/Stream/MemoryOutputStream.cpp
)

cocktail_source_directory(CORE_FILES    
    FOLDER "IO/Output/Writer"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/IO/Output/Writer/Writer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/IO/Output/Writer/FileWriter.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/IO/Output/Writer/FileWriter.cpp
)

cocktail_source_directory(CORE_FILES    
    FOLDER "Log"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Log/ConsoleLogChannel.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Log/Log.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Log/LogCategory.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Log/LogChannel.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Log/LogEntry.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Log/LogLevel.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Log/LogManager.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Log/LogServiceProvider.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Log/ConsoleLogChannel.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Log/Log.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Log/LogCategory.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Log/LogChannel.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Log/LogManager.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Log/LogServiceProvider.cpp
)

cocktail_source_directory(CORE_FILES    
    FOLDER "Math"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Math/Angle.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Math/BoxCorner.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Math/Constants.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Math/EulerAngles.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Math/Intersection.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Math/Plane.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Math/Quaternion.hpp
)

cocktail_source_directory(CORE_FILES    
    FOLDER "Math/Matrix"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Math/Matrix/Matrix.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Math/Matrix/Matrix3.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Math/Matrix/Matrix4.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Math/Matrix/SquareMatrix.hpp
)

cocktail_source_directory(CORE_FILES    
    FOLDER "Math/Polygon"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Math/Polygon/Polygon.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Math/Polygon/Rectangle.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Math/Polygon/Triangle.hpp
)

cocktail_source_directory(CORE_FILES    
    FOLDER "Math/Vector"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Math/Vector/Vector2.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Math/Vector/Vector3.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Math/Vector/Vector4.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Math/Vector/Vector.hpp
)

cocktail_source_directory(CORE_FILES    
    FOLDER "Math/Volume"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Math/Volume/Box.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Math/Volume/Cylinder.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Math/Volume/Frustum.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Math/Volume/Sphere.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Math/Volume/Volume.hpp
)

cocktail_source_directory(CORE_FILES    
    FOLDER "Memory/Allocator"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Memory/Allocator/AllocatorUtils.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Memory/Allocator/BumpAllocator.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Memory/Allocator/SizedHeapAllocator.hpp
)

cocktail_source_directory(CORE_FILES    
    FOLDER "Signal"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Signal/Connection.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Signal/Observable.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Signal/Signal.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Signal/Connection.cpp
)

cocktail_source_directory(CORE_FILES    
    FOLDER "Signal/Detail"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Signal/Detail/ObservableBase.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Signal/Detail/SignalBase.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Signal/Detail/Slot.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Signal/Detail/SlotContainer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Signal/Detail/SlotState.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Signal/Detail/SlotState.cpp
)

cocktail_source_directory(CORE_FILES    
    FOLDER "System"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/SystemError.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/SystemServiceProvider.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/SystemServiceProvider.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/SystemServiceProviderImpl.hpp
)

if (WIN32)
	cocktail_source_directory(CORE_FILES    
		FOLDER "System/Win32"
		SRC
			${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Win32/SystemError.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Win32/Windows.hpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Win32/WindowsServiceProvider.hpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Win32/WindowsServiceProvider.cpp
	)
endif ()

if (UNIX)
    cocktail_source_directory(CORE_FILES
        FOLDER "System/Unix"  
        SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Unix/SystemError.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Unix/UnixServiceProvider.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Unix/UnixServiceProvider.cpp
    )

    cocktail_source_directory(CORE_FILES
        FOLDER "System/Unix/Xlib"
        SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Unix/Xlib/Xlib.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Unix/Xlib/XlibServiceProvider.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Unix/Xlib/XlibServiceProvider.cpp
    )
endif ()

cocktail_source_directory(CORE_FILES    
    FOLDER "System/Clipboard"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Clipboard/Clipboard.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Clipboard/ClipboardService.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Clipboard/Clipboard.cpp
)

if (WIN32)
    cocktail_source_directory(CORE_FILES    
        FOLDER "System/Clipboard/Win32"
        SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Clipboard/Win32/ClipboardService.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Clipboard/Win32/ClipboardService.cpp
    )
endif ()

if (LINUX)
    cocktail_source_directory(CORE_FILES    
        FOLDER "System/Clipboard/Xlib"
        SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Clipboard/Xlib/ClipboardService.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Clipboard/Xlib/ClipboardService.cpp
    )
endif ()

cocktail_source_directory(CORE_FILES    
    FOLDER "System/Concurrency"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Concurrency/AffinityMaskBuilder.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Concurrency/BlockingQueue.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Concurrency/NullMutex.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Concurrency/Runnable.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Concurrency/SpinMutex.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Concurrency/Thread.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Concurrency/ThreadManager.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Concurrency/AffinityMaskBuilder.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Concurrency/NullMutex.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Concurrency/SpinMutex.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Concurrency/Thread.cpp
)

if (WIN32)
    cocktail_source_directory(CORE_FILES    
        FOLDER "System/Concurrency/Win32"
        SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Concurrency/Win32/Thread.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Concurrency/Win32/Thread.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Concurrency/Win32/ThreadManager.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Concurrency/Win32/ThreadManager.cpp
    )
elseif (LINUX)
    cocktail_source_directory(CORE_FILES    
        FOLDER "System/Concurrency/Pthread"
        SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Concurrency/Pthread/PthreadErrorCategory.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Concurrency/Pthread/PthreadErrorCategory.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Concurrency/Pthread/Thread.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Concurrency/Pthread/Thread.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Concurrency/Pthread/ThreadManager.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Concurrency/Pthread/ThreadManager.cpp
    )
endif ()

cocktail_source_directory(CORE_FILES    
    FOLDER "System/Console"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Console/Console.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Console/ConsoleColor.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Console/ConsoleService.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Console/ConsoleStyle.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Console/Console.cpp
)

if (WIN32)
    cocktail_source_directory(CORE_FILES    
        FOLDER "System/Console/Win32"
        SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Console/Win32/ConsoleReader.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Console/Win32/ConsoleReader.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Console/Win32/ConsoleService.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Console/Win32/ConsoleService.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Console/Win32/ConsoleWriter.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Console/Win32/ConsoleWriter.cpp
    )
elseif (LINUX)
    cocktail_source_directory(CORE_FILES    
        FOLDER "System/Console/Unix"
        SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Console/Unix/ConsoleReader.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Console/Unix/ConsoleReader.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Console/Unix/ConsoleService.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Console/Unix/ConsoleService.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Console/Unix/ConsoleWriter.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Console/Unix/ConsoleWriter.cpp
    )
endif ()

cocktail_source_directory(CORE_FILES    
    FOLDER "System/FileSystem"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/FileSystem/Directory.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/FileSystem/DirectoryWatcher.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/FileSystem/File.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/FileSystem/FileCursorMode.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/FileSystem/FileLock.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/FileSystem/FileOpenFlags.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/FileSystem/FileSystemDriver.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/FileSystem/Path.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/FileSystem/Storage.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/FileSystem/StorageService.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/FileSystem/URI.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Path.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Storage.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/StorageService.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/URI.cpp
)

cocktail_source_directory(CORE_FILES    
    FOLDER "System/FileSystem/Embedded"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/FileSystem/Embedded/EmbeddedFileSystemDriver.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Embedded/EmbdeddDirectory.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Embedded/EmbdeddDirectory.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Embedded/EmbdeddFile.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Embedded/EmbdeddFile.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Embedded/EmbeddedFileSystemDriver.cpp
)

cocktail_source_directory(CORE_FILES    
    FOLDER "System/FileSystem/Local"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/FileSystem/Local/LocalFileSystem.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/FileSystem/Local/LocalFileSystemDriver.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/FileSystem/Local/LocalFileSystemService.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Local/LocalFileSystem.cpp
)

if (WIN32)
    cocktail_source_directory(CORE_FILES    
        FOLDER "System/FileSystem/Local/Win32"
        SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Local/Win32/LocalDirectory.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Local/Win32/LocalDirectory.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Local/Win32/LocalDirectoryWatcher.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Local/Win32/LocalDirectoryWatcher.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Local/Win32/LocalFile.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Local/Win32/LocalFile.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Local/Win32/LocalFileLock.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Local/Win32/LocalFileLock.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Local/Win32/LocalFileSystemDriver.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Local/Win32/LocalFileSystemDriver.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Local/Win32/LocalFileSystemService.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Local/Win32/LocalFileSystemService.cpp
    )
elseif (LINUX)
    cocktail_source_directory(CORE_FILES    
        FOLDER "System/FileSystem/Local/Unix"
        SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Local/Unix/LocalDirectory.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Local/Unix/LocalDirectory.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Local/Unix/LocalDirectoryWatcher.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Local/Unix/LocalDirectoryWatcher.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Local/Unix/LocalFile.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Local/Unix/LocalFile.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Local/Unix/LocalFileLock.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Local/Unix/LocalFileLock.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Local/Unix/LocalFileSystemDriver.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Local/Unix/LocalFileSystemDriver.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Local/Unix/LocalFileSystemService.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/FileSystem/Local/Unix/LocalFileSystemService.cpp
    )
endif ()

cocktail_source_directory(CORE_FILES    
    FOLDER "System/Joystick"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Joystick/Joystick.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Joystick/JoystickAxis.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Joystick/JoystickCapabilities.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Joystick/JoystickEvent.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Joystick/JoystickManager.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Joystick/JoystickPovPosition.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Joystick/Joystick.cpp
)

if (WIN32)
    cocktail_source_directory(CORE_FILES    
        FOLDER "System/Joystick/Win32"
        SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Joystick/Win32/Joystick.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Joystick/Win32/Joystick.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Joystick/Win32/JoystickManager.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Joystick/Win32/JoystickManager.cpp
    )
endif ()

cocktail_source_directory(CORE_FILES    
    FOLDER "System/Keyboard"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Keyboard/Keyboard.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Keyboard/KeyboardEvent.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Keyboard/KeyboardKey.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Keyboard/KeyboardService.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Keyboard/Keyboard.cpp
)

if (WIN32)
    cocktail_source_directory(CORE_FILES    
        FOLDER "System/Keyboard/Win32"
        SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Keyboard/Win32/KeyboardService.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Keyboard/Win32/KeyboardService.cpp
    )
endif ()

if (LINUX)
    cocktail_source_directory(CORE_FILES    
        FOLDER "System/Keyboard/Xlib"
        SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Keyboard/Xlib/KeyboardService.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Keyboard/Xlib/KeyboardService.cpp
    )
endif ()

cocktail_source_directory(CORE_FILES
    FOLDER "System/Library"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Library/Library.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Library/LibraryService.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Library/Library.cpp
)

if (WIN32)
    cocktail_source_directory(CORE_FILES
        FOLDER "System/Library/Win32"
        SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Library/Win32/Library.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Library/Win32/Library.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Library/Win32/LibraryService.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Library/Win32/LibraryService.cpp
    )
elseif (LINUX)
cocktail_source_directory(CORE_FILES
    FOLDER "System/Library/Unix"
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Library/Unix/Library.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Library/Unix/Library.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Library/Unix/LibraryService.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Library/Unix/LibraryService.cpp
)
endif ()

cocktail_source_directory(CORE_FILES    
    FOLDER "System/Monitor"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Monitor/Monitor.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Monitor/MonitorService.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Monitor/VideoMode.hpp
)

if (WIN32)
    cocktail_source_directory(CORE_FILES    
        FOLDER "System/Monitor/Win32"
        SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Monitor/Win32/Monitor.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Monitor/Win32/Monitor.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Monitor/Win32/MonitorService.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Monitor/Win32/MonitorService.cpp
    )
endif ()

if (LINUX)
    cocktail_source_directory(CORE_FILES    
        FOLDER "System/Monitor/Xlib"
        SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Monitor/Xlib/Monitor.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Monitor/Xlib/Monitor.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Monitor/Xlib/MonitorService.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Monitor/Xlib/MonitorService.cpp
    )
endif ()

cocktail_source_directory(CORE_FILES    
    FOLDER "System/Mouse"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Mouse/Mouse.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Mouse/MouseButton.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Mouse/MouseEvent.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Mouse/MouseService.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Mouse/MouseWheel.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Mouse/Mouse.cpp
)

if (WIN32)
    cocktail_source_directory(CORE_FILES    
        FOLDER "System/Mouse/Win32"
        SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Mouse/Win32/MouseService.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Mouse/Win32/MouseService.cpp
    )
endif ()

if (LINUX)
    cocktail_source_directory(CORE_FILES    
        FOLDER "System/Mouse/Xlib"
        SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Mouse/Xlib/MouseService.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Mouse/Xlib/MouseService.cpp
    )
endif ()

cocktail_source_directory(CORE_FILES    
    FOLDER "System/Network"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Network/Socket.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Network/SocketPoller.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Network/TcpClient.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Network/TcpServer.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Network/UdpSocket.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Network/Socket.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Network/SocketAddress.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Network/SocketAddress.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Network/SocketHeader.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Network/SocketPoller.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Network/TcpClient.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Network/TcpServer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Network/UdpSocket.cpp
)

if (WIN32)
    cocktail_source_directory(CORE_FILES    
        FOLDER "System/Network/Win32"
        SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Network/Win32/SocketPollerImpl.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Network/Win32/SocketPollerImpl.cpp
    )
endif ()

if (LINUX)
    cocktail_source_directory(CORE_FILES    
        FOLDER "System/Network/Unix"
        SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Network/Unix/SocketPollerImpl.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Network/Unix/SocketPollerImpl.cpp
    )
endif ()

cocktail_source_directory(CORE_FILES    
    FOLDER "System/Network/Address"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Network/Address/IpAddress.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Network/Address/IpAddressV4.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Network/Address/IpAddressV6.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Network/Address/IpAddress.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Network/Address/IpAddressV4.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Network/Address/IpAddressV6.cpp
)

cocktail_source_directory(CORE_FILES    
    FOLDER "System/Window"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Window/Cursor.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Window/ImageCursor.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Window/SystemCursor.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Window/SystemCursorType.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Window/Window.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Window/WindowCreateInfo.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Window/WindowDisplayMode.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Window/WindowEvent.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Window/WindowFactory.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/System/Window/WindowStyle.hpp
)

if (WIN32)
	cocktail_source_directory(CORE_FILES    
		FOLDER "System/Window/Win32"
		SRC
			${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Window/Win32/ImageCursor.hpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Window/Win32/ImageCursor.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Window/Win32/SystemCursor.hpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Window/Win32/SystemCursor.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Window/Win32/Window.hpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Window/Win32/Window.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Window/Win32/WindowClass.hpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Window/Win32/WindowClass.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Window/Win32/WindowEventDispatcher.hpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Window/Win32/WindowEventDispatcher.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Window/Win32/WindowFactory.hpp
			${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Window/Win32/WindowFactory.cpp
	)
endif ()

if (UNIX)
    cocktail_source_directory(CORE_FILES
        FOLDER "System/Window/Xlib"
        SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Window/Xlib/AtomManager.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Window/Xlib/AtomManager.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Window/Xlib/ImageCursor.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Window/Xlib/ImageCursor.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Window/Xlib/SystemCursor.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Window/Xlib/SystemCursor.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Window/Xlib/Window.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Window/Xlib/Window.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Window/Xlib/WindowFactory.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/System/Window/Xlib/WindowFactory.cpp
    )
endif ()

cocktail_source_directory(CORE_FILES    
    FOLDER "Utility"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/AcyclicGraph.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/ByteArray.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/ByteArrayView.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/CharUtils.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/CompositeKey.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/EnumMap.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/ExceptionUtils.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/FileUtils.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/Finder.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/FixedArray.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/FunctionTraits.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/LazyValue.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/ObjectPool.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/Optional.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/ParameterPack.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/StringConvertion.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/StringFormater.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/StringUtils.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/TranslatorCast.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Utility/ByteArray.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Utility/ByteArrayView.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Utility/ExceptionUtils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Utility/FileUtils.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Utility/Finder.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Utility/StringConvertion.cpp
)

cocktail_source_directory(CORE_FILES
    FOLDER "Utility/Encoding"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/Encoding/AsciiEncoder.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/Encoding/Utf8Encoder.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/Encoding/Utf16Encoder.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/Encoding/Utf32Encoder.hpp
)

cocktail_source_directory(CORE_FILES    
    FOLDER "Utility/PropertyTree"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/PropertyTree/PropertyTree.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/PropertyTree/PropertyTreeCustomNode.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/PropertyTree/PropertyTreeElement.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/PropertyTree/PropertyTreeNode.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/PropertyTree/PropertyTreePath.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/PropertyTree/PropertyTreeSequence.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/PropertyTree/PropertyTreeValue.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Utility/PropertyTree/PropertyTreePath.cpp
)

cocktail_source_directory(CORE_FILES    
    FOLDER "Utility/PropertyTree/Xml"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/PropertyTree/Xml/XmlProperties.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/PropertyTree/Xml/XmlCDataNode.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Utility/PropertyTree/Xml/XmlProperties.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Utility/PropertyTree/Xml/XmlCDataNode.cpp
)

cocktail_source_directory(CORE_FILES    
    FOLDER "Utility/Time"
    INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/Time/Clock.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/Time/Duration.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/Time/Instant.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/Cocktail/Core/Utility/Time/TimeUnit.hpp
    SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Utility/Time/Clock.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Utility/Time/Duration.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Utility/Time/Instant.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Utility/Time/TimeUnit.cpp
)

if (WIN32)
    cocktail_source_directory(CORE_FILES    
        FOLDER "Utility/Time/Win32"
        SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Utility/Time/Win32/Instant.cpp
    )
elseif (LINUX)
    cocktail_source_directory(CORE_FILES    
        FOLDER "Utility/Time/Unix"
        SRC
            ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocktail/Core/Utility/Time/Unix/Instant.cpp
    )
endif ()

set (CORE_LINK_LIBRARIES stb_image stb_image_write tinyxml2::tinyxml2 cmrc::base)
if (WIN32)
    list (APPEND CORE_LINK_LIBRARIES winmm ws2_32)
elseif (LINUX)
    find_package(X11 REQUIRED COMPONENTS Xrandr Xcursor)
    list (APPEND CORE_LINK_LIBRARIES dl pthread X11::X11 X11::Xrandr X11::Xcursor)
endif ()

cocktail_add_runtime_library(
    NAME Core 
    DEPENDS magic_enum
    LINK ${CORE_LINK_LIBRARIES}
    SOURCES ${CORE_FILES}
)
