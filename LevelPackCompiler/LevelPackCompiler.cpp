#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <Windows.h>

#include <allegro5/allegro.h>

#include "Console.h"
#include "Utils.h"

#include "StringUtils.hpp"

#include "LevelPackFileFormat.h"

using std::string;
using std::fstream;
using std::ios;
using std::vector;
using std::cout;
using std::endl;

/*
 * TODO list:
 *     - zrobic szyfrowanie danego levelu
 */

typedef struct {
	int plri_index;
	string plri_room;
} ParsedLevelRoomInfo;

typedef struct {
	int pli_num_rooms;
	vector<ParsedLevelRoomInfo> pli_rooms;
	string pli_symbols;
	string pli_targets;
	string pli_teleport;
	string pli_info;
	string pli_bgsnd_file;
	string pli_bgsnd_author;
	string pli_bgsnd_album;
	string pli_bgsnd_title;
} ParsedLevelInfo;

typedef struct {
	int plpi_num_levels;
	vector<ParsedLevelInfo> plpi_levels;
	string plpi_name;
	string plpi_author;
	string plpi_file;
	string plpi_compatible;
	string plpi_description;
} ParsedLevelPackInfo;

static void Help()
{
	cout << "> lpc.exe <lev_script0>[ <lev_script1>[ <lev_script2>]] - compiles level pack" << endl;
	cout << "> lpc.exe <directory> - compiles all level packs inside <directory>" << endl;
}

static void AutoFlushBuffer(bool& readMultiLineMode, vector<string>& vector, string* param)
{
	if (readMultiLineMode) {
		*param = "";
		for (size_t i = 0; i < vector.size(); i++) {
			*param += vector.at(i) + "\n";
		}
		vector.clear();
		readMultiLineMode = false;
	}
}

static void PrintLevelPackInfo(ParsedLevelPackInfo levelPackInfo)
{
	cout << "Level pack" << endl;

	cout << "\t Name: " << levelPackInfo.plpi_name << endl;
	cout << "\t Author: " << levelPackInfo.plpi_author<< endl;
	cout << "\t Compatible: " << levelPackInfo.plpi_compatible << endl;
	cout << "\t Gen File: " << levelPackInfo.plpi_file << endl;
	cout << "\t Description: " << levelPackInfo.plpi_description << endl;

	cout << "\t Number of levels: " << levelPackInfo.plpi_num_levels << endl;

	for (int i = 0; i < levelPackInfo.plpi_num_levels; i++) {
		ParsedLevelInfo levelInfo = levelPackInfo.plpi_levels.at(i);
		cout << "\t\t Level targets: \n" << levelInfo.pli_targets << endl;
		cout << "\t\t Level symbols: \n" << levelInfo.pli_symbols << endl;
		cout << "\t\t Level teleport: \n" << levelInfo.pli_teleport << endl;
		cout << "\t\t Level info: \n" << levelInfo.pli_info << endl;
		cout << "\t\t Level BG music file: " << levelInfo.pli_bgsnd_file << endl;
		cout << "\t\t Level BG music author: " << levelInfo.pli_bgsnd_author<< endl;
		cout << "\t\t Level BG music album: " << levelInfo.pli_bgsnd_album << endl;
		cout << "\t\t Level BG music title: " << levelInfo.pli_bgsnd_title << endl;

		cout << "\t\t Number of level rooms: " << levelInfo.pli_num_rooms << endl;
		for (int j = 0; j < levelInfo.pli_num_rooms; j++) {
			cout << "\t\t\t Room index: " << levelInfo.pli_rooms.at(j).plri_index << endl;
			cout << "\t\t\t Room data: \n" << levelInfo.pli_rooms.at(j).plri_room;
		}
	}

	cout << endl;
}

