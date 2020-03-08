#pragma once

#include <string>
#include <vector>

#include <allegro5/allegro.h>
#include <allegro5/allegro_physfs.h>
#include <allegro5/file.h>

#include "GameCommon.h"
#include "LevelPack.h"
#include "LevelPackFileFormat.h"

#define LEVELPACKS_DIR			"levelpacks\\"

using std::string;
using std::vector;

typedef RegularFile LevelPackFile;
typedef vector<LevelPackFile> LevelPackFileList;

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
	 *     @param alFile                    : allegro's library file instance of level pack
	 *
	 * Returns:
	 *     true : if provided file is valid level pack file
	 *     false : if provided file is not level pack file
	 */
	bool ValidateLevelPackFile(ALLEGRO_FILE* alFile);

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

public:
	LevelPackLoader(LevelPackValidator& _levelPackValidator);

	LevelPackFileList ScanLevelPackDirectory();
	bool LoadLevelPack(LevelPackFile levelPackFile);
	LevelPack GetLevelPack();
};
