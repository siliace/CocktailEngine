#include <Cocktail/Core/IO/Input/Reader/BufferedReader.hpp>
#include <Cocktail/Core/IO/Input/Reader/InputStreamReader.hpp>
#include <Cocktail/Core/IO/Input/Stream/FileInputStream.hpp>
#include <Cocktail/Core/IO/Output/Stream/FileOutputStream.hpp>
#include <Cocktail/Core/System/FileSystem/Storage.hpp>
#include <Cocktail/Core/Utility/FileUtils.hpp>

namespace Ck
{
	void FileUtils::MakeDirectories(const std::filesystem::path& path)
	{
		if (!Storage::IsDirectory(path))
		{
			MakeDirectories(path.parent_path());

			Storage::CreateDirectory(path);
		}
	}

	ByteArray FileUtils::ReadFile(const std::filesystem::path& path)
	{
		ByteArray content;
		if (!Storage::IsFile(path))
			return content;

		std::unique_ptr<File> file = Storage::OpenFile(path, FileOpenFlagBits::Read | FileOpenFlagBits::Existing);

		const std::size_t size = file->GetSize();
		if (!size)
			return content;

		content.Resize(size);
		file->Read(content.GetData(), content.GetSize());

		return content;
	}

	Array<std::string> FileUtils::ReadFileLines(const std::filesystem::path& path)
	{
		if (!Storage::IsFile(path))
			return {};

		std::unique_ptr<File> file = Storage::OpenFile(path, FileOpenFlagBits::Read | FileOpenFlagBits::Existing);
		FileInputStream inputStream(*file);

		InputStreamReader inputStreamReader(inputStream);
		BufferedReader reader(inputStreamReader);

		Array<std::string> lines;
		while (!reader.IsEof())
			lines.Add(reader.ReadLine());

		return lines;
	}

	void FileUtils::WriteFile(const std::filesystem::path& path, ByteArrayView content, bool append)
	{
		if (!Storage::IsFile(path))
		{
			MakeDirectories(path.parent_path());
			Storage::CreateFile(path);
		}

		FileOpenFlagBits openFlags = FileOpenFlagBits::Write;
		openFlags |= append ? FileOpenFlagBits::Append : FileOpenFlagBits::Truncate;

		std::unique_ptr<File> file = Storage::OpenFile(path, openFlags);
		file->Write(content.GetData(), content.GetSize());
	}

	void FileUtils::WriteFileLines(const std::filesystem::path& path, const Array<std::string>& lines, bool append)
	{
		if (!Storage::IsFile(path))
		{
			MakeDirectories(path.parent_path());
			Storage::CreateFile(path);
		}

		FileOpenFlagBits openFlags = FileOpenFlagBits::Write;
		openFlags |= append ? FileOpenFlagBits::Append : FileOpenFlagBits::Truncate;

		std::unique_ptr<File> file = Storage::OpenFile(path, openFlags);
		FileOutputStream outputStream(*file);

		lines.ForEach([&](const std::string& line) {
			outputStream.Write(line.data(), line.length());
		});

		outputStream.Flush();
	}
}