static void ReadLevelPackScript(ALLEGRO_FS_ENTRY* alFile, ParsedLevelPackInfo& levelPackInfo)
{
	ALLEGRO_FILE* alFileStream = al_open_fs_entry(alFile, "r");
	ParsedLevelRoomInfo levelRoomInfo = { 0 };
	ParsedLevelInfo levelInfo = { 0 };
	string* multiLineParam = nullptr;
	vector<string> multiLineVec;
	string currentLine;
	char lineBuffer[255];
	bool lpHeaderFound = false;
	bool lpLevelOpened = false;
	bool lpLevelRoomOpened = false;
	bool lpBgMusicOpened = false;
	bool readMultiLine = false;
	int lineCount = 0;

	while (!al_feof(alFileStream) && (al_fgets(alFileStream, lineBuffer, 255) != NULL)) {
		currentLine = String(lineBuffer);
		currentLine = StringUtils::Trim(currentLine);
		if ((currentLine != "") && (currentLine[0] == ';')) {
			continue;
		}

		if (StringUtils::Equals(currentLine, LP_LEVEL_PACK_KEY)) { /* detected .levelpack header */
			lpHeaderFound = true;
		} else {
			if (lpHeaderFound) { /* .levelpack section opened */
				if (StringUtils::Contains(currentLine, LP_LEVEL_START_KEY)) {
					if (lpLevelOpened) {
						cout << "ERROR on parsing level, missing .endlevel section" << endl;
						goto done;
					} else {
						AutoFlushBuffer(readMultiLine, multiLineVec, multiLineParam);
						lpLevelOpened = true;
					}
				}
				else if (StringUtils::Contains(currentLine, LP_LEVEL_ROOM_START_KEY))
				{
					if (lpLevelRoomOpened) {
						cout << "ERROR on parsing level room, missing .endroom section" << endl;
						goto done;
					} else {
						AutoFlushBuffer(readMultiLine, multiLineVec, multiLineParam);
						lpLevelRoomOpened = true;
						readMultiLine = true;

						/* fetch room index */
						sscanf_s(currentLine.c_str(), String(String(LP_LEVEL_ROOM_START_KEY) + " %d").c_str(), &levelRoomInfo.plri_index);

						/* set room data */
						multiLineParam = &levelRoomInfo.plri_room;
					}
				}
				else if (StringUtils::Contains(currentLine, LP_LEVEL_ROOM_END_KEY))
				{
					if (!lpLevelRoomOpened) {
						cout << "ERROR on parsing level room, missing .room <id> section" << endl;
						goto done;
					} else {
						AutoFlushBuffer(readMultiLine, multiLineVec, multiLineParam);
							
						/* increment number of rooms per level */
						levelInfo.pli_num_rooms++;

						/* add new level room */
						levelInfo.pli_rooms.push_back(levelRoomInfo);

						/* clear level room structure */
						levelRoomInfo = { 0 };

						lpLevelRoomOpened = false;
					}
				}
				else if (StringUtils::Contains(currentLine, LP_LEVEL_END_KEY))
				{
					if (!lpLevelOpened) {
						cout << "ERROR on parsing level, missing .level section" << endl;
						goto done;
					} else {
						AutoFlushBuffer(readMultiLine, multiLineVec, multiLineParam);
						lpLevelOpened = false;
						lpBgMusicOpened = false;

						/* add new level */
						levelPackInfo.plpi_levels.push_back(levelInfo);

						/* clear level info structure */
						levelInfo = { 0 };

						/* increment number of levels per level pack */
						levelPackInfo.plpi_num_levels++;
					}
				}
				else if (StringUtils::Contains(currentLine, LP_LEVEL_SYMBOLS_KEY))
				{
					AutoFlushBuffer(readMultiLine, multiLineVec, multiLineParam);
					readMultiLine = true;
					multiLineParam = &levelInfo.pli_symbols;
				}
				else if (StringUtils::Contains(currentLine, LP_LEVEL_TARGETS_KEY))
				{
					AutoFlushBuffer(readMultiLine, multiLineVec, multiLineParam);
					readMultiLine = true;
					multiLineParam = &levelInfo.pli_targets;
				}
				else if (StringUtils::Contains(currentLine, LP_LEVEL_INFO_KEY))
				{
					AutoFlushBuffer(readMultiLine, multiLineVec, multiLineParam);
					readMultiLine = true;
					multiLineParam = &levelInfo.pli_info;
				}
				else if (StringUtils::Contains(currentLine, LP_LEVEL_BGMUSIC_KEY))
				{
					AutoFlushBuffer(readMultiLine, multiLineVec, multiLineParam);
					lpBgMusicOpened = true;
				}
				else
				{
					if (StringUtils::Contains(currentLine, ":")) /* detected colon character `:` (script parameter) */
					{
						size_t colonIndex = StringUtils::IndexOf(currentLine, ':');
						string key = currentLine.substr(0, colonIndex);
						string value = currentLine.substr(colonIndex + 1, currentLine.length() - colonIndex);
						if (lpBgMusicOpened)
						{
							if (StringUtils::Equals(key, LP_LEVEL_MUSIC_FILE_KEY)) {
								if ((currentLine.length() - 1) == colonIndex) {
									readMultiLine = true;
									multiLineParam = &levelInfo.pli_bgsnd_file;
								} else {
									levelInfo.pli_bgsnd_file = StringUtils::Trim(value);
								}
							} else if (StringUtils::Equals(key, LP_LEVEL_MUSIC_AUTHOR_KEY)) {
								if ((currentLine.length() - 1) == colonIndex) {
									readMultiLine = true;
									multiLineParam = &levelInfo.pli_bgsnd_author;
								} else {
									levelInfo.pli_bgsnd_author = StringUtils::Trim(value);
								}
							} else if (StringUtils::Equals(key, LP_LEVEL_MUSIC_ALBUM_KEY)) {
								if ((currentLine.length() - 1) == colonIndex) {
									readMultiLine = true;
									multiLineParam = &levelInfo.pli_bgsnd_album;
								} else {
									levelInfo.pli_bgsnd_album = StringUtils::Trim(value);
								}
							} else if (StringUtils::Equals(key, LP_LEVEL_MUSIC_TITLE_KEY)) {
								if ((currentLine.length() - 1) == colonIndex) {
									readMultiLine = true;
									multiLineParam = &levelInfo.pli_bgsnd_title;
								} else {
									levelInfo.pli_bgsnd_title = StringUtils::Trim(value);
								}
							}
						} else {
							if (StringUtils::Equals(key, LP_NAME_KEY)) {
								if ((currentLine.length() - 1) == colonIndex) {
									readMultiLine = true;
									multiLineParam = &levelPackInfo.plpi_name;
								} else {
									levelPackInfo.plpi_name = StringUtils::Trim(value);
								}
							}
							else if (StringUtils::Equals(key, LP_AUTHOR_KEY)) {
								if ((currentLine.length() - 1) == colonIndex) {
									readMultiLine = true;
									multiLineParam = &levelPackInfo.plpi_author;
								} else {
									levelPackInfo.plpi_author = StringUtils::Trim(value);
								}
							}
							else if (StringUtils::Equals(key, LP_COMPATIBLE_KEY)) {
								if ((currentLine.length() - 1) == colonIndex) {
									readMultiLine = true;
									multiLineParam = &levelPackInfo.plpi_compatible;
								} else {
									levelPackInfo.plpi_compatible = StringUtils::Trim(value);
								}
							}
							else if (StringUtils::Equals(key, LP_FILE_KEY)) {
								if ((currentLine.length() - 1) == colonIndex) {
									readMultiLine = true;
									multiLineParam = &levelPackInfo.plpi_file;
								} else {
									levelPackInfo.plpi_file = StringUtils::Trim(value);
								}
							}
							else if (StringUtils::Equals(key, LP_DESCRIPTION_KEY)) {
								if ((currentLine.length() - 1) == colonIndex) {
									readMultiLine = true;
									multiLineParam = &levelPackInfo.plpi_description;
								} else {
									levelPackInfo.plpi_description = StringUtils::Trim(value);
								}
							}
						}
					} else { /* detected other non-param data */
						if (readMultiLine) {
							if (currentLine == "") {
								AutoFlushBuffer(readMultiLine, multiLineVec, multiLineParam);
							} else {
								multiLineVec.push_back(StringUtils::Trim(currentLine));
							}
						}
					}
				}
			} else {
				cout << "Cannot parse level pack script! Missing .levelpack header" << endl;
				goto done;
			}
		}

		lineCount++;
	}

done:
	al_fclose(alFileStream);
}

