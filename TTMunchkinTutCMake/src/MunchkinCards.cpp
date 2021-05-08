#include "MunchkinCards.h"

using namespace std;



MunchkinCard::MunchkinCard(
		int _markerID,
		const string & _cardName,
		const string & _effect,
		const string & _badThings,
		const string & _itemEffect,
		const string & _itemNeeds,
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
	) :
		markerID(_markerID),
		cardName(_cardName),
		effect(_effect),
		bonis(_bonis),
		badThings(_badThings),
		itemType(_itemType),
		itemEffect(_itemEffect),
		itemNeeds(_itemNeeds),
		parentCardType(_parentCardType),
		type(_type),
		strengthBoni(_strengthBoni),
		debuff(_debuff),
		monsStrength(_monsStrength),
		lvlUp(_lvlUp),
		treasures(_treasures),
		itemValue(_itemValue),
		handsNeeded(_handsNeeded),
		itemLarge(_itemLarge)
{
}

vector<MunchkinCard> MunchkinCard::cardsConstr() {
	vector<MunchkinCard> cards;

	MunchkinCard card0;

	card0.markerID = 0;
	card0.cardName = "FenrisWolf";
	card0.type = CardType::monster;
	card0.parentCardType = ParentCardType::door;
	card0.monsStrength = 16;
	card0.badThings = "hands--";
	card0.lvlUp = 2;
	card0.treasures = 4;
	card0.badThingsFunc = badthings::looseHand;

	//cards[0].cardName = "FenrisWolf";
	//cards[0].type = CardType::monster;
	//cards[0].monsStrength = 16;
	//cards[0].badThings = "hands--";
	//cards[0].lvlUp = 2;
	//cards[0].treasures = 4;

	MunchkinCard card1;
	card1.markerID = 1;
	card1.cardName = "J.R.R. Trollkin";
	card1.type = CardType::monster;
	card1.parentCardType = ParentCardType::door;
	card1.effect = "+10 vs epic Munchkins";
	card1.monsStrength = 7;
	card1.badThings = "read all following monster cards out lout until you kill a monster without help of others";
	card1.lvlUp = 1;
	card1.treasures = 2;
	card1.badThingsFunc = badthings::emptyBadThings;

	MunchkinCard card2;
	card2.markerID = 2;
	card2.cardName = "Morgan Le Fay";
	card2.type = CardType::monster;
	card2.parentCardType = ParentCardType::door;
	card2.effect = "-4 vs Gladiators but +5 vs male Munchkins";
	card2.monsStrength = 17;
	card2.badThings = "male Munchkins die! Female Munchkins lvl--";
	card2.lvlUp = 2;
	card2.treasures = 4;
	card2.badThingsFunc = badthings::maleDeadFemaleLevelDown;

	MunchkinCard card3;
	card3.markerID = 3;
	card3.cardName = "";

	MunchkinCard card4;
	card4.markerID = 4;
	card4.cardName = "";

	MunchkinCard card5;
	card5.markerID = 5;
	card5.cardName = "";

	MunchkinCard card6;
	card6.markerID = 6;
	card6.cardName = "Krieger";
	card6.type = CardType::munchClass;
	card6.parentCardType = ParentCardType::door;
	card6.bonis.push_back("Lege im Kampf bis zu 3 Karten ab f�r jeweils +1;");
	card6.bonis.push_back("Bei Gleichstand im Kampf gewinnst du;");

	MunchkinCard card7;
	card7.markerID = 7;
	card7.cardName = "Schummeln!";
	card7.type = CardType::joker;
	card7.parentCardType = ParentCardType::door;
	card7.effect = "Lege Karte an Gegenstand du darfst diesen ausr�sten egal was die Vorraussetzungen sind";

	MunchkinCard card8;
	card8.markerID = 8;
	card8.cardName = "Zwerg";
	card8.type = CardType::race;
	card8.parentCardType = ParentCardType::door;
	card8.bonis.push_back("beliebige Anzahl gro�er Gegenst�nde;");
	card8.bonis.push_back("6 Karten auf Hand erlaubt;");

	MunchkinCard card9;
	card9.markerID = 9;
	card9.cardName = "Wunschring";
	card9.parentCardType = ParentCardType::treasure;
	card9.type = CardType::item;
	card9.itemType = ItemType::joker;
	card9.itemEffect = "beende einen Fluch";
	card9.itemValue = 500;

	MunchkinCard card10;
	card10.markerID = 10;
	card10.cardName = "Pfeil und Bogen des Herkules";
	card10.parentCardType = ParentCardType::treasure;
	card10.type = CardType::item;
	card10.itemType = ItemType::weapon;
	card10.bonis.push_back("Angriffe z\x84hlen als Angriff mit Feuer und Flamme");
	card10.strengthBoni = 4;
	card10.handsNeeded = 2;
	card10.itemValue = 700;

	MunchkinCard card11;
	card11.markerID = 11;
	card11.cardName = "Magisches Fagott";
	card11.parentCardType = ParentCardType::treasure;
	card11.type = CardType::item;
	card11.itemType = ItemType::weapon;
	card11.strengthBoni = 4;
	card11.itemLarge = true;
	card11.handsNeeded = 2;
	card11.itemValue = 600;

	MunchkinCard card12;
	card12.markerID = 12;
	card12.cardName = "Bermudashorts der Unverwundbarkeit";
	card12.parentCardType = ParentCardType::treasure;
	card12.type = CardType::item;
	card12.itemType = ItemType::clothing;
	card12.itemNeeds = "Nicht von Kriegern nutzbar";
	card12.strengthBoni = 4;
	card12.itemValue = 400;

	MunchkinCard card13;
	card13.markerID = 13;
	card13.cardName = "von Gott gegeben";
	card13.parentCardType = ParentCardType::treasure;
	card13.type = CardType::itemBuff;
	card13.strengthBoni = 3;

	MunchkinCard card14;
	card14.markerID = 14;
	card14.cardName = "Ueberlebe Ragnar\x94k";
	card14.parentCardType = ParentCardType::treasure;
	card14.type = CardType::lvlUp;
	card14.lvlUp = 1;

	MunchkinCard card15;
	card15.markerID = 15;
	card15.cardName = "FLUCH! Die Laune des Schicksals";
	card15.parentCardType = ParentCardType::door;
	card15.type = CardType::curse;
	card15.effect = "Verliere deine Klasse";
	card15.badThingsFunc = badthings::looseClass;

	cards.push_back(std::move(card0));
	cards.push_back(std::move(card1));
	cards.push_back(std::move(card2));
	cards.push_back(std::move(card3));
	cards.push_back(std::move(card4));
	cards.push_back(std::move(card5));
	cards.push_back(std::move(card6));
	cards.push_back(std::move(card7));
	cards.push_back(std::move(card8));
	cards.push_back(std::move(card9));
	cards.push_back(std::move(card10));
	cards.push_back(std::move(card11));
	cards.push_back(std::move(card12));
	cards.push_back(std::move(card13));
	cards.push_back(std::move(card14));
	cards.push_back(std::move(card15));

	return cards;

}