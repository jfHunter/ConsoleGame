#include <string>
#include <vector>

#include "Level.h"

using std::vector;
using std::string;

LevelRoom::LevelRoom(LevelRoomInfo _roomInfo)
{
	this->m_id = _roomInfo.lr_room_idx;
	this->m_width = _roomInfo.lr_width;
	this->m_height = _roomInfo.lr_height;
	this->m_dataMap = _roomInfo.lr_data;
}

int LevelRoom::GetId()
{
	return this->m_id;
}

int LevelRoom::GetWidth()
{
	return this->m_width;
}

int LevelRoom::GetHeight()
{
	return this->m_height;
}

char* LevelRoom::GetMapData()
{
	return this->m_dataMap;
}



LevelPortal::LevelPortal(LevelPortalInfo _portalInfo)
{
	this->m_gateACoords = CoordEx(_portalInfo.lp_room_a_idx, _portalInfo.lp_portal_a_x, _portalInfo.lp_portal_a_y);
	this->m_gateBCoords = CoordEx(_portalInfo.lp_room_b_idx, _portalInfo.lp_portal_b_x, _portalInfo.lp_portal_b_y);
}

CoordEx LevelPortal::GetGateACoords()
{
	return this->m_gateACoords;
}

CoordEx LevelPortal::GetGateBCoords()
{
	return this->m_gateBCoords;
}



LevelTarget::LevelTarget(LevelTargetInfo _targetInfo)
{
	this->m_objectA = _targetInfo.obj_a;
	this->m_objectB = _targetInfo.obj_b;
	this->m_actionType = 0;
	this->m_targetType = 0;
}

int LevelTarget::GetTargetType()
{
	return this->m_targetType;
}

int LevelTarget::GetActionType()
{
	return this->m_actionType;
}

char LevelTarget::GetObjectA()
{
	return this->m_objectA;
}

char LevelTarget::GetObjectB()
{
	return this->m_objectB;
}


LevelSymbol::LevelSymbol(char _symChar, string _symDesc)
{
	this->m_symbolChar = _symChar;
	this->m_symbolDesc = _symDesc;
}

char LevelSymbol::GetCharacter()
{
	return this->m_symbolChar;
}

string LevelSymbol::GetDescription()
{
	return this->m_symbolDesc;
}


LevelMusic::LevelMusic(int _id, string _author, string _album, string _title, uint64_t _length, char* _data)
{
	this->m_id = _id;
	this->m_author = _author;
	this->m_album = _album;
	this->m_title = _title;
	this->m_length = _length;
	this->m_data = _data;
}

int LevelMusic::GetId()
{
	return this->m_id;
}

string LevelMusic::GetAuthor()
{
	return this->m_author;
}

string LevelMusic::GetAlbum()
{
	return this->m_album;
}

string LevelMusic::GetTitle()
{
	return this->m_title;
}

uint64_t LevelMusic::GetLength()
{
	return this->m_length;
}

char* LevelMusic::GetData()
{
	return this->m_data;
}

Level::Level(
	vector<LevelRoomInfo> _levRooms,
	vector<LevelTargetInfo> _levTargets,
	vector<LevelSymbolInfo> _levSymbols,
	vector<LevelPortalInfo> _levPortals,
	string _levInfo,
	LevelMusic _levMusic)
	:
	m_rooms(_levRooms),
	m_targets(_levTargets),
	m_symbols(_levSymbols),
	m_portals(_levPortals),
	m_levelInfo(_levInfo),
	m_levelMusic(_levMusic
) { }

LevelMusic Level::GetLevelMusic()
{
	return this->m_levelMusic;
}

string Level::GetLevelInfo()
{
	return this->m_levelInfo;
}

int Level::GetNumberOfRooms()
{
	return (int)this->m_rooms.size();
}

int Level::GetNumberOfTargets()
{
	return (int)this->m_targets.size();
}

int Level::GetNumberOfSymbols()
{
	return (int)this->m_symbols.size();
}

int Level::GetNumberOfPortals()
{
	return (int)this->m_portals.size();
}

LevelRoomInfo Level::GetLevelRoom(int index)
{
	LevelRoomInfo result = { 0 };

	for (size_t i = 0; i < this->m_rooms.size(); i++) {
		if (i == index) {
			return this->m_rooms.at(i);
		}
	}

	return result;
}

LevelTargetInfo Level::GetLevelTarget(int index)
{
	LevelTargetInfo result = { 0 };

	for (size_t i = 0; i < this->m_targets.size(); i++) {
		if (i == index) {
			return this->m_targets.at(i);
		}
	}

	return result;
}

LevelSymbolInfo Level::GetLevelSymbol(int index)
{
	LevelSymbolInfo result = { 0 };

	for (size_t i = 0; i < this->m_symbols.size(); i++) {
		if (i == index) {
			return this->m_symbols.at(i);
		}
	}

	return result;
}

LevelPortalInfo Level::GetLevelPortal(int index)
{
	LevelPortalInfo result = { 0 };

	for (size_t i = 0; i < this->m_portals.size(); i++) {
		if (i == index) {
			return this->m_portals.at(i);
		}
	}

	return result;
}
