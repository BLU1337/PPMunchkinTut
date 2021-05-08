#include <BadThings.h>
#include <MunchkinCards.h>
#include <GameState.h>

BadThingsRetVal badthings::looseHand(GameState& gamestate, const MunchkinCard& card)
{
	std::cout << "AHHHHH MY HAAAANNND!!!" << std::endl;
	gamestate.player01.hands--;
	if (gamestate.player01.hands < 0)
	{
		std::cout << "You need to get rid of a item that requires at least 1 Hand!!" << endl;
	}
	return BadThingsRetVal();
}

BadThingsRetVal badthings::looseClass(GameState& gamestate, const MunchkinCard& card)
{
	gamestate.player01.availableClasses--;
	return BadThingsRetVal();
}

BadThingsRetVal badthings::emptyBadThings(GameState& gamestate, const MunchkinCard& card)
{
	return BadThingsRetVal();
}

BadThingsRetVal badthings::playerDies(GameState& gamestate, const MunchkinCard& card)
{
	return BadThingsRetVal();
}

BadThingsRetVal badthings::looseLevel(GameState& gamestate, const MunchkinCard& card)
{
	gamestate.player01.lvl--;
	return BadThingsRetVal();
}

BadThingsRetVal badthings::maleDeadFemaleLevelDown(GameState& gamestate, const MunchkinCard& card)
{
	if (gamestate.player01.sex == "male")
		badthings::playerDies(gamestate, card);
	else
		badthings::looseLevel(gamestate, card);

	return BadThingsRetVal();
}