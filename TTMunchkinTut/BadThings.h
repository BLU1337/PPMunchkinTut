#pragma once
#include <functional>

struct BadThingsRetVal
{
	
};

// bad things function
class MunchkinCard;
class GameState;
using BadThingsFunc = std::function<BadThingsRetVal(GameState&, const MunchkinCard&)>;

// bad things functions
namespace badthings
{
	BadThingsRetVal emptyBadThings(GameState& gamestate, const MunchkinCard& card);
	BadThingsRetVal looseHand(GameState& gamestate, const MunchkinCard& card);
	BadThingsRetVal looseClass(GameState& gamestate, const MunchkinCard& card);
	BadThingsRetVal playerDies(GameState& gamestate, const MunchkinCard& card);
	BadThingsRetVal looseLevel(GameState& gamestate, const MunchkinCard& card);
	BadThingsRetVal maleDeadFemaleLevelDown(GameState& gamestate, const MunchkinCard& card);
}