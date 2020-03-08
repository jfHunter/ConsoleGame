#include <Windows.h>
#include <string.h>
#include <fstream>

#pragma once
class FileUtils
{
public:

	static std::string GetAbsoluteFilePath(const std::string cstrRelativeFilePath)
	{
		char rcBuffer[4096];
		int retVal = GetFullPathName(cstrRelativeFilePath.c_str(), 4096, rcBuffer, 0);
		std::string strAbsolueStr(rcBuffer);
		return strAbsolueStr;
	}

	static bool IsFileRegular(const std::string cstrFilePath)
	{
		DWORD dwFileAttributes = GetFileAttributes(cstrFilePath.c_str());
		return (dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
	}

	static bool FileExists(const std::string cstrFilePath)
	{
		return (((DWORD)-1) != GetFileAttributes(cstrFilePath.c_str()));
	}

	static unsigned int GetFileSize(std::string filePath)
	{
		if (!FileUtils::FileExists(filePath) || !FileUtils::IsFileRegular(filePath))
			return -1;

		std::ifstream file(filePath.c_str(), std::ios::ate | std::ios::binary);
		int fileSize = (unsigned int)file.tellg();
		file.close();
		return fileSize;
	}
};

