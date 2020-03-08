#pragma once

#include <string>
#include <vector>
#include <map>

#include "Level.h"
#include "LevelPackFileFormat.h"

using std::string;
using std::vector;
using std::map;

class LevelPack;

typedef vector<LevelPack> LevelPackList;

class LevelPack
{
private:
	LevelList    m_levels;
	string       m_name;
	string       m_author;
	GameVersion  m_version;
	string       m_description;

public:
	LevelPack();
	LevelPack(LevelList _levelList, string _name, string _author, GameVersion _gameVersion, string _description);
	string GetName();
	string GetAuthor();
	GameVersion GetVersion();
	string GetDescription();
	vector<Level> GetLevels();
};

#ifdef _interface_

void example()
{
	LevelPack levelPack;

	string levelPack.GetName();
	string levelPack.GetAuthor();
	GameVersion levelPack.GetVersion();
	string levelPack.GetDescription();

	vector<Level> levelPack.GetLevels();

	Level level;
	{
		string level.GetInfo();
		vector<LevelRoom> level.GetRooms();
		vector<LevelPortal> level.GetPortals();
		vector<LevelTarget> level.GetTargets();
		vector<LevelSymbol> level.GetSymbols();
	}

	LevelRoom
	{
		LevelRoom levelRoom;
		int levelRoom.GetId();
		int levelRoom.GetWidth();
		int levelRoom.GetHeight();
		char * levelRoom.GetMapData();
	}

	LevelPortal
	{
		LevelPortal levelPortal;
		CoordEx levelPortal.GetGateA();
		CoordEx levelPortal.GetGateB();
	}

	CoordEx
	{
		CoordEx coordEx;
		coordEx.GetRoomId();
		coordEx.GetX();
		coordEx.GetY();
	}

	LevelTarget
	{
		LevelTarget levelTarget;
		int levelTarget.GetType(); // TARGET_TYPE_FAIL, TARGET_TYPE_PASS, TARGET_TYPE_ADDITIONAL
		int levelTarget.GetAction(); // TARGET_ACTION_COLLISION, TARGET_ACTION_COLLECT
		bool levelTarget.IsPrimmary(); // must be true for Player to Exit
		char levelTarget.GetObjectA();
		char levelTarget.GetObjectB();
	}

	LevelSymbol
	{
		LevelSymbol levelSymbol;
		char levelSymbol.GetChar();
		string levelSymbol.GetDescription();
		ObjectType levelSymbol.GetObjectType();

		// P0:<0:1,2> - player
		// K0<0:1,2> - key "red"
		// K1<1:13,34> - key "green"
		// P1<> - fake player
	}
}

#endif