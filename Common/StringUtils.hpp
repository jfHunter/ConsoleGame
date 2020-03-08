#pragma once

#include <vector>
#include <string>
#include <cstdio>
#include <regex>

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include <Windows.h>

typedef std::vector<std::string> StringVect;

const std::string DEFAULT_WHITESPACE = " \n\r\t";

#define String(x) std::string(x)

/**
 * This class provides helper functions API for std::string manipulating
 */
class StringUtils
{
public:
	StringUtils(StringUtils const&) = delete;
	void operator=(StringUtils const&) = delete;
	StringUtils(void) = delete;

	static bool Equals(std::string strA, std::string strB)
	{
		return strA == strB;
	}

	static bool Equals(const char* strA, const char* strB)
	{
		return (strcmp(strA, strB) == 0);
	}

	static std::string TrimLeft(std::string& str)
	{
		size_t start = str.find_first_not_of(DEFAULT_WHITESPACE);
		return (start == std::string::npos) ? "" : str.substr(start);
	}

	static std::string TrimRight(std::string& str)
	{
		size_t end = str.find_last_not_of(DEFAULT_WHITESPACE);
		return (end == std::string::npos) ? "" : str.substr(0, end + 1);
	}

	static std::string Trim(std::string& str)
	{
		str = TrimLeft(str);
		return TrimRight(str);
	}

	LPCWSTR StrToLPCWSTR(std::string str)
	{
		std::wstring _wstr(str.begin(), str.end());
		return _wstr.c_str();
	}

	std::string LPCWSTRToStr(LPCWSTR lpcwstr)
	{
		char _chr[260];
		char _defchr = ' ';
		WideCharToMultiByte(CP_ACP, 0, lpcwstr, -1, _chr, 260, &_defchr, NULL);
		return std::string(_chr);
	}

	static bool RegexTest(std::string pattern, std::string input)
	{
		std::regex reg(pattern);
		return std::regex_match(input, reg);
	}

	static int IndexOf(std::string text, char character)
	{
		for (size_t i = 0; i < text.length(); i++)
			if (text[i] == character)
				return i;
		return -1;
	}

	static int IndexOf(std::string text, std::string subString)
	{
		if (subString.length() == 0)
			return -1;

		size_t textLength = text.length();
		size_t subStringLength = subString.length();

		for (size_t i = 0; i < textLength; i++)
		{
			if (i <= (textLength - subStringLength)) {
				if (text.substr(i, subStringLength) == subString)
					return i;
			}
		}
		return -1;
	}

	static bool Contains(std::string text, char character)
	{
		for (size_t i = 0; i < text.length(); i++)
			if (text[i] == character)
				return true;
		return false;
	}

	static bool Contains(std::string text, std::string subString)
	{
		if (subString.length() == 0)
			return false;

		size_t textLength = text.length();
		size_t subStringLength = subString.length();

		for (size_t i = 0; i < textLength; i++)
		{
			if (i <= (textLength - subStringLength)) {
				if (text.substr(i, subStringLength) == subString)
					return true;
			}
		}
		return false;
	}

	static std::string ToUpperCase(std::string text)
	{
		for (size_t i = 0; i < text.length(); i++)
			text[i] = toupper(text[i]);
		return text;
	}

	static std::string ToLowerCase(std::string text)
	{
		for (size_t i = 0; i < text.length(); i++)
			text[i] = tolower(text[i]);
		return text;
	}

	static StringVect Split(std::string text, char delimiter)
	{
		std::string currentToken = "";
		StringVect stringVect;

		for (size_t i = 0; i < text.length(); i++)
		{
			if (text[i] == delimiter)
			{
				if (currentToken != "")
					stringVect.push_back(currentToken);
				currentToken = "";
			} else {
				currentToken.push_back(text[i]);
			}
		}

		if (currentToken != "")
			stringVect.push_back(currentToken);

		return stringVect;
	}

	static StringVect Split(std::string text, std::string delimiter)
	{
		StringVect stringVect;
		std::string currentToken = "";

		size_t textLength = text.length();
		size_t subStringLength = delimiter.length();

		for (size_t i = 0; i < textLength; i++)	{
			if (i <= (textLength - subStringLength)) {
				if (text.substr(i, subStringLength) == delimiter) {
					if (currentToken != "")
						stringVect.push_back(currentToken);
					currentToken = "";
					i += subStringLength - 1;
				} else {
					currentToken.push_back(text[i]);
				}
			}
		}

		if (currentToken != "")
			stringVect.push_back(currentToken);

		return stringVect;
	}

	static std::string Join(StringVect array, char joiner)
	{
		std::string joined = "";
		for (size_t i = 0; i < array.size(); i++) {
			joined += array.at(i);
			if ((i + 1) != array.size())
				joined.push_back(joiner);
		}
		return joined;
	}

	static std::string Join(StringVect array, std::string joiner)
	{
		std::string joined = "";
		for (size_t i = 0; i < array.size(); i++) {
			joined += array.at(i);
			if ((i + 1) != array.size())
				joined += joiner;
		}
		return joined;
	}

	static std::string Replace(std::string text, std::string findStr, std::string replaceStr)
	{
		if (IndexOf(text, findStr) == -1)
			return text;

		StringVect splittedText = Split(text, findStr);
		return Join(splittedText, replaceStr);
	}

	static std::string GetFileName(std::string filePath)
	{
		size_t lastPathSeparatorIndex = filePath.find_last_of("/\\");
		if (lastPathSeparatorIndex != std::string::npos) {
			return filePath.substr(lastPathSeparatorIndex + 1);
		}
		return filePath;
	}

	static std::string GetFileLocation(std::string filePath)
	{
		size_t lastPathSeparatorIndex = filePath.find_last_of("/\\");
		if (lastPathSeparatorIndex != std::string::npos) {
			return filePath.substr(0, lastPathSeparatorIndex);
		}
		return filePath;
	}

	static std::string GetFileExtension(std::string fileName)
	{
		size_t lastDotIndex = fileName.find_last_of(".");
		if (lastDotIndex != std::string::npos) {
			return fileName.substr(lastDotIndex + 1);
		}
		return fileName;
	}

	static std::string GetFileRealName(std::string fileName)
	{
		size_t lastDotIndex = fileName.find_last_of(".");
		if (lastDotIndex != std::string::npos) {
			return fileName.substr(0, lastDotIndex);
		}
		return fileName;
	}

	static bool IsDigit(std::string input)
	{
		int length;
		if ((length = input.length()) == 0)
			return false;

		for (int i = 0; i < length; i++) {
			if (!isdigit(input[i]))
				return false;
		}

		return true;
	}

	static bool IsHex(std::string input)
	{
		return RegexTest("(0x)?([[:xdigit:]])+", input);
	}

	static bool ParseHexStrToUInt32(std::string input, uint32_t * outNumber)
	{
		if (!IsHex(input))
			return false;

		input = ToLowerCase(input);
		sscanf_s(input.c_str(), (IndexOf(input, 'x') >= 0 ?
			"0x%08X" : "%08X"), outNumber);

		return true;
	}
};
