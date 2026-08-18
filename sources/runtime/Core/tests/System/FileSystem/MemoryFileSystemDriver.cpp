#include <catch2/catch_all.hpp>

#include <CocktailEngine/Core/IO/Input/Stream/FileInputStream.hpp>
#include <CocktailEngine/Core/IO/Output/Stream/FileOutputStream.hpp>
#include <CocktailEngine/Core/System/FileSystem/Memory/MemoryFileSystemDriver.hpp>

namespace
{
	constexpr unsigned int CharacterSize = sizeof(Ck::TextChar);

	Ck::String ReadAll(Ck::FileSystemDriver& driver, const Ck::Path& path)
	{
		Ck::UniquePtr<Ck::File> file = driver.OpenFile(path, Ck::FileOpenFlagBits::Read);

		Ck::String content;
		Ck::TextChar character;
		while (file->Read(&character, CharacterSize) == CharacterSize)
			content.Append(character);

		return content;
	}

	void WriteAll(Ck::FileSystemDriver& driver, const Ck::Path& path, const Ck::String& content)
	{
		const Ck::FileOpenFlags flags = Ck::FileOpenFlags::Of(Ck::FileOpenFlagBits::Write, Ck::FileOpenFlagBits::Truncate);

		Ck::UniquePtr<Ck::File> file = driver.OpenFile(path, flags);
		file->Write(content.GetData(), static_cast<unsigned int>(content.GetLength()) * CharacterSize);
	}
}

TEST_CASE("Create entries in a memory filesystem", "[MemoryFileSystemDriver]")
{
	Ck::MemoryFileSystemDriver driver;

	SECTION("The root always exists and is a directory")
	{
		REQUIRE(driver.IsDirectory(Ck::Path::Empty));
		REQUIRE(driver.IsDirectory(CK_TEXT("/")));
		REQUIRE_FALSE(driver.IsFile(Ck::Path::Empty));
	}

	SECTION("A file can be created at the root")
	{
		driver.CreateFile(CK_TEXT("hello.txt"));

		REQUIRE(driver.IsFile(CK_TEXT("hello.txt")));
		REQUIRE_FALSE(driver.IsDirectory(CK_TEXT("hello.txt")));
		REQUIRE(driver.GetPathInfo(CK_TEXT("hello.txt")).Type == Ck::PathType::File);
		REQUIRE(driver.GetPathInfo(CK_TEXT("hello.txt")).Size == 0);
	}

	SECTION("Creating an existing file is not an error")
	{
		driver.CreateFile(CK_TEXT("hello.txt"));

		REQUIRE_NOTHROW(driver.CreateFile(CK_TEXT("hello.txt")));
	}

	SECTION("Creating an existing directory is an error")
	{
		driver.CreateDirectory(CK_TEXT("assets"));

		REQUIRE_THROWS_AS(driver.CreateDirectory(CK_TEXT("assets")), std::system_error);
	}

	SECTION("Parent directories are not created implicitly")
	{
		REQUIRE_THROWS_AS(driver.CreateFile(CK_TEXT("assets/textures/wood.png")), std::system_error);

		driver.CreateDirectory(CK_TEXT("assets"));
		driver.CreateDirectory(CK_TEXT("assets/textures"));

		REQUIRE_NOTHROW(driver.CreateFile(CK_TEXT("assets/textures/wood.png")));
		REQUIRE(driver.IsFile(CK_TEXT("assets/textures/wood.png")));
	}

	SECTION("An unknown path has no type")
	{
		REQUIRE(driver.GetPathInfo(CK_TEXT("nope.txt")).Type == Ck::PathType::None);
	}
}

