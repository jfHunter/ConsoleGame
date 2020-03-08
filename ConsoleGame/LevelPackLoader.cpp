#include <iostream>
#include <map>

#include <allegro5/allegro5.h>

#include "StringUtils.hpp"

#include "LevelPackLoader.h"

#include "GameCommon.h"

using std::endl;
using std::cout;
using std::map;

bool LevelPackValidator::ValidateLevelPackFile(uint32_t magicNumValue)
{
	return (magicNumValue == LEVEL_PACK_MAGIC_NUM) || (SWAP_U32(magicNumValue) == LEVEL_PACK_MAGIC_NUM);
}

bool LevelPackValidator::CheckLevelPackSupport(uint16_t compatible)
{
	return true;
}

LevelPackLoader::LevelPackLoader(LevelPackValidator& _levelPackValidator)
{
	this->m_levelPackValidator = _levelPackValidator;
}

void LevelPackLoader::CreateBinaryObjectTable(LevelPackFileFormat& lpff, BinObjMap& binaryObjMap)
{
	for (int i = 0; i < lpff.lpff_num_bin_data; i++) {
		char data[256] = { 0 };
		memcpy(data, lpff.lpff_bin_data[i].bd_data_bytes, lpff.lpff_bin_data[i].bd_num_data_bytes);
		string dataStr(data);
		binaryObjMap.insert({ lpff.lpff_bin_data[i].bd_idx, dataStr });
	}
}

string LevelPackLoader::GetBinaryObject(BinObjMap& binaryObjMap, int objectId)
{
	return binaryObjMap.at(objectId);
}

BgMusicInfo * LevelPackLoader::GetMusicInfo(LevelPackFileFormat lpff, int bgMusicId)
{
	for (int i = 0; i < lpff.lpff_num_bg_sounds; i++) {
		if (lpff.lpff_bg_sounds[i].bgsnd_idx == bgMusicId) {
			return &lpff.lpff_bg_sounds[i];
		}
	}

	return NULL;
}

void LevelPackLoader::CreateLevelPack(LevelPackFileFormat& lpff)
{
	BinObjMap binaryObjMap;
	LevelList levelList;
	LevelPackInfo lpi = lpff.lpff_level_pack_info;
	GameVersion lpGameVer;
	string lpName, lpAuthor, lpDescription;

	CreateBinaryObjectTable(lpff, binaryObjMap);

	lpName = GetBinaryObject(binaryObjMap, lpi.lpi_name_idx);
	lpAuthor = GetBinaryObject(binaryObjMap, lpi.lpi_author_idx);
	lpDescription = GetBinaryObject(binaryObjMap, lpi.lpi_desc_idx);
	lpGameVer = (GameVersion)lpi.lpi_game_compatible;

	for (int i = 0; i < lpff.lpff_num_levels; i++) {
		RoomList roomList;
		for (int j = 0; j < lpff.lpff_levels[i].li_num_rooms; j++) {
			roomList.push_back(LevelRoom(lpff.lpff_levels[i].li_rooms[j]));
		}

		TargetList targetList;
		for (int j = 0; j < lpff.lpff_levels[i].li_num_targets; j++) {
			targetList.push_back(LevelTarget(lpff.lpff_levels[i].li_targets[j]));
		}

		PortalList portalList;
		for (int j = 0; j < lpff.lpff_levels[i].li_num_tp_portals; j++) {
			portalList.push_back(LevelPortal(lpff.lpff_levels[i].li_tp_portals[j]));
		}

		SymbolList symbolList;
		for (int j = 0; j < lpff.lpff_levels[i].li_num_symbols; j++) {
			symbolList.push_back(LevelSymbol(
				lpff.lpff_levels[i].li_symbols[j].ls_char,
				GetBinaryObject(binaryObjMap, lpff.lpff_levels[i].li_symbols[j].ls_desc)
			));
		}

		BgMusicInfo * musicInfo = GetMusicInfo(lpff, lpff.lpff_levels[i].li_bgmusic_idx);
		if (!musicInfo) {
			return;
		}

		LevelMusic levelMusic(
			GetBinaryObject(binaryObjMap, musicInfo->bgsnd_author_idx),
			GetBinaryObject(binaryObjMap, musicInfo->bgsnd_album_idx),
			GetBinaryObject(binaryObjMap, musicInfo->bgsnd_title_idx),
			musicInfo->bgsnd_len,
			musicInfo->bgsnd_data
		);

		levelList.push_back(Level(roomList, targetList, symbolList, portalList, GetBinaryObject(binaryObjMap, lpff.lpff_levels[i].li_info), levelMusic));
	}

	this->m_levelPack = LevelPack(levelList, lpName, lpAuthor, lpGameVer, lpDescription);
}

