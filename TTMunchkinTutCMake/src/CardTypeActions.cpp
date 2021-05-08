#include "CardTypeActions.h"
#include <iostream>
#include "GameState.h"
#include "MunchkinCards.h"
using namespace std;

#pragma region aux
bool runAway(GameState& gamestate)
{
	int diceRoll = rand() % 6 + 1;
	if (diceRoll > gamestate.player01.runStrength)
		return true;
	else
		return false;
}
#pragma endregion

#pragma region card_type_function_defs

CardTypeRetVal cardtypeaction::curse(GameState & gamestate, const MunchkinCard & card)
{
	gamestate.mouseparams.tutText = { "FLUCH! ", "Du wirst von dem Fluch ", card.cardName, "getroffen! Dir passiert:", card.effect, "Wenn du einen Wunschring hast," , "kannst du ihn einsetzen", "und den Fluch verhindern!", "Um fortzufahren druecke den", "gruenen Button!" };
	card.badThingsFunc(gamestate, card);

	gamestate.end_turn = true;
	return CardTypeRetVal();
}

CardTypeRetVal cardtypeaction::joker(GameState & gamestate, const MunchkinCard & card)
{
	std::cout << "TODO: implement function call for effects: " << card.effect << endl;

	gamestate.end_turn = true;
	return CardTypeRetVal();
}

CardTypeRetVal cardtypeaction::monster(GameState & gamestate, const MunchkinCard & card)
{
	int monsStrength = card.monsStrength;
	int playerStrength = gamestate.player01.lvl + gamestate.player01.strength;
	// TODO: monsStrength +- monster effects functionCall/code in string?
	if (gamestate.should_continue != true)
	{
		gamestate.mouseparams.tutText = { "Hinter der Tuer erwartet ", "dich ein/e", card.cardName, "versuche es zu bekaempfen!", "Druecke den gruenen Button!" };
		gamestate.buttons.at(1).visible = true;
		return CardTypeRetVal();
	}
	else
	{
		if (monsStrength > playerStrength /* && monsStrength > playerStrength + bonis*/)
		{
			if (!gamestate.run_away)
			{
				gamestate.mouseparams.tutText = { "Du bist zu schwach!", "Drücke den grünen Button", "und versuche wegzulaufen!" };
				gamestate.buttons.at(1).visible = true;
				gamestate.run_away = true;
				return CardTypeRetVal();
			}
			else
			{
				if (runAway(gamestate))
				{
					gamestate.mouseparams.tutText = { "Du bist weggelaufen!", "Um fortzufahren druecke den", "gruenen Button!" };
					gamestate.should_continue = false;
					gamestate.run_away = false;
					gamestate.end_turn = true;
					return CardTypeRetVal();
				}
				else
				{
					gamestate.mouseparams.tutText = { "Weglaufen Fehlgeschlagen!", "Dich erwarten schlimme Dinge!", card.badThings, "Um fortzufahren druecke den", "gruenen Button!" };
					card.badThingsFunc(gamestate, card);
					gamestate.should_continue = false;
					gamestate.run_away = false;
					gamestate.end_turn = true;
					return CardTypeRetVal();
				}

			}
		}
		else
		{
			gamestate.player01.lvl += card.lvlUp;
			gamestate.should_continue = false;
			gamestate.run_away = false;
			gamestate.mouseparams.tutText = {"Du hast ", card.cardName, "besiegt! Neues Level: " , std::to_string(gamestate.player01.lvl), "Du erhältst Schätze: ", std::to_string(card.treasures), "Um fortzufahren druecke den", "gruenen Button!" };
			if (gamestate.player01.lvl >= 10)
				gamestate.mouseparams.tutText = {"HERZLICHEN GLUECKWUNSCH!!!", "Du hast deine erste Partie", "Munchkins gewonnen!!!", "Freue dich darueber", "solange du noch kannst!", "Und starte dann eine neue Partie!"};

			gamestate.end_turn = true;
			return CardTypeRetVal();

		}

	}
}