TEST_CASE("Normalize paths of a memory filesystem", "[MemoryFileSystemDriver]")
{
	Ck::MemoryFileSystemDriver driver;
	driver.CreateDirectory(CK_TEXT("assets"));
	driver.CreateFile(CK_TEXT("assets/wood.png"));

	SECTION("The root component is ignored")
	{
		REQUIRE(driver.IsFile(CK_TEXT("/assets/wood.png")));
	}

	SECTION("Separators are interchangeable")
	{
		REQUIRE(driver.IsFile(CK_TEXT(R"(assets\wood.png)")));
		REQUIRE(driver.IsFile(CK_TEXT(R"(\assets\wood.png)")));
	}

	SECTION("Trailing separators are ignored")
	{
		REQUIRE(driver.IsDirectory(CK_TEXT("assets/")));
	}

	SECTION("A canonical path is rooted in the virtual filesystem")
	{
		REQUIRE(driver.MakeCanonical(CK_TEXT(R"(assets\wood.png)")).ToString() == CK_TEXT("/assets/wood.png"));
		REQUIRE(driver.TryMakeCanonical(CK_TEXT("nope.png")).IsEmpty());
	}
}

TEST_CASE("Read and write a file of a memory filesystem", "[MemoryFileSystemDriver]")
{
	Ck::MemoryFileSystemDriver driver;

	SECTION("Opening a missing file for reading fails")
	{
		REQUIRE_THROWS_AS(driver.OpenFile(CK_TEXT("nope.txt"), Ck::FileOpenFlagBits::Read), std::system_error);
	}

	SECTION("Opening a missing file for writing creates it")
	{
		Ck::UniquePtr<Ck::File> file = driver.OpenFile(CK_TEXT("hello.txt"), Ck::FileOpenFlagBits::Write);

		REQUIRE(driver.IsFile(CK_TEXT("hello.txt")));
		REQUIRE(file->GetPath().ToString() == CK_TEXT("hello.txt"));
		REQUIRE(file->GetSystemHandle() == nullptr);
	}

	SECTION("Opening a missing existing-only file for writing fails")
	{
		const Ck::FileOpenFlags flags = Ck::FileOpenFlags::Of(Ck::FileOpenFlagBits::Write, Ck::FileOpenFlagBits::Existing);

		REQUIRE_THROWS_AS(driver.OpenFile(CK_TEXT("nope.txt"), flags), std::system_error);
	}

	SECTION("Opening a directory as a file fails")
	{
		driver.CreateDirectory(CK_TEXT("assets"));

		REQUIRE_THROWS_AS(driver.OpenFile(CK_TEXT("assets"), Ck::FileOpenFlagBits::Read), std::system_error);
	}

	SECTION("What is written can be read back")
	{
		WriteAll(driver, CK_TEXT("hello.txt"), CK_TEXT("Hello world"));

		REQUIRE(driver.GetPathInfo(CK_TEXT("hello.txt")).Size == 11 * CharacterSize);
		REQUIRE(ReadAll(driver, CK_TEXT("hello.txt")) == CK_TEXT("Hello world"));
	}

	SECTION("Writing respects the access flags")
	{
		driver.CreateFile(CK_TEXT("hello.txt"));

		Ck::UniquePtr<Ck::File> file = driver.OpenFile(CK_TEXT("hello.txt"), Ck::FileOpenFlagBits::Read);

		REQUIRE_THROWS_AS(file->Write(CK_TEXT("nope"), 4 * CharacterSize), std::system_error);
	}

	SECTION("Truncating drops the previous content")
	{
		WriteAll(driver, CK_TEXT("hello.txt"), CK_TEXT("Hello world"));
		WriteAll(driver, CK_TEXT("hello.txt"), CK_TEXT("Bye"));

		REQUIRE(ReadAll(driver, CK_TEXT("hello.txt")) == CK_TEXT("Bye"));
	}

	SECTION("Appending writes at the end whatever the cursor is")
	{
		WriteAll(driver, CK_TEXT("hello.txt"), CK_TEXT("Hello"));

		const Ck::FileOpenFlags flags = Ck::FileOpenFlags::Of(Ck::FileOpenFlagBits::Write, Ck::FileOpenFlagBits::Append);
		Ck::UniquePtr<Ck::File> file = driver.OpenFile(CK_TEXT("hello.txt"), flags);

		file->SetCursor(Ck::FileCursorMode::Begin, 0);
		file->Write(CK_TEXT(" world"), 6 * CharacterSize);

		REQUIRE(ReadAll(driver, CK_TEXT("hello.txt")) == CK_TEXT("Hello world"));
	}

	SECTION("The cursor can be moved around")
	{
		WriteAll(driver, CK_TEXT("hello.txt"), CK_TEXT("Hello world"));

		Ck::UniquePtr<Ck::File> file = driver.OpenFile(CK_TEXT("hello.txt"), Ck::FileOpenFlagBits::Read);

		REQUIRE(file->GetSize() == 11 * CharacterSize);
		REQUIRE(file->SetCursor(Ck::FileCursorMode::End, 0) == 11 * CharacterSize);
		REQUIRE(file->SetCursor(Ck::FileCursorMode::Begin, 2) == 2);
		REQUIRE(file->SetCursor(Ck::FileCursorMode::Current, 2) == 4);
		REQUIRE_THROWS_AS(file->SetCursor(Ck::FileCursorMode::Begin, -1), std::system_error);

		// Reading past the end yields nothing rather than failing
		file->SetCursor(Ck::FileCursorMode::End, 0);

		Ck::TextChar character;
		REQUIRE(file->Read(&character, CharacterSize) == 0);
	}

	SECTION("Writing past the end leaves a hole filled with zeros")
	{
		Ck::UniquePtr<Ck::File> file = driver.OpenFile(CK_TEXT("hole.bin"), Ck::FileOpenFlagBits::Write);
		file->SetCursor(Ck::FileCursorMode::Begin, 4);

		const Ck::Byte value = 42;
		file->Write(&value, sizeof(Ck::Byte));

		REQUIRE(file->GetSize() == 5);

		Ck::Byte content[5] = { 1, 1, 1, 1, 1 };
		Ck::UniquePtr<Ck::File> reader = driver.OpenFile(CK_TEXT("hole.bin"), Ck::FileOpenFlagBits::Read);

		REQUIRE(reader->Read(content, 5) == 5);
		REQUIRE(content[0] == 0);
		REQUIRE(content[3] == 0);
		REQUIRE(content[4] == 42);
	}

	SECTION("Two handles share the content but not the cursor")
	{
		WriteAll(driver, CK_TEXT("hello.txt"), CK_TEXT("Hello"));

		Ck::UniquePtr<Ck::File> first = driver.OpenFile(CK_TEXT("hello.txt"), Ck::FileOpenFlagBits::Read);
		Ck::UniquePtr<Ck::File> second = driver.OpenFile(CK_TEXT("hello.txt"), Ck::FileOpenFlagBits::Read);

		first->SetCursor(Ck::FileCursorMode::End, 0);

		REQUIRE(first->GetCursor() != second->GetCursor());
		REQUIRE(first->GetSize() == second->GetSize());
	}
}

