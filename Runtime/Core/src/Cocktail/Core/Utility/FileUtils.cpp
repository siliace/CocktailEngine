#include <Cocktail/Core/IO/Input/Reader/BufferedReader.hpp>
#include <Cocktail/Core/IO/Input/Reader/InputStreamReader.hpp>
#include <Cocktail/Core/IO/Input/Reader/LineReader.hpp>
#include <Cocktail/Core/IO/Input/Stream/FileInputStream.hpp>
#include <Cocktail/Core/IO/Output/Stream/FileOutputStream.hpp>
#include <Cocktail/Core/IO/Output/Writer/BufferedWriter.hpp>
#include <Cocktail/Core/IO/Output/Writer/FileWriter.hpp>
#include <Cocktail/Core/System/FileSystem/Storage.hpp>
#include <Cocktail/Core/Utility/FileUtils.hpp>
#include <Cocktail/Core/Utility/Encoding/Encoders.hpp>

namespace Ck
{
	void FileUtils::MakeDirectories(const URI& uri)
	{
		if (!Storage::IsDirectory(uri))
		{
			MakeDirectories(URI(uri.GetScheme(), uri.GetPath().GetParent()));
			Storage::CreateDirectory(uri);
		}
	}

	ByteArray FileUtils::ReadFile(const URI& uri)
	{
		ByteArray content;
		if (!Storage::IsFile(uri))
			return content;

		std::unique_ptr<File> file = Storage::OpenFile(uri, FileOpenFlagBits::Read | FileOpenFlagBits::Existing);

		const std::size_t size = file->GetSize();
		if (!size)
			return content;

		content.Resize(size);
		file->Read(content.GetData(), content.GetSize());

		return content;
	}

	String FileUtils::ReadFileText(const URI& uri)
	{
		auto content = ReadFile(uri);
		if (content.IsEmpty())
			return String::Empty;

		return Encoders::GetString<Encoders::Text, String>(content);
	}

	void FileUtils::WriteFile(const URI& uri, ByteArrayView content, bool append)
	{
		if (!Storage::IsFile(uri))
		{
			MakeDirectories(URI(uri.GetScheme(), uri.GetPath().GetParent()));
			Storage::CreateFile(uri);
		}

		FileOpenFlagBits openFlags = FileOpenFlagBits::Write;
		openFlags |= append ? FileOpenFlagBits::Append : FileOpenFlagBits::Truncate;

		std::unique_ptr<File> file = Storage::OpenFile(uri, openFlags);
		file->Write(content.GetData(), content.GetSize());
	}

	void FileUtils::WriteFileLines(const URI& uri, const Array<String>& lines, bool append)
	{
		if (!Storage::IsFile(uri))
		{
			MakeDirectories(URI(uri.GetScheme(), uri.GetPath().GetParent()));
			Storage::CreateFile(uri);
		}

		FileOpenFlagBits openFlags = FileOpenFlagBits::Write;
		openFlags |= append ? FileOpenFlagBits::Append : FileOpenFlagBits::Truncate;

		std::unique_ptr<File> file = Storage::OpenFile(uri, openFlags);
		FileWriter writer(*file);
		BufferedWriter bufferedWriter(writer);

		lines.ForEach([&](const String& line) {
			bufferedWriter.Write(line.GetData(), line.GetLength());
		});
	}
}