CardTypeRetVal cardtypeaction::munchClass(GameState & gamestate, const MunchkinCard & card)
{
	if (gamestate.remove_card == true)
	{
		if (!card.bonis.empty())
		{
			for (int i = 0; i < card.bonis.size(); i++)
			{
				auto itr = std::find(gamestate.player01.bonis.begin(), gamestate.player01.bonis.end(), card.bonis[i]);
				if (itr != gamestate.player01.bonis.end()) gamestate.player01.bonis.erase(itr);
			}
		}
		for (int i = 0; i < gamestate.player01.munchClasses.size(); i++)
		{
			auto itr = std::find(gamestate.player01.munchClasses.begin(), gamestate.player01.munchClasses.end(), card.cardName);
			if (itr != gamestate.player01.munchClasses.end()) gamestate.player01.munchClasses.erase(itr);
		}
		gamestate.player01.availableClasses++;
		gamestate.mouseparams.tutText = { "Du hast eine Klasse abgelegt!", "Um fortzufahren druecke den", "gruenen Button!" };
		gamestate.remove_card = false;
	}
	else
	{
		if (gamestate.player01.availableClasses > 0)
		{
			gamestate.mouseparams.tutText = { "Du erhältst eine neue Klasse!", "Du bist nun ein/e", card.cardName, "Um fortzufahren druecke den", "gruenen Button!" };
			gamestate.player01.munchClasses.push_back(card.cardName);
			gamestate.player01.availableClasses--;
			for (int i = 0; i < card.bonis.size(); i++)
				gamestate.player01.bonis.push_back(card.bonis.at(i));

			gamestate.end_turn = true;
		}
		else
			gamestate.mouseparams.tutText = { "Du hast zu viele Klassen!", "Du kannst die alte Klasse", "ablegen und gegen die", "neue Klasse wechseln!", "Ansonsten nimm die Karte", "auf deine Hand!" , "Um fortzufahren druecke den", "gruenen Button!" };
	}
	gamestate.end_turn = true;
	return CardTypeRetVal();
}

CardTypeRetVal cardtypeaction::race(GameState & gamestate, const MunchkinCard & card)
{
	if (gamestate.remove_card == true)
	{
		if (!card.bonis.empty())
		{
			for (int i = 0; i < card.bonis.size(); i++)
			{
				auto itr = std::find(gamestate.player01.bonis.begin(), gamestate.player01.bonis.end(), card.bonis[i]);
				if (itr != gamestate.player01.bonis.end()) gamestate.player01.bonis.erase(itr);
			}
		}
		for (int i = 0; i < gamestate.player01.munchRaces.size(); i++)
		{
			auto itr = std::find(gamestate.player01.munchRaces.begin(), gamestate.player01.munchRaces.end(), card.cardName);
			if (itr != gamestate.player01.munchRaces.end()) gamestate.player01.munchRaces.erase(itr);
		}
		gamestate.player01.availableRaces++;
		gamestate.mouseparams.tutText = { "Du hast eine Rasse abgelegt!", "Um fortzufahren druecke den", "gruenen Button!" };
		gamestate.remove_card = false;
	}
	else
	{
		if (gamestate.player01.availableRaces > 0)
		{
			gamestate.mouseparams.tutText = { "Du erhaeltst eine neue Rasse!", "Du bist nun ein/e", card.cardName, "Um fortzufahren druecke den","gruenen Button!" };
			gamestate.player01.munchRaces.push_back(card.cardName);
			gamestate.player01.availableRaces--;
			for (int i = 0; i < card.bonis.size(); i++)
				gamestate.player01.bonis.push_back(card.bonis.at(i));

		}
		else
			gamestate.mouseparams.tutText = { "Du hast zu viele Rassen!", "Du kannst die alte Rasse", "ablegen und gegen die", "neue Klasse wechseln!", "Ansonsten nimm die Karte", "auf deine Hand!", "Um fortzufahren druecke den", "gruenen Button!" };
	}

	gamestate.end_turn = true;
	return CardTypeRetVal();
}

