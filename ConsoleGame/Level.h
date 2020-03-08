#pragma once
#include <string>
#include <vector>

#include "LevelPackFileFormat.h"

#include "GameCommon.h"

using std::vector;
using std::string;

class Level;
class LevelRoom;
class LevelPortal;
class LevelTarget;
class LevelSymbol;

typedef vector<Level> LevelList;
typedef vector<LevelRoom> RoomList;
typedef vector<LevelPortal> PortalList;
typedef vector<LevelTarget> TargetList;
typedef vector<LevelSymbol> SymbolList;

class LevelRoom
{
private:
	int    m_id;
	int    m_width;
	int    m_height;
	char * m_dataMap;

public:
	LevelRoom(LevelRoomInfo _roomInfo);

	int GetId();
	int GetWidth();
	int GetHeight();
	char * GetMapData();
};

class LevelPortal
{
private:
	CoordEx m_gateACoords;
	CoordEx m_gateBCoords;

public:
	LevelPortal(LevelPortalInfo _portalInfo);

	CoordEx GetGateACoords();
	CoordEx GetGateBCoords();
};

class LevelTarget
{
private:
	int   m_targetType; /* FAIL/PASS/ADDITIONAL */
	int   m_actionType; /* COLLISION/COLLECT */
	char  m_objectA;
	char  m_objectB;

public:
	LevelTarget(LevelTargetInfo _targetInfo);

	int GetTargetType();
	int GetActionType();
	char GetObjectA();
	char GetObjectB();
};

class LevelSymbol
{
private:
	char m_symbolChar;
	string m_symbolDesc;

public:
	LevelSymbol(char _symChar, string _symDesc);
	char GetCharacter();
	string GetDescription();
};

class LevelMusic
{
private:
	string    m_author;
	string    m_album;
	string    m_title;
	char *    m_data;
	uint64_t  m_length;

public:
	LevelMusic(string _author, string _album, string _title, uint64_t _length, char * _data);

	string GetAuthor();
	string GetAlbum();
	string GetTitle();
	uint64_t GetLength();
	char * GetData();
};

class Level
{
private:
	RoomList    m_rooms;
	TargetList  m_targets;
	SymbolList  m_symbols;
	PortalList  m_portals;
	LevelMusic  m_levelMusic;
	string      m_levelInfo;

public:
	Level(RoomList _levRooms, TargetList _levTargets, SymbolList _levSymbols, PortalList _levPortals, string _levInfo, LevelMusic _levMusic);
	const RoomList& GetRooms();
	const TargetList& GetTargets();
	const SymbolList& GetSymbols();
	const PortalList& GetPortals();

	LevelMusic GetMusic();
	string GetLevelInfo();
};
