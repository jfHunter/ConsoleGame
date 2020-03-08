#include <iostream>

#include <allegro5/allegro5.h>

#include "StringUtils.hpp"

#include "LevelPackLoader.h"

using std::endl;
using std::cout;

bool LevelPackValidator::ValidateLevelPackFile(ALLEGRO_FILE* alFile)
{
	uint32_t levelPackMagic = al_fread32be(alFile);
	return (levelPackMagic == LEVEL_PACK_MAGIC_NUM);
}

bool LevelPackValidator::CheckLevelPackSupport(uint16_t compatible)
{
	return true;
}

LevelPackLoader::LevelPackLoader(LevelPackValidator& _levelPackValidator)
{
	this->m_levelPackValidator = _levelPackValidator;
}

bool LevelPackLoader::LoadLevelPack(LevelPackFile levelPackFile)
{
	bool status = false;
	bool isCompatible = false;
	uint32_t levRoomSize;
	ALLEGRO_FILE* alFile;
	LevelPackFileFormat levelPackFormat;

	levelPackFile.OpenFile("rb");
	alFile = levelPackFile.GetAlInstance();
	if (alFile) {
		if (!this->m_levelPackValidator.ValidateLevelPackFile(alFile)) {
			cout << "Invalid level pack magic number" << endl;
			goto end;
		}

		levelPackFormat.lpff_level_magic = LEVEL_PACK_MAGIC_NUM;

		levelPackFormat.lpff_level_pack_info.lpi_name_idx = al_fgetc(alFile);
		levelPackFormat.lpff_level_pack_info.lpi_author_idx = al_fgetc(alFile);
		levelPackFormat.lpff_level_pack_info.lpi_gen_file_idx = al_fgetc(alFile);
		levelPackFormat.lpff_level_pack_info.lpi_game_compatible = al_fread16be(alFile);
		levelPackFormat.lpff_level_pack_info.lpi_desc_idx = al_fgetc(alFile);

		isCompatible = this->m_levelPackValidator.CheckLevelPackSupport(levelPackFormat.lpff_level_pack_info.lpi_game_compatible);

		levelPackFormat.lpff_num_levels = al_fgetc(alFile);
		if (levelPackFormat.lpff_num_levels == 0) {
			cout << "Invalid number of levels" << endl;
			goto end;
		}
		levelPackFormat.lpff_levels = new LevelInfo[levelPackFormat.lpff_num_levels];

		for (int i = 0; i < levelPackFormat.lpff_num_levels; i++) {
			levelPackFormat.lpff_levels[i].li_info = al_fgetc(alFile);

			levelPackFormat.lpff_levels[i].li_num_targets = al_fgetc(alFile);
			if (levelPackFormat.lpff_levels[i].li_num_targets == 0) {
				cout << "Invalid number of targets" << endl;
				goto end;
			}

			levelPackFormat.lpff_levels[i].li_targets = new LevelTargetInfo[levelPackFormat.lpff_levels[i].li_num_targets];
			for (int j = 0; j < levelPackFormat.lpff_levels[i].li_num_targets; j++) {
				levelPackFormat.lpff_levels[i].li_targets[j].lt_target_type = al_fgetc(alFile);
				levelPackFormat.lpff_levels[i].li_targets[j].obj_a = al_fgetc(alFile);
				levelPackFormat.lpff_levels[i].li_targets[j].obj_b = al_fgetc(alFile);
			}

			levelPackFormat.lpff_levels[i].li_num_rooms = al_fgetc(alFile);
			if (levelPackFormat.lpff_levels[i].li_num_rooms == 0) {
				cout << "Invalid number of level rooms" << endl;
				goto end;
			}

			levelPackFormat.lpff_levels[i].li_rooms = new LevelRoomInfo[levelPackFormat.lpff_levels[i].li_num_rooms];
			for (int j = 0; j < levelPackFormat.lpff_levels[i].li_num_rooms; j++) {
				levelPackFormat.lpff_levels[i].li_rooms[j].lr_room_idx = al_fgetc(alFile);
				levelPackFormat.lpff_levels[i].li_rooms[j].lr_width = al_fgetc(alFile);
				levelPackFormat.lpff_levels[i].li_rooms[j].lr_height = al_fgetc(alFile);

				levRoomSize = levelPackFormat.lpff_levels[i].li_rooms[j].lr_width * levelPackFormat.lpff_levels[i].li_rooms[j].lr_height;
				if (levRoomSize == 0) {
					cout << "Invalid number of room width or height" << endl;
					goto end;
				}

				levelPackFormat.lpff_levels[i].li_rooms[j].lr_data = new char[levRoomSize];
				al_fread(alFile, levelPackFormat.lpff_levels[i].li_rooms[j].lr_data, levRoomSize);
			}

			levelPackFormat.lpff_levels[i].li_num_symbols = al_fgetc(alFile);
			if (levelPackFormat.lpff_levels[i].li_num_symbols == 0) {
				cout << "Invalid number of level symbols" << endl;
				goto end;
			}

			levelPackFormat.lpff_levels[i].li_symbols = new LevelSymbolInfo[levelPackFormat.lpff_levels[i].li_num_symbols];
			for (int j = 0; j < levelPackFormat.lpff_levels[i].li_num_symbols; j++) {
				levelPackFormat.lpff_levels[i].li_symbols[j].ls_char = al_fgetc(alFile);
				levelPackFormat.lpff_levels[i].li_symbols[j].ls_desc = al_fgetc(alFile);
			}

			/* TODO: support portals */
			levelPackFormat.lpff_levels[i].li_num_tp_portals = 0;
			levelPackFormat.lpff_levels[i].li_tp_portals = NULL;

			levelPackFormat.lpff_levels[i].li_bgmusic_idx = al_fgetc(alFile);
		}

		levelPackFormat.lpff_num_bg_sounds = al_fgetc(alFile);
		if (levelPackFormat.lpff_num_bg_sounds == 0) {
			cout << "Invalid number of bg sounds" << endl;
			goto end;
		}

		levelPackFormat.lpff_bg_sounds = new BgMusicInfo[levelPackFormat.lpff_num_bg_sounds];
		for (int i = 0; i < levelPackFormat.lpff_num_bg_sounds; i++) {
			levelPackFormat.lpff_bg_sounds[i].bgsnd_idx = al_fgetc(alFile);
			levelPackFormat.lpff_bg_sounds[i].bgsnd_author_idx = al_fgetc(alFile);
			levelPackFormat.lpff_bg_sounds[i].bgsnd_album_idx = al_fgetc(alFile);
			levelPackFormat.lpff_bg_sounds[i].bgsnd_title_idx = al_fgetc(alFile);
			levelPackFormat.lpff_bg_sounds[i].bgsnd_len = al_fread32be(alFile);

			if (levelPackFormat.lpff_bg_sounds[i].bgsnd_len == 0) {
				cout << "Invalid number of bg sound length" << endl;
				goto end;
			}

			levelPackFormat.lpff_bg_sounds[i].bgsnd_data = new uint8_t[levelPackFormat.lpff_bg_sounds[i].bgsnd_len];
			al_fread(alFile, levelPackFormat.lpff_bg_sounds[i].bgsnd_data, levelPackFormat.lpff_bg_sounds[i].bgsnd_len);
		}

		levelPackFormat.lpff_num_bin_data = al_fgetc(alFile);
		if (levelPackFormat.lpff_num_bin_data == 0) {
			cout << "Invalid number of binary data object" << endl;
			goto end;
		}

		levelPackFormat.lpff_bin_data = new BinaryDataInfo[levelPackFormat.lpff_num_bin_data];
		for (int i = 0; i < levelPackFormat.lpff_num_bin_data; i++) {
			levelPackFormat.lpff_bin_data[i].bd_idx = al_fgetc(alFile);
			levelPackFormat.lpff_bin_data[i].bd_num_data_bytes = al_fread16be(alFile);

			if (levelPackFormat.lpff_bin_data[i].bd_num_data_bytes == 0) {
				cout << "Invalid number of binary data length" << endl;
				goto end;
			}

			levelPackFormat.lpff_bin_data[i].bd_data_bytes = new char[levelPackFormat.lpff_bin_data[i].bd_num_data_bytes];
			al_fread(alFile, levelPackFormat.lpff_bin_data[i].bd_data_bytes, levelPackFormat.lpff_bin_data[i].bd_num_data_bytes);
		}

		this->m_levelPack = LevelPack(levelPackFormat, isCompatible);

		status = true;
	}
end:
	return status;
}

LevelPackFileList LevelPackLoader::ScanLevelPackDirectory()
{
	string currentDirectory = al_get_current_directory();
	LevelPackFileList levelPackFileList;
	ALLEGRO_FS_ENTRY * alFsEntry = al_create_fs_entry(String(currentDirectory + '\\' + LEVELPACKS_DIR).c_str());
	if (alFsEntry) {
		if (al_open_directory(alFsEntry)) {
			ALLEGRO_FS_ENTRY* nextAlFile;
			while ((nextAlFile = al_read_directory(alFsEntry)) != NULL) {
				if (al_get_fs_entry_mode(nextAlFile) & ALLEGRO_FILEMODE_ISFILE) {
					levelPackFileList.push_back(LevelPackFile(al_get_fs_entry_name(nextAlFile)));
				}
			}
		}
		al_destroy_fs_entry(alFsEntry);
	}

	return levelPackFileList;
}

LevelPack LevelPackLoader::GetLevelPack()
{
	return this->m_levelPack;
}
