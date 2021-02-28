#pragma once
#include <functional>

struct CardTypeRetVal
{

};

// card type function
class MunchkinCard;
class GameState;
using CardTypeFunc = std::function<CardTypeRetVal(GameState&, const MunchkinCard&)>;

namespace cardtypeaction
{
	CardTypeRetVal curse(GameState& gamestate, const MunchkinCard& card);
	CardTypeRetVal joker(GameState& gamestate, const MunchkinCard& card);
    CardTypeRetVal monster(GameState& gamestate, const MunchkinCard& card);
	CardTypeRetVal munchClass(GameState& gamestate, const MunchkinCard& card);
	CardTypeRetVal race(GameState& gamestate, const MunchkinCard& card);
	CardTypeRetVal item(GameState& gamestate, const MunchkinCard& card);
	CardTypeRetVal itemBuff(GameState& gamestate, const MunchkinCard& card);
	CardTypeRetVal lvlUp(GameState& gamestate, const MunchkinCard& card);
}