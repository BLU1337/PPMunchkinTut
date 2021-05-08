/** \file CardTypeActions.h
*	\author Benjamin Lueben
*
*	\brief Class to define the functions for munchkin card types

	In this class all the functions for the different munchkin card types are defined which are used to implement the game logic of the munchkin cardgame.

*/

#pragma once
#include <functional>

struct CardTypeRetVal
{

};

// card type function
class MunchkinCard;
class GameState;
using CardTypeFunc = std::function<CardTypeRetVal(GameState&, const MunchkinCard&)>;

/**
	\brief Namespace containing all cardtype functions. These are mapped to the actual card type.
*/
namespace cardtypeaction
{
	/**
	\brief processes card type curse

	\param gamestate	The game state is modified according to the corresponding munchkin rule/card.
	\param card			Used to supply additional information about the card.
	*/
	CardTypeRetVal curse(GameState& gamestate, const MunchkinCard& card);
	/**
	\brief processes card type joker

	\param gamestate	The game state is modified according to the corresponding munchkin rule/card.
	\param card			Used to supply additional information about the card.
	*/
	CardTypeRetVal joker(GameState& gamestate, const MunchkinCard& card);
	/**
	\brief proceses card type monster

	\param gamestate	The game state is modified according to the corresponding munchkin rule/card.
	\param card			Used to supply additional information about the card.
	*/
    CardTypeRetVal monster(GameState& gamestate, const MunchkinCard& card);
	/**
	\brief processes card Type munchkin class

	\param gamestate	The game state is modified according to the corresponding munchkin rule/card.
	\param card			Used to supply additional information about the card.
	*/
	CardTypeRetVal munchClass(GameState& gamestate, const MunchkinCard& card);
	/**
	\brief processes card type munchkin race

	\param gamestate	The game state is modified according to the corresponding munchkin rule/card.
	\param card			Used to supply additional information about the card.
	*/
	CardTypeRetVal race(GameState& gamestate, const MunchkinCard& card);
	/**
	\brief processes card type item

	\param gamestate	The game state is modified according to the corresponding munchkin rule/card.
	\param card			Used to supply additional information about the card.
	*/
	CardTypeRetVal item(GameState& gamestate, const MunchkinCard& card);
	/**
	\brief processes card type item buff

	\param gamestate	The game state is modified according to the corresponding munchkin rule/card.
	\param card			Used to supply additional information about the card.
	*/
	CardTypeRetVal itemBuff(GameState& gamestate, const MunchkinCard& card);
	/**
	\brief processes card type level up

	\param gamestate	The game state is modified according to the corresponding munchkin rule/card.
	\param card			Used to supply additional information about the card.
	*/
	CardTypeRetVal lvlUp(GameState& gamestate, const MunchkinCard& card);
}