TEST_CASE("Enumerate a directory of a memory filesystem", "[MemoryFileSystemDriver]")
{
	Ck::MemoryFileSystemDriver driver;
	driver.CreateDirectory(CK_TEXT("assets"));
	driver.CreateFile(CK_TEXT("assets/wood.png"));
	driver.CreateFile(CK_TEXT("assets/stone.png"));
	driver.CreateDirectory(CK_TEXT("assets/models"));

	SECTION("An iterator walks every entry once")
	{
		Ck::UniquePtr<Ck::DirectoryIterator> iterator = driver.CreateDirectoryIterator(CK_TEXT("assets"));
		REQUIRE(iterator != nullptr);

		unsigned int files = 0;
		unsigned int directories = 0;
		for (; !iterator->IsEnd(); iterator->Next())
		{
			// The iterator yields paths relative to the root of the virtual filesystem
			REQUIRE(iterator->GetPath().GetParent().ToString() == CK_TEXT("assets"));

			switch (iterator->GetPathInfo().Type)
			{
			case Ck::PathType::File:
				files++;
				break;

			case Ck::PathType::Directory:
				directories++;
				break;

			default:
				break;
			}
		}

		REQUIRE(files == 2);
		REQUIRE(directories == 1);
	}

	SECTION("An iterator on something else than a directory is empty")
	{
		REQUIRE(driver.CreateDirectoryIterator(CK_TEXT("assets/wood.png")) == nullptr);
		REQUIRE(driver.CreateDirectoryIterator(CK_TEXT("nope")) == nullptr);
	}

	SECTION("A directory handle lists the name of its entries")
	{
		Ck::UniquePtr<Ck::Directory> directory = driver.OpenDirectory(CK_TEXT("assets"));

		REQUIRE(directory->GetPath().ToString() == CK_TEXT("assets"));
		REQUIRE(directory->GetContent().GetSize() == 3);
		REQUIRE(directory->GetContent().Contains(Ck::Path(CK_TEXT("wood.png"))));
	}

	SECTION("Opening something else than a directory fails")
	{
		REQUIRE_THROWS_AS(driver.OpenDirectory(CK_TEXT("assets/wood.png")), std::system_error);
		REQUIRE_THROWS_AS(driver.OpenDirectory(CK_TEXT("nope")), std::system_error);
	}
}