bool LevelPackLoader::GetLevelPackFileFormat(LevelPackFileFormat& lpff, char * levelPackBuffer, uint64_t levelPackSize)
{
	uint8_t * bufPtr = (uint8_t *)levelPackBuffer;
	int roomSize;

	GET_U32_BE_AND_MOVE(lpff.lpff_level_magic, bufPtr);
	if (!this->m_levelPackValidator.ValidateLevelPackFile(lpff.lpff_level_magic)) {
		return false;
	}

	/* For getting 16-bit/32-bit numbers, big-endian convention
	 * is used as data is stored in such format in file.
	 * No direct pointer conversions are done for those numbers.
	 */

	GET_U8_AND_MOVE(lpff.lpff_level_pack_info.lpi_name_idx, bufPtr);
	GET_U8_AND_MOVE(lpff.lpff_level_pack_info.lpi_author_idx, bufPtr);
	GET_U8_AND_MOVE(lpff.lpff_level_pack_info.lpi_gen_file_idx, bufPtr);
	GET_U16_BE_AND_MOVE(lpff.lpff_level_pack_info.lpi_game_compatible, bufPtr);
	GET_U8_AND_MOVE(lpff.lpff_level_pack_info.lpi_desc_idx, bufPtr);

	GET_U8_AND_MOVE(lpff.lpff_num_levels, bufPtr);
	lpff.lpff_levels = new LevelInfo[lpff.lpff_num_levels];
	for (int i = 0; i < lpff.lpff_num_levels; i++) {
		GET_U8_AND_MOVE(lpff.lpff_levels[i].li_info, bufPtr);

		GET_U8_AND_MOVE(lpff.lpff_levels[i].li_num_targets, bufPtr);
		if (lpff.lpff_levels[i].li_num_targets) {
			lpff.lpff_levels[i].li_targets = new LevelTargetInfo[lpff.lpff_levels[i].li_num_targets];
			for (int j = 0; j < lpff.lpff_levels[i].li_num_targets; j++) {
				GET_U8_AND_MOVE(lpff.lpff_levels[i].li_targets[j].lt_target_type, bufPtr);
				GET_U8_AND_MOVE(lpff.lpff_levels[i].li_targets[j].obj_a, bufPtr);
				GET_U8_AND_MOVE(lpff.lpff_levels[i].li_targets[j].obj_b, bufPtr);
			}
		}

		GET_U8_AND_MOVE(lpff.lpff_levels[i].li_num_rooms, bufPtr);
		if (lpff.lpff_levels[i].li_num_rooms) {
			lpff.lpff_levels[i].li_rooms = new LevelRoomInfo[lpff.lpff_levels[i].li_num_rooms];
			for (int j = 0; j < lpff.lpff_levels[i].li_num_rooms; j++) {
				GET_U8_AND_MOVE(lpff.lpff_levels[i].li_rooms[j].lr_room_idx, bufPtr);
				GET_U8_AND_MOVE(lpff.lpff_levels[i].li_rooms[j].lr_width, bufPtr);
				GET_U8_AND_MOVE(lpff.lpff_levels[i].li_rooms[j].lr_height, bufPtr);
				roomSize = lpff.lpff_levels[i].li_rooms[j].lr_width * lpff.lpff_levels[i].li_rooms[j].lr_height;
				lpff.lpff_levels[i].li_rooms[j].lr_data = new char[roomSize];
				memcpy(lpff.lpff_levels[i].li_rooms[j].lr_data, bufPtr, roomSize);
				bufPtr += roomSize;
			}
		}

		GET_U8_AND_MOVE(lpff.lpff_levels[i].li_num_symbols, bufPtr);
		if (lpff.lpff_levels[i].li_num_symbols) {
			lpff.lpff_levels[i].li_symbols = new LevelSymbolInfo[lpff.lpff_levels[i].li_num_symbols];
			for (int j = 0; j < lpff.lpff_levels[i].li_num_symbols; j++) {
				GET_U8_AND_MOVE(lpff.lpff_levels[i].li_symbols[j].ls_char, bufPtr);
				GET_U8_AND_MOVE(lpff.lpff_levels[i].li_symbols[j].ls_desc, bufPtr);
			}
		}

		GET_U8_AND_MOVE(lpff.lpff_levels[i].li_num_tp_portals, bufPtr);
		if (lpff.lpff_levels[i].li_num_tp_portals) {
			lpff.lpff_levels[i].li_tp_portals = new LevelPortalInfo[lpff.lpff_levels[i].li_num_tp_portals];
			for (int j = 0; j < lpff.lpff_levels[i].li_num_tp_portals; j++) {
				GET_U8_AND_MOVE(lpff.lpff_levels[i].li_tp_portals[j].lp_room_a_idx, bufPtr);
				GET_U8_AND_MOVE(lpff.lpff_levels[i].li_tp_portals[j].lp_room_b_idx, bufPtr);
				GET_U16_BE_AND_MOVE(lpff.lpff_levels[i].li_tp_portals[j].lp_portal_a_x, bufPtr);
				GET_U16_BE_AND_MOVE(lpff.lpff_levels[i].li_tp_portals[j].lp_portal_a_y, bufPtr);
				GET_U16_BE_AND_MOVE(lpff.lpff_levels[i].li_tp_portals[j].lp_portal_b_y, bufPtr);
				GET_U16_BE_AND_MOVE(lpff.lpff_levels[i].li_tp_portals[j].lp_portal_b_y, bufPtr);
			}
		}

		GET_U8_AND_MOVE(lpff.lpff_levels[i].li_bgmusic_idx, bufPtr);
	}

	GET_U8_AND_MOVE(lpff.lpff_num_bg_sounds, bufPtr);
	if (lpff.lpff_num_bg_sounds) {
		lpff.lpff_bg_sounds = new BgMusicInfo[lpff.lpff_num_bg_sounds];
		for (int i = 0; i < lpff.lpff_num_bg_sounds; i++) {
			GET_U8_AND_MOVE(lpff.lpff_bg_sounds[i].bgsnd_idx, bufPtr);
			GET_U8_AND_MOVE(lpff.lpff_bg_sounds[i].bgsnd_author_idx, bufPtr);
			GET_U8_AND_MOVE(lpff.lpff_bg_sounds[i].bgsnd_album_idx, bufPtr);
			GET_U8_AND_MOVE(lpff.lpff_bg_sounds[i].bgsnd_title_idx, bufPtr);
			GET_U32_BE_AND_MOVE(lpff.lpff_bg_sounds[i].bgsnd_len, bufPtr);
			if (lpff.lpff_bg_sounds[i].bgsnd_len) {
				lpff.lpff_bg_sounds[i].bgsnd_data = new char[lpff.lpff_bg_sounds[i].bgsnd_len];
				memcpy(lpff.lpff_bg_sounds[i].bgsnd_data, bufPtr, lpff.lpff_bg_sounds[i].bgsnd_len);
			}
			bufPtr += lpff.lpff_bg_sounds[i].bgsnd_len;
		}
	}

	GET_U8_AND_MOVE(lpff.lpff_num_bin_data, bufPtr);
	if (lpff.lpff_num_bin_data) {
		lpff.lpff_bin_data = new BinaryDataInfo[lpff.lpff_num_bin_data];
		for (int i = 0; i < lpff.lpff_num_bin_data; i++) {
			//lpff.lpff_bin_data
			GET_U8_AND_MOVE(lpff.lpff_bin_data[i].bd_idx, bufPtr);
			GET_U16_BE_AND_MOVE(lpff.lpff_bin_data[i].bd_num_data_bytes, bufPtr);
			if (lpff.lpff_bin_data[i].bd_num_data_bytes) {
				lpff.lpff_bin_data[i].bd_data_bytes = new char[lpff.lpff_bin_data[i].bd_num_data_bytes];
				memcpy(lpff.lpff_bin_data[i].bd_data_bytes, bufPtr, lpff.lpff_bin_data[i].bd_num_data_bytes);
			}
			bufPtr += lpff.lpff_bin_data[i].bd_num_data_bytes;
		}
	}

	return true;
}

bool LevelPackLoader::LoadLevelPack(LevelPackFile levelPackFile)
{
	ALLEGRO_FILE* alFile;
	LevelPackFileFormat lpff;
	int64_t lpffSize;
	char * lpffBuf;

	levelPackFile.OpenFile("rb");
	alFile = levelPackFile.GetAlInstance();
	lpffSize = al_fsize(alFile);
	lpffBuf = new char[lpffSize];
	al_fread(alFile, lpffBuf, lpffSize);

	this->GetLevelPackFileFormat(lpff, lpffBuf, lpffSize);
	this->CreateLevelPack(lpff);

	return true;
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
