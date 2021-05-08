/** \file MunchkinCards.h
*	\author Benjamin Lueben
*
*	\brief MunchkinCards class in which all parameters for the munchkin cards are stored

	In this class the munchkin Card object is defined with every parameter the card could have and with custom constructors which could be used to define the cards more easily

*/

#pragma once
#include <string>;
#include <vector>;
# include "opencv2/opencv.hpp"
# include "opencv2/imgcodecs.hpp"
# include "opencv2/imgproc.hpp"
#include <functional>
#include "GameState.h"
#include "BadThings.h"
//#include "Extras.h"

using namespace std;

// --- enums
/**
	\brief enum class for parent card types
*/
enum class ParentCardType
{
	door,		///< card type door
	treasure	///< card type treasure
};

/**
	\brief enum class for "child" card Types
*/
enum class CardType
{
	curse,			///< card type curse
	munchClass,		///< card type munchkin class
	joker,			///< card type joker
	lvlUp,			///< card type level up
	itemBuff,		///< card type item buff
	monster,		///< card type monster
	race,			///< card type munchkin race
	item,			///< card type item
	removeCard		///< param to signal removing a card

};

/**
	\brief enum class of item types
*/
enum class ItemType
{
	armor,		///< item type armor
	shoes,		///< item class shoes
	hat,		///< item class hat
	boni,		///< item class boni
	weapon,		///< item class weapon
	joker,		///< item class joker
	clothing	///< item class clothing
};

// --- classes

/**
	\brief class of munchkin cards with params that could be needed for a single card
*/
class MunchkinCard
{
public:
	/// Default constructor
	MunchkinCard() {}
	/**
		\brief custom constructor 
		\param _markerID marker id that corresponse with the card
		\param _cardName card name
		\param _effect card effect
		\param _badThings (deprecated) string of bad things
		\param _itemEffect effect that an item has e.g. attack with fire and flame
		\param _itemNeeds param that a munchkin needs to equip a item e.g. has to be a dwarf
		\param _bonis boni that the card can have
		\param _parentCardType param for parent card type
		\param _type param for "child" card type
		\param _itemType item type
		\param _strengthBoni how much strength the munchkin receives with this card
		\param _debuff how much strength the munchkin looses through this card
		\param _monsStrength param how much the monsters strength is  
		\param _lvlUp how many level you get r.g. when defeating the monster
		\param _treasures how many treasure the munchkin gets when defeating the monster  
		\param _itemValue how much gold the item sells for
		\param _handsNeeded	how many hands a munchkin needs to equip the item  
		\param _itemLarge wether or not the item is large

	*/
	MunchkinCard(
		int _markerID,
		const string& _cardName,
		const string& _effect,
		const string& _badThings,
		const string& _itemEffect,
		const string& _itemNeeds,
		vector<string> _bonis,
		ParentCardType _parentCardType,
		CardType _type,
		ItemType _itemType,
		int _strengthBoni,
		int _debuff,
		int _monsStrength,
		int _lvlUp,
		int _treasures,
		int _itemValue,
		int _handsNeeded,
		bool _itemLarge
	);
	/// default copy constructor
	MunchkinCard(const MunchkinCard& other) = default;
	/// default move constructor
	MunchkinCard(MunchkinCard&& other) = default;

	/// default copy assignment
	MunchkinCard& operator=(const MunchkinCard& other) = default;
	/// default move assignment
	MunchkinCard& operator=(MunchkinCard&& other) = default;

	string cardName;					///< card name
	string effect;						///< card effect
	vector<string> bonis;				///< boni that the card can have
	string badThings;					///< (deprecated) string of bad things
	string itemEffect;					///< effect that an item has e.g. attack with fire and flame
	string itemNeeds;					///< param that a munchkin needs to equip a item e.g. has to be a dwarf
	ParentCardType parentCardType;		///< param for parent card type
	CardType type;						///< param for "child" card type
	ItemType itemType;					///< item type
	int markerID;						///< marker id that corresponse with the card
	int strengthBoni;					///< how much strength the munchkin receives with this card
	int debuff;							///< how much strength the munchkin looses through this card
	int monsStrength;					///< param how much the monsters strength is
	int lvlUp;							///< how many level you get r.g. when defeating the monster
	int treasures;						///< how many treasure the munchkin gets when defeating the monster
	int itemValue;						///< how much gold the item sells for
	int handsNeeded;					///< how many hands a munchkin needs to equip the item
	bool itemLarge;						///< wether or not the item is large
	// card functions
	BadThingsFunc badThingsFunc;		///< bad things function that mapps onto a bad things function in badThingsFunc.h
	//ExtrasFunc	  extrasFunc;
	
	/// function to create all cards from MunchkinCards.cpp
	static vector<MunchkinCard> cardsConstr();
};

