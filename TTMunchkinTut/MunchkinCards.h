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
enum class ParentCardType
{
	door,
	treasure
};
enum class CardType
{
	curse,
	munchClass,
	joker,
	lvlUp,
	itemBuff,
	monster,
	race,
	item,
	removeCard

};
enum class ItemType
{
	armor,
	shoes,
	hat,
	boni,
	weapon,
	joker,
	clothing
};

// --- classes

class MunchkinCard
{
public:
	MunchkinCard() {}
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

	MunchkinCard(const MunchkinCard& other) = default;
	MunchkinCard(MunchkinCard&& other) = default;

	MunchkinCard& operator=(const MunchkinCard& other) = default;
	MunchkinCard& operator=(MunchkinCard&& other) = default;

	string cardName;
	string effect;
	vector<string> bonis;
	string badThings;
	string itemEffect;
	string itemNeeds;
	ParentCardType parentCardType;
	CardType type;			//1=curse, 2=MunchClass, 3=joker, 4=lvlUp, 5=itemBuff, 6=monster, 7=race, 8=item
	ItemType itemType;		//armor, shoes, hat, 
	int markerID;
	int strengthBoni;
	int debuff;
	int monsStrength;
	int lvlUp;
	int treasures;
	int itemValue;
	int handsNeeded;
	bool itemLarge;
	// card functions
	BadThingsFunc badThingsFunc;
	//ExtrasFunc	  extrasFunc;
	
	static vector<MunchkinCard> cardsConstr();
};

