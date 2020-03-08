#include <vector>
#include <string>

#include "Level.h"
#include "LevelPackFileFormat.h"

#include "LevelPack.h"

using std::vector;
using std::string;

LevelPack::LevelPack()
{

}

void LevelPack::CreateBinDataTable()
{
	for (int i = 0; i < m_lpFormat.lpff_num_bin_data; i++) {
		char dataTemp[255] = { 0 };
		BinaryDataInfo binData = m_lpFormat.lpff_bin_data[i];
		memcpy(dataTemp, binData.bd_data_bytes, binData.bd_num_data_bytes);
		string binStr = string(dataTemp);
		m_binDataTable.insert({ binData.bd_idx, binStr });
	}
}

BgMusicInfo * LevelPack::GetBgMusicInfo(int musicId)
{
	for (int i = 0; i < m_lpFormat.lpff_num_bg_sounds; i++) {
		if (m_lpFormat.lpff_bg_sounds[i].bgsnd_idx == musicId) {
			return &m_lpFormat.lpff_bg_sounds[i];
		}
	}

	return NULL;
}

LevelPack::LevelPack(LevelPackFileFormat _lpff, bool isCompatible)
{
	m_lpFormat = _lpff;

	CreateBinDataTable();
	CreateLevels();
}

string LevelPack::GetBinObjectAtIndex(int index)
{
	return m_binDataTable.at(index);
}

void LevelPack::CreateLevels()
{
	LevelMusic levelMusicInfo;

	for (int i = 0; i < m_lpFormat.lpff_num_levels; i++) {
		LevelInfo levelInfo = m_lpFormat.lpff_levels[i];
		
		levelMusicInfo.lm_id = levelInfo.li_bgmusic_idx;

		BgMusicInfo * bgMusicInfo = GetBgMusicInfo(levelInfo.li_bgmusic_idx);
		if (bgMusicInfo != NULL) {
			levelMusicInfo.lm_author = GetBinObjectAtIndex(bgMusicInfo->bgsnd_author_idx);
			levelMusicInfo.lm_album = GetBinObjectAtIndex(bgMusicInfo->bgsnd_album_idx);
			levelMusicInfo.lm_title = GetBinObjectAtIndex(bgMusicInfo->bgsnd_title_idx);
			levelMusicInfo.lm_data = bgMusicInfo->bgsnd_data;
			levelMusicInfo.lm_size = bgMusicInfo->bgsnd_len;
		}

		this->m_levels.push_back(Level(
			vector<LevelRoomInfo>(levelInfo.li_rooms, levelInfo.li_rooms + levelInfo.li_num_rooms),
			vector<LevelTargetInfo>(levelInfo.li_targets, levelInfo.li_targets + levelInfo.li_num_targets),
			vector<LevelSymbolInfo>(levelInfo.li_symbols, levelInfo.li_symbols + levelInfo.li_num_symbols),
			vector<LevelPortalInfo>(levelInfo.li_tp_portals, levelInfo.li_tp_portals + levelInfo.li_num_tp_portals),
			GetBinObjectAtIndex(levelInfo.li_info),
			levelMusicInfo
		));
	}
}

string LevelPack::GetName()
{
	return GetBinObjectAtIndex(m_lpFormat.lpff_level_pack_info.lpi_name_idx);
}

string LevelPack::GetAuthor()
{
	return GetBinObjectAtIndex(m_lpFormat.lpff_level_pack_info.lpi_author_idx);
}

string LevelPack::GetOriginalFile()
{
	return GetBinObjectAtIndex(m_lpFormat.lpff_level_pack_info.lpi_gen_file_idx);
}

string LevelPack::GetVersion()
{
	return std::to_string(m_lpFormat.lpff_level_pack_info.lpi_game_compatible);
}

string LevelPack::GetDescription()
{
	return GetBinObjectAtIndex(m_lpFormat.lpff_level_pack_info.lpi_desc_idx);
}

vector<Level> LevelPack::GetLevels()
{
	vector<Level> dummyReturn;

	return dummyReturn;
}
