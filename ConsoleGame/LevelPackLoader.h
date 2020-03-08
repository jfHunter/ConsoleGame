#pragma once

#include <string>
#include <vector>
#include <map>

#include <allegro5/allegro.h>
#include <allegro5/allegro_physfs.h>
#include <allegro5/file.h>

#include "GameCommon.h"
#include "LevelPack.h"
#include "LevelPackFileFormat.h"

#define LEVELPACKS_DIR			"levelpacks\\"

using std::string;
using std::vector;
using std::map;

typedef RegularFile LevelPackFile;
typedef vector<LevelPackFile> LevelPackFileList;
typedef map<int, string> BinObjMap;

/*
 * Class LevelPackValidator
 *
 * Verifies level pack data
 */
class LevelPackValidator
{
public:

	/**
	 * Function validates level pack file.
	 * It checks level pack magic number to recognize
	 * if it's true level pack file.
	 *
	 * Parameters:
	 *     @param magicNumber               : magic number value
	 *
	 * Returns:
	 *     true : if provided file is valid level pack file
	 *     false : if provided file is not level pack file
	 */
	bool ValidateLevelPackFile(uint32_t magicNumber);

	/**
	 * Function checks game version support of
	 * provided level pack version number if
	 * this level pack is compatible to run
	 * on this game version.
	 *
	 * Parameters:
	 *     @param compatible                : level pack version
	 *
	 * Returns:
	 *     true : if current game version supports this level pack
	 *     false : if level pack is not supported for this game version (too old)
	 */
	bool CheckLevelPackSupport(uint16_t compatible);
};

class LevelPackLoader
{
private:
	LevelPack            m_levelPack;
	LevelPackValidator   m_levelPackValidator;

	BgMusicInfo * GetMusicInfo(LevelPackFileFormat lpff, int bgMusicId);
	string GetBinaryObject(BinObjMap& binaryObjMap, int objectId);
	void CreateBinaryObjectTable(LevelPackFileFormat& lpff, BinObjMap& binaryObjMap);
	void CreateLevelPack(LevelPackFileFormat& lpff);
	bool GetLevelPackFileFormat(LevelPackFileFormat& lpff, char * levelPackBuffer, uint64_t levelPackSize);

public:
	LevelPackLoader(LevelPackValidator& _levelPackValidator);

	/**
	 * Function scans level pack base directory.
	 *
	 * Returns:
	 *     list of level pack files
	 */
	LevelPackFileList ScanLevelPackDirectory();

	/**
	 * Function loads specified level pack denoted
	 * by its file.
	 *
	 * Parameters:
	 *     levelPackFile                    : level pack file to load
	 *
	 * Returns:
	 *     true : if no error occured during level pack load
	 *     false : if failed to load level pack
	 */
	bool LoadLevelPack(LevelPackFile levelPackFile);

	/**
	 * Function returns loaded level pack
	 *
	 * Returns:
	 *     level pack object
	 */
	LevelPack GetLevelPack();
};