TEST_CASE("Copy entries of a memory filesystem", "[MemoryFileSystemDriver]")
{
	Ck::MemoryFileSystemDriver driver;
	driver.CreateDirectory(CK_TEXT("assets"));
	WriteAll(driver, CK_TEXT("assets/wood.png"), CK_TEXT("wood"));
	driver.CreateDirectory(CK_TEXT("assets/models"));
	WriteAll(driver, CK_TEXT("assets/models/cube.gltf"), CK_TEXT("cube"));

	SECTION("A file is copied with its content")
	{
		driver.CopyFile(CK_TEXT("assets/wood.png"), CK_TEXT("assets/stone.png"), {});

		REQUIRE(driver.IsFile(CK_TEXT("assets/wood.png")));
		REQUIRE(ReadAll(driver, CK_TEXT("assets/stone.png")) == CK_TEXT("wood"));
	}

	SECTION("Copying over an existing file requires Overwrite")
	{
		driver.CreateFile(CK_TEXT("assets/stone.png"));

		REQUIRE_THROWS_AS(driver.CopyFile(CK_TEXT("assets/wood.png"), CK_TEXT("assets/stone.png"), {}), std::system_error);

		Ck::FileCopyOptions options;
		options.Overwrite = true;

		REQUIRE_NOTHROW(driver.CopyFile(CK_TEXT("assets/wood.png"), CK_TEXT("assets/stone.png"), options));
		REQUIRE(ReadAll(driver, CK_TEXT("assets/stone.png")) == CK_TEXT("wood"));
	}

	SECTION("A shallow copy leaves the subdirectories out")
	{
		driver.CopyDirectory(CK_TEXT("assets"), CK_TEXT("backup"), {});

		REQUIRE(driver.IsFile(CK_TEXT("backup/wood.png")));
		REQUIRE_FALSE(driver.IsDirectory(CK_TEXT("backup/models")));
	}

	SECTION("A recursive copy duplicates the whole subtree")
	{
		Ck::DirectoryCopyOptions options;
		options.Recursive = true;

		driver.CopyDirectory(CK_TEXT("assets"), CK_TEXT("backup"), options);

		REQUIRE(driver.IsDirectory(CK_TEXT("backup/models")));
		REQUIRE(ReadAll(driver, CK_TEXT("backup/wood.png")) == CK_TEXT("wood"));
		REQUIRE(ReadAll(driver, CK_TEXT("backup/models/cube.gltf")) == CK_TEXT("cube"));

		// The source is left untouched
		REQUIRE(ReadAll(driver, CK_TEXT("assets/models/cube.gltf")) == CK_TEXT("cube"));
	}

	SECTION("Copying only the structure leaves the files out")
	{
		Ck::DirectoryCopyOptions options;
		options.Recursive = true;
		options.OnlyStructure = true;

		driver.CopyDirectory(CK_TEXT("assets"), CK_TEXT("backup"), options);

		REQUIRE(driver.IsDirectory(CK_TEXT("backup/models")));
		REQUIRE_FALSE(driver.IsFile(CK_TEXT("backup/wood.png")));
	}

	SECTION("Copying a directory into itself is refused")
	{
		Ck::DirectoryCopyOptions options;
		options.Recursive = true;

		REQUIRE_THROWS_AS(driver.CopyDirectory(CK_TEXT("assets"), CK_TEXT("assets/backup"), options), std::system_error);
	}
}