CardTypeRetVal cardtypeaction::item(GameState & gamestate, const MunchkinCard & card)
{
	switch (card.itemType)
	{
		//Schummeln?
	case(ItemType::joker):
		gamestate.player01.itemEffects.push_back(card.itemEffect);
		break;
	case(ItemType::boni):
		if (gamestate.remove_card == true)
		{
			for (int i = 0; i < card.bonis.size(); i++)
			{
				auto itr = std::find(gamestate.player01.bonis.begin(), gamestate.player01.bonis.end(), card.bonis[i]);
				if (itr != gamestate.player01.bonis.end()) gamestate.player01.bonis.erase(itr);
			}
			gamestate.mouseparams.tutText = { "Du hast einen Boni abgelegt!", "Um fortzufahren druecke den", "gruenen Button!" };
			gamestate.remove_card = false;
		}
		else
		{
			for (int i = 0; i < card.bonis.size(); i++)
				gamestate.player01.bonis.push_back(card.bonis[i]);


			gamestate.mouseparams.tutText = { "Du hast einen neuen Boni erhalten!", "Um fortzufahren druecke den", "gruenen Button!" };
		}
		break;
	case(ItemType::hat):
		if (gamestate.remove_card == true)
		{
			if (!card.bonis.empty())
			{
				for (int i = 0; i < card.bonis.size(); i++)
				{
					auto itr = std::find(gamestate.player01.bonis.begin(), gamestate.player01.bonis.end(), card.bonis[i]);
					if (itr != gamestate.player01.bonis.end()) gamestate.player01.bonis.erase(itr);
				}
			}
			gamestate.player01.strength -= card.strengthBoni;
			gamestate.player01.hasHat = false;
			gamestate.mouseparams.tutText = { "Du hast einen Hut abgelegt!", "Du verlierst bonus Staerke:", std::to_string(card.strengthBoni), "Um fortzufahren druecke den", "gruenen Button!" };
			gamestate.remove_card = false;
		}
		else
		{
			// TODO: Check for itemNeeds on card.itemNeeds
			if (gamestate.player01.hasHat)
			{
				if (!card.bonis.empty())
				{
					for (int i = 0; i < card.bonis.size(); i++)
						gamestate.player01.bonis.push_back(card.bonis[i]);
				}
				gamestate.player01.strength += card.strengthBoni;
				gamestate.player01.hasHat = true;
				gamestate.mouseparams.tutText = { "Du hast einen neuen Hut angelegt!", "Er gibt dir bonus Staerke:", std::to_string(card.strengthBoni), "Um fortzufahren druecke den", "gruenen Button!" };
			}
			else
				gamestate.mouseparams.tutText = { "Du kannst nur einen Hut tragen!", "Nimm die Karte auf die Hand!", "Um fortzufahren druecke den", "gruenen Button!" };
		}
		break;
	case(ItemType::armor):
		if (gamestate.remove_card == true)
		{
			if (!card.bonis.empty())
			{
				for (int i = 0; i < card.bonis.size(); i++)
				{
					auto itr = std::find(gamestate.player01.bonis.begin(), gamestate.player01.bonis.end(), card.bonis[i]);
					if (itr != gamestate.player01.bonis.end()) gamestate.player01.bonis.erase(itr);
				}
			}
			gamestate.player01.strength -= card.strengthBoni;
			gamestate.player01.hasArmor = false;
			gamestate.mouseparams.tutText = { "Du hast eine Rüstung abgelegt!", "Du verlierst bonus Staerke:", std::to_string(card.strengthBoni), "Um fortzufahren druecke den", "gruenen Button!" };
			gamestate.remove_card = false;
		}
		else
		{
			// TODO: Check for itemNeeds on card.itemNeeds
			if (gamestate.player01.hasArmor)
			{
				if (!card.bonis.empty())
				{
					for (int i = 0; i < card.bonis.size(); i++)
						gamestate.player01.bonis.push_back(card.bonis[i]);
				}
				gamestate.player01.strength += card.strengthBoni;
				gamestate.player01.hasArmor = true;
				gamestate.mouseparams.tutText = { "Du hast eine neue Ruestung angelegt!", "Sie gibt dir bonus Staerke:", std::to_string(card.strengthBoni), "Um fortzufahren druecke den", "gruenen Button!" };
			}
			else
				gamestate.mouseparams.tutText = { "Du kannst nur eine Ruestung tragen!", "Nimm die Karte auf deine Hand!", "Um fortzufahren druecke den", "gruenen Button!" };
		}
		break;
	case(ItemType::shoes):
		if (gamestate.remove_card == true)
		{
			if (!card.bonis.empty())
			{
				for (int i = 0; i < card.bonis.size(); i++)
				{
					auto itr = std::find(gamestate.player01.bonis.begin(), gamestate.player01.bonis.end(), card.bonis[i]);
					if (itr != gamestate.player01.bonis.end()) gamestate.player01.bonis.erase(itr);
				}
			}
			gamestate.player01.strength -= card.strengthBoni;
			gamestate.player01.hasShoes = false;
			gamestate.mouseparams.tutText = { "Du hast deine Schuhe abgelegt!", "Du verlierst bonus Staerke:", std::to_string(card.strengthBoni), "Um fortzufahren druecke den", "gruenen Button!" };
			gamestate.remove_card = false;
		}
		else
		{
			// TODO: Check for itemNeeds on card.itemNeeds
			if (gamestate.player01.hasShoes)
			{
				if (!card.bonis.empty())
				{
					for (int i = 0; i < card.bonis.size(); i++)
						gamestate.player01.bonis.push_back(card.bonis[i]);
				}
				gamestate.player01.strength += card.strengthBoni;
				gamestate.player01.hasShoes = true;
				gamestate.mouseparams.tutText = { "Du hast neue Schuhe angelegt!", "Sie geben dir bonus Staerke:", std::to_string(card.strengthBoni), "Um fortzufahren druecke den", "gruenen Button!" };
			}
			else
				gamestate.mouseparams.tutText = { "Du kannst nur ein Paar Schuhe tragen!", "Nimm die Karte auf deine Hand!", "Um fortzufahren druecke den", "gruenen Button!" };
		}
		break;
	case(ItemType::clothing):
		if (gamestate.remove_card == true)
		{
			if (!card.bonis.empty())
			{
				for (int i = 0; i < card.bonis.size(); i++)
				{
					auto itr = std::find(gamestate.player01.bonis.begin(), gamestate.player01.bonis.end(), card.bonis[i]);
					if (itr != gamestate.player01.bonis.end()) gamestate.player01.bonis.erase(itr);
				}
			}
			gamestate.player01.strength -= card.strengthBoni;
			gamestate.mouseparams.tutText = { "Du hast Kleidung abgelegt!", "Du verlierst bonus Staerke:", std::to_string(card.strengthBoni), "Um fortzufahren druecke den", "gruenen Button!" };
			gamestate.remove_card = false;
		}
		else
		{
			// TODO: Check for itemNeeds on card.itemNeeds
			if (!card.bonis.empty())
			{
				for (int i = 0; i < card.bonis.size(); i++)
					gamestate.player01.bonis.push_back(card.bonis[i]);
			}
			gamestate.player01.strength += card.strengthBoni;
			gamestate.mouseparams.tutText = { "Du hast neue Kleidung angelegt!", "Sie gibt dir bonus Staerke:", std::to_string(card.strengthBoni), "Um fortzufahren druecke den", "gruenen Button!" };
		}
		break;
	case(ItemType::weapon):
		if (gamestate.remove_card == true)
		{
			if (!card.bonis.empty())
			{
				for (int i = 0; i < card.bonis.size(); i++)
				{
					auto itr = std::find(gamestate.player01.bonis.begin(), gamestate.player01.bonis.end(), card.bonis[i]);
					if (itr != gamestate.player01.bonis.end()) gamestate.player01.bonis.erase(itr);
				}
			}
			gamestate.player01.strength -= card.strengthBoni;
			gamestate.player01.hands += card.handsNeeded;
			if (card.itemLarge == true) gamestate.player01.carriesLargeItems = false;
			gamestate.mouseparams.tutText = { "Du hast eine Waffe abgelegt!", "Du verlierst bonus Staerke:", std::to_string(card.strengthBoni), "Um fortzufahren druecke den", "gruenen Button!" };
			gamestate.remove_card = false;
		}
		else
		{
			//TODO: Check for itemNeeds on card.itemNeeds
			if (gamestate.player01.hands >= card.handsNeeded)
			{
				if (gamestate.player01.carriesLargeItems == false || card.itemLarge == false)
				{
					if (!card.bonis.empty())
					{
						for (int i = 0; i < card.bonis.size(); i++)
							gamestate.player01.bonis.push_back(card.bonis[i]);
					}
					gamestate.player01.strength += card.strengthBoni;
					gamestate.player01.hands -= card.handsNeeded;
					gamestate.player01.carriesLargeItems = card.itemLarge ? true : false;
					gamestate.mouseparams.tutText = { "Du hast eine neue Waffe angelegt!", "Sie gibt dir bonus Staerke:", std::to_string(card.strengthBoni), "Um fortzufahren druecke den", "gruenen Button!" };

				}
				else
					gamestate.mouseparams.tutText = { "Du kannst nur einen grossen Gegenstand tragen!", "Nimm die Karte auf deine Hand!", "Um fortzufahren druecke den", "gruenen Button!" };
			}
			else
				gamestate.mouseparams.tutText = { "Du hast nicht genug Hände!", "Nimm die Karte auf deine Hand!", "Um fortzufahren druecke den", "gruenen Button!" };
		}

		break;
	}
	gamestate.end_turn = true;
	return CardTypeRetVal();
}