static void WriteBinData(ALLEGRO_FILE* alFileStream, vector<string>& strVect, string strData)
{
	strVect.push_back(strData);
	uint8_t strIndex = (uint8_t)strVect.size();
	al_fputc(alFileStream, (int)strIndex);
}

static uint8_t GetBgMusicId(vector<string>& musicVect, string soundFile)
{
	musicVect.push_back(soundFile);
	return (uint8_t)musicVect.size();
}

static uint8_t GetMatchingMusicId(vector<string>& musicVect, std::string soundFile)
{
	for (size_t i = 0; i < musicVect.size(); i++) {
		if (StringUtils::Equals(musicVect.at(i), soundFile)) {
			return (uint8_t)(i + 1);
		}
	}

	return 0;
}

static bool ReadMusicFileData(std::string filePath, uint8_t* data, uint64_t size)
{
	ALLEGRO_FILE* alFileMusic = al_fopen(filePath.c_str(), "rb");

	if (alFileMusic == NULL) {
		return false;
	}

	al_fread(alFileMusic, data, size);

	al_fclose(alFileMusic);

	return true;
}

static bool GetMusicInfo(std::string filePath, uint64_t& length)
{
	ALLEGRO_FILE* alFileMusic = al_fopen(filePath.c_str(), "rb");
	int64_t musicFileSize;

	if (alFileMusic == NULL) {
		return false;
	}

	musicFileSize = al_fsize(alFileMusic);
	if (musicFileSize == (int64_t)-1) {
		return false;
	}

	length = (uint64_t)musicFileSize;

	al_fclose(alFileMusic);

	return true;
}