TEST_CASE("Move entries of a memory filesystem", "[MemoryFileSystemDriver]")
{
	Ck::MemoryFileSystemDriver driver;
	driver.CreateDirectory(CK_TEXT("assets"));
	WriteAll(driver, CK_TEXT("assets/wood.png"), CK_TEXT("wood"));
	driver.CreateDirectory(CK_TEXT("assets/models"));
	WriteAll(driver, CK_TEXT("assets/models/cube.gltf"), CK_TEXT("cube"));

	SECTION("A file changes place and keeps its content")
	{
		driver.MoveFile(CK_TEXT("assets/wood.png"), CK_TEXT("wood.png"), {});

		REQUIRE_FALSE(driver.IsFile(CK_TEXT("assets/wood.png")));
		REQUIRE(ReadAll(driver, CK_TEXT("wood.png")) == CK_TEXT("wood"));
		REQUIRE(driver.OpenDirectory(CK_TEXT("assets"))->GetContent().GetSize() == 1);
	}

	SECTION("Moving over an existing file requires Overwrite")
	{
		WriteAll(driver, CK_TEXT("stone.png"), CK_TEXT("stone"));

		REQUIRE_THROWS_AS(driver.MoveFile(CK_TEXT("assets/wood.png"), CK_TEXT("stone.png"), {}), std::system_error);

		Ck::FileMoveOptions options;
		options.Overwrite = true;
		driver.MoveFile(CK_TEXT("assets/wood.png"), CK_TEXT("stone.png"), options);

		REQUIRE(ReadAll(driver, CK_TEXT("stone.png")) == CK_TEXT("wood"));
		REQUIRE(driver.OpenDirectory(Ck::Path::Empty)->GetContent().GetSize() == 2);
	}

	SECTION("Moving to a missing directory fails and changes nothing")
	{
		REQUIRE_THROWS_AS(driver.MoveFile(CK_TEXT("assets/wood.png"), CK_TEXT("nope/wood.png"), {}), std::system_error);
		REQUIRE(driver.IsFile(CK_TEXT("assets/wood.png")));
	}

	SECTION("A directory drags its whole subtree along")
	{
		driver.MoveDirectory(CK_TEXT("assets"), CK_TEXT("backup"), {});

		REQUIRE_FALSE(driver.IsDirectory(CK_TEXT("assets")));
		REQUIRE(driver.IsDirectory(CK_TEXT("backup/models")));
		REQUIRE(ReadAll(driver, CK_TEXT("backup/wood.png")) == CK_TEXT("wood"));
		REQUIRE(ReadAll(driver, CK_TEXT("backup/models/cube.gltf")) == CK_TEXT("cube"));
		REQUIRE(driver.MakeCanonical(CK_TEXT("backup/models/cube.gltf")).ToString() == CK_TEXT("/backup/models/cube.gltf"));
	}

	SECTION("Moving a directory into itself is refused")
	{
		REQUIRE_THROWS_AS(driver.MoveDirectory(CK_TEXT("assets"), CK_TEXT("assets/backup"), {}), std::system_error);
		REQUIRE(driver.IsDirectory(CK_TEXT("assets")));
	}

	SECTION("The root cannot be moved")
	{
		REQUIRE_THROWS_AS(driver.MoveDirectory(Ck::Path::Empty, CK_TEXT("backup"), {}), std::system_error);
	}
}

