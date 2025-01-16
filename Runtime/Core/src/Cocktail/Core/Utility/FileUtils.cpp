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
		Ref<File> file = Storage::OpenFile(path, FileOpenFlagBits::Read | FileOpenFlagBits::Existing);

		const std::size_t size = file->GetSize();
		std::unique_ptr<std::uint8_t[]> buffer = std::make_unique<std::uint8_t[]>(size);

		const std::size_t read = file->Read(buffer.get(), size);

		return ByteArray(reinterpret_cast<char*>(buffer.get()), read);
	}

	std::vector<std::string> FileUtils::ReadFileLines(const std::filesystem::path& path)
	{
		Ref<File> file = Storage::OpenFile(path, FileOpenFlagBits::Read | FileOpenFlagBits::Existing);

		FileInputStream inputStream(file);
		InputStreamReader inputStreamReader(inputStream);
		BufferedReader reader(inputStreamReader);

		std::vector<std::string> lines;
		while (!reader.IsEof())
			lines.push_back(reader.ReadLine());

		return lines;
	}

	void FileUtils::WriteFile(const std::filesystem::path& path, ByteArrayView content)
	{
		if (!Storage::IsFile(path))
			Storage::CreateFile(path);

		Ref<File> file = Storage::OpenFile(path, FileOpenFlagBits::Write | FileOpenFlagBits::Truncate);
		file->Write(content.GetData(), content.GetSize());
	}

	void FileUtils::WriteFileLines(const std::filesystem::path& path, const std::vector<std::string>& lines)
	{
	}

	void FileUtils::AppendFileLines(const std::filesystem::path& path, const std::vector<std::string>& lines)
	{
	}
}