static void SaveLevelPackFile(ParsedLevelPackInfo levelPackInfo, string location)
{
	string filePath = location + '\\' + levelPackInfo.plpi_file;
	ALLEGRO_FILE* alFileStream;
	vector<string> binDataVect;
	vector<string> musicInfoVect;

	if (al_filename_exists(filePath.c_str())) {
		al_remove_filename(filePath.c_str());
	}

	alFileStream = al_fopen(filePath.c_str(), "wb");

	if (alFileStream != NULL)
	{
		/* Magic number 4B */
		al_fwrite32be(alFileStream, LEVEL_PACK_MAGIC_NUM);

		/* Level pack name 1B */
		WriteBinData(alFileStream, binDataVect, levelPackInfo.plpi_name);

		/* Level pack author 1B */
		WriteBinData(alFileStream, binDataVect, levelPackInfo.plpi_author);

		/* Level pack generation file 1B */
		WriteBinData(alFileStream, binDataVect, levelPackInfo.plpi_file);

		/* Level pack version (compatible) 2B */
		al_fwrite16be(alFileStream, ParseGameCompatible(levelPackInfo.plpi_compatible));

		/* Level pack description 1B */
		WriteBinData(alFileStream, binDataVect, levelPackInfo.plpi_description);

		/* Number of levels 1B */
		al_fputc(alFileStream, levelPackInfo.plpi_num_levels);
		for (int i = 0; i < levelPackInfo.plpi_num_levels; i++) {
			ParsedLevelInfo levelInfo = levelPackInfo.plpi_levels.at(i);
			StringVect levelTargets = StringUtils::Split(levelInfo.pli_targets, '\n');
			StringVect levelSymbols = StringUtils::Split(levelInfo.pli_symbols, '\n');

			/* Level information 1B */
			WriteBinData(alFileStream, binDataVect, levelInfo.pli_info);
			
			/* Level number of targets */
			al_fputc(alFileStream, levelTargets.size());
			for (size_t j = 0; j < levelTargets.size(); j++) {
				LevelTargetInfo levelTargetInfo = ParseLevelTarget(levelTargets.at(j));

				/* Target type 1B */
				al_fputc(alFileStream, levelTargetInfo.lt_target_type);

				/* Object A 1B */
				al_fputc(alFileStream, levelTargetInfo.obj_a);

				/* Object B 1B */
				al_fputc(alFileStream, levelTargetInfo.obj_b);
			}

			/* Number of rooms */
			al_fputc(alFileStream, levelInfo.pli_num_rooms);
			for (int j = 0; j < levelInfo.pli_num_rooms; j++) {
				StringVect roomMapLines = StringUtils::Split(levelInfo.pli_rooms.at(j).plri_room, '\n');
				string roomMapBytes = StringUtils::Join(roomMapLines, "");

				if (roomMapLines.size() < 1) {
					cout << "ERROR: Invalid room data" << endl;
					goto done;
				}

				/* Room index 1B */
				al_fputc(alFileStream, levelInfo.pli_rooms.at(j).plri_index);

				/* Room width 1B */
				al_fputc(alFileStream, roomMapLines.at(0).length());

				/* Room height 1B */
				al_fputc(alFileStream, roomMapLines.size());

				/* Room bytes vary - <num_room_bytes> */
				al_fwrite(alFileStream, roomMapBytes.c_str(), roomMapBytes.length());
			}

			/* Number of symbols 1B */
			al_fputc(alFileStream, levelSymbols.size());
			for (size_t j = 0; j < levelSymbols.size(); j++) {
				char levelSymChr = ParseSymbolCharacter(levelSymbols.at(j));
				string levelSymDesc = ParseSymbolDescription(levelSymbols.at(j));

				/* Symbol character 1B */
				al_fputc(alFileStream, levelSymChr);

				/* Symbol description 1B */
				WriteBinData(alFileStream, binDataVect, levelSymDesc);
			}

			/* TODO: .teleport */

			/* Background music Id */
			al_fputc(alFileStream, GetBgMusicId(musicInfoVect, levelInfo.pli_bgsnd_file));
		}

		/* Number of bg sounds - number of levels 1B */
		al_fputc(alFileStream, levelPackInfo.plpi_num_levels);
		for (int i = 0; i < levelPackInfo.plpi_num_levels; i++) {
			ParsedLevelInfo levelInfo = levelPackInfo.plpi_levels.at(i);
			string musicFilePath = String(location) + '\\' + levelInfo.pli_bgsnd_file;
			uint64_t musicDataLen;
			uint8_t* musicData;

			/* BgSound Id 1B */
			al_fputc(alFileStream, GetMatchingMusicId(musicInfoVect, levelInfo.pli_bgsnd_file));

			/* BgSound author 1B */
			WriteBinData(alFileStream, binDataVect, levelInfo.pli_bgsnd_author);

			/* BgSound album 1B */
			WriteBinData(alFileStream, binDataVect, levelInfo.pli_bgsnd_album);

			/* BgSound title 1B */
			WriteBinData(alFileStream, binDataVect, levelInfo.pli_bgsnd_title);

			if (!GetMusicInfo(musicFilePath, musicDataLen)) {
				cout << "Failed to open sound file" << endl;
				goto done;
			}

			/* BgSound size 4B */
			al_fwrite32be(alFileStream, musicDataLen);

			musicData = new uint8_t[musicDataLen];

			if (!ReadMusicFileData(musicFilePath, musicData, musicDataLen)) {
				cout << "Failed to read sound file" << endl;
				goto done;
			}

			/* BgSound data vary <num_bg_sound_bytes> */
			al_fwrite(alFileStream, musicData, musicDataLen);

			delete[] musicData;
		}

		/* Number of binary data objects 1B */
		al_fputc(alFileStream, binDataVect.size());
		for (size_t j = 0; j < binDataVect.size(); j++) {
			/* Binary data object ID 1B */
			al_fputc(alFileStream, j + 1);

			/* Size of binary data object 2B */
			al_fwrite16be(alFileStream, (uint16_t)binDataVect.at(j).length());

			/* Binary data object vary - <num_bin_data_bytes> */
			al_fwrite(alFileStream, binDataVect.at(j).c_str(), binDataVect.at(j).length());
		}
	} else {
		cout << "Cannot open file for writing" << endl;
	}

done:
	al_fclose(alFileStream);
}

