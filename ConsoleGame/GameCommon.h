#pragma once

#include <iostream>
#include <string>

#include <allegro5/allegro.h>
#include <allegro5/file.h>

#include "StringUtils.hpp"

using std::cout;
using std::endl;
using std::string;

class Coord
{
private:
	int m_x;
	int m_y;

public:
	Coord(int _x, int _y)
	{
		this->m_x = _x;
		this->m_y = _y;
	}

	int GetX()
	{
		return this->m_x;
	}

	int GetY()
	{
		return this->m_y;
	}
};

class CoordEx : public Coord
{
private:
	int m_roomId;
	int m_x;
	int m_y;

public:
	CoordEx(int _roomId, int _x, int _y)
		: Coord(_x, _y), m_roomId(_roomId) { }

	CoordEx() : Coord(0, 0) {

	}

	int GetRoomId()
	{
		return this->m_roomId;
	}

	int GetX()
	{
		return this->m_x;
	}

	int GetY()
	{
		return this->m_y;
	}
};

class RegularFile
{
private:
	ALLEGRO_FILE*   m_allegroFile;
	string          m_filePath;
	string          m_fileName;
	string          m_fileLocation;

public:
	RegularFile(string _filePath)
	{
		this->m_filePath = _filePath;
		this->m_allegroFile = NULL;

		this->m_fileLocation = StringUtils::GetFileLocation(this->m_filePath);
		this->m_fileName = StringUtils::GetFileName(this->m_filePath);
	}

	RegularFile(string _filePath, string _openMode)
	{
		this->m_filePath = _filePath;
		this->m_allegroFile = al_fopen(_filePath.c_str(), _openMode.c_str());

		this->m_fileLocation = StringUtils::GetFileLocation(this->m_filePath);
		this->m_fileName = StringUtils::GetFileName(this->m_filePath);
	}

	//RegularFile(const RegularFile& _regularFile)
	//{
		/*  */
	//}

	~RegularFile()
	{
		if (this->m_allegroFile != NULL) {
			al_fclose(this->m_allegroFile);
			this->m_allegroFile = NULL;
		}
	}

	void OpenFile(string _openMode)
	{
		this->m_allegroFile = al_fopen(this->m_filePath.c_str(), _openMode.c_str());
	}

	string GetFilePath()
	{
		return this->m_filePath;
	}

	string GetFileName()
	{
		return this->m_fileName;
	}

	string GetFileLocation()
	{
		return this->m_fileLocation;
	}

	ALLEGRO_FILE* GetAlInstance()
	{
		return this->m_allegroFile;
	}
};
