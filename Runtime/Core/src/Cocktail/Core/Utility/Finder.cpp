#include <Cocktail/Core/System/FileSystem/Storage.hpp>
#include <Cocktail/Core/Utility/Finder.hpp>

namespace Ck
{
	Finder& Finder::From(const std::filesystem::path& path)
	{
		mSource.Add(path);

		return *this;
	}

	Finder& Finder::WithExtension(const std::string& fileExtension)
	{
		mFileExtension.clear();
		if (fileExtension[0] != '.')
			mFileExtension.append(".");

		mFileExtension.append(fileExtension);

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

	Array<std::filesystem::path> Finder::Get() const
	{
		Array<std::filesystem::path> content;
		for (const std::filesystem::path& source : mSource)
			content.Append(Get(source, mDepth));

		return content;
	}

	Array<std::filesystem::path> Finder::Get(const std::filesystem::path& source, unsigned int depth) const
	{
		Array<std::filesystem::path> content;
		std::shared_ptr<Directory> directory = Storage::OpenDirectory(source);

		for (const std::filesystem::path& child : directory->GetContent())
		{
			if (child == "." || child == "..")
				continue;

			if (Storage::IsFile(child) && !mIgnoreFiles)
			{
				if (!mFileExtension.empty() && child.extension() != mFileExtension)
					continue;

				content.Add(child);
			}
			else if (Storage::IsDirectory(child))
			{
				if (!mIgnoreDirectories)
					content.Add(child);

				if (depth > 0)
					content.Append(Get(child, depth - 1));
			}
		}

		return content;
	}
}
