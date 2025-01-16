#include <Cocktail/Core/System/FileSystem/Storage.hpp>
#include <Cocktail/Core/Utility/Finder.hpp>

namespace Ck
{
	Finder& Finder::From(const std::filesystem::path& path)
	{
		mSource.emplace_back(path);

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

	std::vector<std::filesystem::path> Finder::Get() const
	{
		std::vector<std::filesystem::path> content;
		for (const std::filesystem::path& source : mSource)
		{
			std::vector<std::filesystem::path> sourceContent = Get(source, mDepth);
			if (sourceContent.empty())
				continue;

			content.insert(content.end(), sourceContent.begin(), sourceContent.end());
		}

		return content;
	}

	std::vector<std::filesystem::path> Finder::Get(const std::filesystem::path& source, unsigned int depth) const
	{
		std::vector<std::filesystem::path> content;
		Ref<Directory> directory = Storage::OpenDirectory(source);

		for (const std::filesystem::path& child : directory->GetContent())
		{
			if (child == "." || child == "..")
				continue;

			if (Storage::IsFile(child) && !mIgnoreFiles)
			{
				if (!mFileExtension.empty() && child.extension() != mFileExtension)
					continue;

				content.push_back(child);
			}
			else if (Storage::IsDirectory(child))
			{
				if (!mIgnoreDirectories)
					content.push_back(child);

				if (depth > 0)
				{
					std::vector<std::filesystem::path> subContent = Get(child, depth - 1);
					content.insert(content.end(), subContent.begin(), subContent.end());
				}
			}
		}

		return content;
	}
}
