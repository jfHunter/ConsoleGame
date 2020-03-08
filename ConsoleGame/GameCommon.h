#pragma once

#include <iostream>
#include <string>

#include <allegro5/allegro.h>
#include <allegro5/file.h>

#include "StringUtils.hpp"

#include "LevelPackFileFormat.h"

#define GAME_VERSION               GAME_VERSION_10

#define GET_U32_BE(ptr_)           (((ptr_)[0] << 24) | ((ptr_)[1] << 16) | ((ptr_)[2] << 8) | ((ptr_)[3] << 0))
#define GET_U16_BE(ptr_)           (((ptr_)[0] << 8)  | ((ptr_)[1] << 0))
#define GET_U8(ptr_)               ((ptr_)[0])

#define SWAP_U32(val_)             ((((val_ >> 24) & 0xFF) << 0) | (((val_ >> 16) & 0xFF) << 8) | (((val_ >> 8) & 0xFF) << 16) | (((val_ >> 0) & 0xFF) << 24))
#define SWAP_U16(val_)             ((((val_ >> 8) & 0xFF) << 0) | (((val_ >> 0) & 0xFF) << 8))

#define GET_U32_BE_AND_MOVE(var_, ptr_) \
	{ var_ = GET_U32_BE(ptr_); ptr_ += 4; }

#define GET_U16_BE_AND_MOVE(var_, ptr_) \
	{ var_ = GET_U16_BE(ptr_); ptr_ += 2; }

#define GET_U8_AND_MOVE(var_, ptr_) \
	{ var_ = GET_U8(ptr_); ptr_ += 1; }

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
