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

LevelPack::LevelPack(LevelList _levelList, string _name, string _author, GameVersion _gameVersion, string _description)
{
	this->m_levels = _levelList;
	this->m_name = _name;
	this->m_author = _author;
	this->m_version = _gameVersion;
	this->m_description = _description;
}

string LevelPack::GetName()
{
	return this->m_name;
}

string LevelPack::GetAuthor()
{
	return this->m_author;
}

GameVersion LevelPack::GetVersion()
{
	return this->m_version;
}

string LevelPack::GetDescription()
{
	return this->m_description;
}

LevelList LevelPack::GetLevels()
{
	return this->m_levels;
}
