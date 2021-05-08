/** \file BadThings.h
*	\author Benjamin Lueben
*
*	\brief Bad Things Functions for Munchkin Cards

	In this class all the BadThings functions for every Munchkin Card are defined.

*/

#pragma once
#include <functional>

struct BadThingsRetVal
{
	
};

// bad things function
class MunchkinCard;
class GameState;
/// Callback type for bad things behaviour
using BadThingsFunc = std::function<BadThingsRetVal(GameState&, const MunchkinCard&)>;

// bad things functions
/**
	\brief Namespace containing all bad things functions. These can be assigned to a specific card.
*/
namespace badthings
{
	/**
	\brief Does nothing (placeholder)
	
	\param gamestate	The game state is modified according to the corresponding munchkin rule/card. 
	\param card			Used to supply additional information about the card.
	*/
	BadThingsRetVal emptyBadThings(GameState& gamestate, const MunchkinCard& card);
	/**
	\brief Player looses 1 hand and the function checks whether or not he can still carry all his items that need hands.

	\param gamestate	The game state is modified according to the corresponding munchkin rule/card.
	\param card			Used to supply additional information about the card.
	*/
	BadThingsRetVal looseHand(GameState& gamestate, const MunchkinCard& card);
	/**
	\brief Player looses his class.

	\param gamestate	The game state is modified according to the corresponding munchkin rule/card.
	\param card			Used to supply additional information about the card.
	*/
	BadThingsRetVal looseClass(GameState& gamestate, const MunchkinCard& card);
	/**
	\brief Player dies.

	\param gamestate	The game state is modified according to the corresponding munchkin rule/card.
	\param card			Used to supply additional information about the card.
	*/
	BadThingsRetVal playerDies(GameState& gamestate, const MunchkinCard& card);
	/**
	\brief Player looses 1 level.

	\param gamestate	The game state is modified according to the corresponding munchkin rule/card.
	\param card			Used to supply additional information about the card.
	*/
	BadThingsRetVal looseLevel(GameState& gamestate, const MunchkinCard& card);
	/**
	\brief Male Players die. Female players loose 1 level.

	\param gamestate	The game state is modified according to the corresponding munchkin rule/card.
	\param card			Used to supply additional information about the card.
	*/
	BadThingsRetVal maleDeadFemaleLevelDown(GameState& gamestate, const MunchkinCard& card);
}