static void ProcessFile(ALLEGRO_FS_ENTRY* alFile, string& currentDir, ParsedLevelPackInfo& levelPackInfo)
{
	ALLEGRO_PATH* alFilePath = al_create_path(al_get_fs_entry_name(alFile));
	if (al_get_fs_entry_mode(alFile) & ALLEGRO_FILEMODE_ISFILE) {
		if (String(al_get_path_extension(alFilePath)) == ".lpscr") {
			cout << "Processing file: " << al_get_path_filename(alFilePath) << endl;
			currentDir = StringUtils::GetFileLocation(String(al_get_fs_entry_name(alFile)));
			ReadLevelPackScript(alFile, levelPackInfo);
		}
	} else {
		cout << "Processing directory: " << al_get_path_filename(alFilePath) << endl;
		if (!al_open_directory(alFile)) {
			cout << "ERROR: Failed to open directory" << endl;
		}

		ALLEGRO_FS_ENTRY* nextAlFile;
		while ((nextAlFile = al_read_directory(alFile)) != NULL) {
			ProcessFile(nextAlFile, currentDir, levelPackInfo);
		}

		al_close_directory(alFile);
	}
	al_destroy_path(alFilePath);
}

int main(int argc, char** argv)
{
	al_init();
	al_set_standard_fs_interface();
	al_set_standard_file_interface();

	cout << "Level pack compiler (C) 2020" << endl;
	cout << "Author: shjeff, version: 1.0" << endl;
	cout << endl;

	if (argc > 1)
	{
		for (int i = 1; i < argc; i++) {
			ALLEGRO_FS_ENTRY* levelPackAlFile = al_create_fs_entry(argv[i]);
			string currentDir;
			ParsedLevelPackInfo levelPackInfo = { 0 };

			ProcessFile(levelPackAlFile, currentDir, levelPackInfo);
			SaveLevelPackFile(levelPackInfo, currentDir);

			al_destroy_fs_entry(levelPackAlFile);
		}
	}
	else
	{
		Help();
	}

	system("PAUSE");

	return 0;
}