TEST_CASE("Remove entries of a memory filesystem", "[MemoryFileSystemDriver]")
{
	Ck::MemoryFileSystemDriver driver;
	driver.CreateDirectory(CK_TEXT("assets"));
	WriteAll(driver, CK_TEXT("assets/wood.png"), CK_TEXT("wood"));

	SECTION("Removing a file unlinks it")
	{
		driver.RemoveFile(CK_TEXT("assets/wood.png"), {});

		REQUIRE_FALSE(driver.IsFile(CK_TEXT("assets/wood.png")));
		REQUIRE(driver.OpenDirectory(CK_TEXT("assets"))->GetContent().IsEmpty());
	}

	SECTION("Removing a missing file depends on IgnoreMissing")
	{
		REQUIRE_NOTHROW(driver.RemoveFile(CK_TEXT("nope.png"), {}));

		Ck::FileRemoveOptions options;
		options.IgnoreMissing = false;

		REQUIRE_THROWS_AS(driver.RemoveFile(CK_TEXT("nope.png"), options), std::system_error);
	}

	SECTION("An open handle survives the removal of its file")
	{
		Ck::UniquePtr<Ck::File> file = driver.OpenFile(CK_TEXT("assets/wood.png"), Ck::FileOpenFlagBits::Read);
		driver.RemoveFile(CK_TEXT("assets/wood.png"), {});

		REQUIRE_FALSE(driver.IsFile(CK_TEXT("assets/wood.png")));
		REQUIRE(file->GetSize() == 4 * CharacterSize);
	}

	SECTION("Removing a non empty directory requires Recursive")
	{
		REQUIRE_THROWS_AS(driver.RemoveDirectory(CK_TEXT("assets"), {}), std::system_error);

		Ck::DirectoryRemoveOptions options;
		options.Recursive = true;
		driver.RemoveDirectory(CK_TEXT("assets"), options);

		REQUIRE_FALSE(driver.IsDirectory(CK_TEXT("assets")));
		REQUIRE_FALSE(driver.IsFile(CK_TEXT("assets/wood.png")));
		REQUIRE(driver.OpenDirectory(Ck::Path::Empty)->GetContent().IsEmpty());
	}

	SECTION("The root cannot be removed")
	{
		Ck::DirectoryRemoveOptions options;
		options.Recursive = true;

		REQUIRE_THROWS_AS(driver.RemoveDirectory(Ck::Path::Empty, options), std::system_error);
	}

	SECTION("Clearing drops everything but the root")
	{
		driver.Clear();

		REQUIRE(driver.IsDirectory(Ck::Path::Empty));
		REQUIRE_FALSE(driver.IsDirectory(CK_TEXT("assets")));
		REQUIRE(driver.GetUsedSize() == 0);
	}
}

TEST_CASE("Stream to and from a memory filesystem", "[MemoryFileSystemDriver]")
{
	Ck::MemoryFileSystemDriver driver;

	constexpr Ck::Uint32 PayloadSize = 5;
	const Ck::Byte payload[PayloadSize] = { 1, 2, 3, 4, 5 };

	// The generic stream adapters must work on the memory driver like on any other
	{
		Ck::FileOutputStream outputStream(CK_TEXT("hello.bin"), true, &driver);
		REQUIRE(outputStream.Write(payload, PayloadSize) == PayloadSize);
	}

	REQUIRE(driver.IsFile(CK_TEXT("hello.bin")));
	REQUIRE(driver.GetUsedSize() == PayloadSize);

	Ck::FileInputStream inputStream(CK_TEXT("hello.bin"), &driver);
	REQUIRE(inputStream.GetSize() == PayloadSize);

	Ck::Byte readBack[PayloadSize] = {};
	REQUIRE(inputStream.Read(readBack, PayloadSize) == PayloadSize);
	REQUIRE(readBack[4] == 5);
}