CardTypeRetVal cardtypeaction::itemBuff(GameState & gamestate, const MunchkinCard & card)
{
	if (gamestate.remove_card == true)
	{
		if (!card.bonis.empty())
		{
			for (int i = 0; i < card.bonis.size(); i++)
			{
				auto itr = std::find(gamestate.player01.bonis.begin(), gamestate.player01.bonis.end(), card.bonis[i]);
				if (itr != gamestate.player01.bonis.end()) gamestate.player01.bonis.erase(itr);
			}
		}
		gamestate.player01.strength -= card.strengthBoni;
		gamestate.mouseparams.tutText = { "Du hast einen Gegenstandverstaerker abgelegt!", "Du verlierst bonus Staerke:", std::to_string(card.strengthBoni), "Um fortzufahren druecke den", "gruenen Button!" };
		gamestate.remove_card = false;
	}
	else
	{
		//TODO: Verweis auf Ausrüstung?
		if (!card.bonis.empty())
		{
			for (int i = 0; i < card.bonis.size(); i++)
				gamestate.player01.bonis.push_back(card.bonis[i]);
		}
		gamestate.player01.strength += card.strengthBoni;
		gamestate.mouseparams.tutText = { "Du hast eine Gegenstandsverstaerkung gefunden!", "Lege sie an einen Gegenstand den", "du ausgeruestet hast an!", "Um fortzufahren druecke den", "gruenen Button!" };
	}

	gamestate.end_turn = true;
	return CardTypeRetVal();
}

CardTypeRetVal cardtypeaction::lvlUp(GameState & gamestate, const MunchkinCard & card)
{
	if (!card.itemNeeds.empty())
	{
		gamestate.mouseparams.tutText = { "Bevor du ein level steigst", "fuehre diese Aktion durch: ", card.itemNeeds, "dann druecke den", "gruenen Knopf!" };
		gamestate.buttons.at(1).visible = true;
	}
	else
		gamestate.should_continue = true;

	if (gamestate.should_continue == true)
	{
		if (gamestate.player01.lvl < 9)
		{
			gamestate.player01.lvl += card.lvlUp;
			gamestate.mouseparams.tutText = { "Du steigst ein Level auf!", "Dein neues Level ist: ", std::to_string(gamestate.player01.lvl), "Um fortzufahren druecke den", "gruenen Button!" };

		}
		else
			gamestate.mouseparams.tutText = {"Mit Level 9 kannst du", "diese Karte nicht verwenden!", "Um fortzufahren druecke den", "gruenen Button!"};


		gamestate.should_continue = false;
		gamestate.end_turn = true;
		return CardTypeRetVal();

	}
}


#pragma endregion