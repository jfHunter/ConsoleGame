#include <string>
#include <vector>

#include "LevelPackFileFormat.h"

#include "Utils.h"
#include "StringUtils.hpp"

LevelGameCompatible ParseGameCompatible(std::string compatible)
{
	if (StringUtils::Equals(compatible, "1.0")) {
		return LEVEL_GAME_COMPATIBLE_10;
	}

	return LEVEL_GAME_COMPATIBLE_UNKNOWN;
}

LevelTargetType ParseLevelTargetType(std::string targetType)
{
	if (StringUtils::Equals(targetType, "->")) {
		return LEVEL_TARGET_TYPE_COLLISION;
	}

	return LEVEL_TARGET_TYPE_UNKNOWN;
}

std::string ParseSymbolDescription(std::string symbol)
{
	StringVect symbolLineSplit = StringUtils::Split(symbol, " - ");
	if (symbolLineSplit.size() == 2)
	{
		return symbolLineSplit.at(1);
	}

	return "<empty>";
}

char ParseSymbolCharacter(std::string symbol)
{
	StringVect symbolLineSplit = StringUtils::Split(symbol, " - ");
	if (symbolLineSplit.size() == 2)
	{
		if (symbolLineSplit.at(0) != "") {
			return symbolLineSplit.at(0)[0];
		}
	}
	return '\0';
}

LevelTargetInfo ParseLevelTarget(std::string target)
{
	LevelTargetInfo levTarget = { 0 };
	StringVect targetSplit = StringUtils::Split(target, " ");
	std::string objA, objB, operation;

	if (targetSplit.size() == 3) {
		objA = targetSplit.at(0);
		operation = targetSplit.at(1);
		objB = targetSplit.at(2);

		if ((objA.length() > 0) && (objB.length() > 0) && (operation.length() > 0)) {
			levTarget.obj_a = objA[0];
			levTarget.obj_b = objB[0];
			levTarget.lt_target_type = ParseLevelTargetType(String(operation));
		}
	}

	return levTarget;
}
