/** \file GameState.h
*	\author Benjamin Lueben
*
*	\brief GameState class to store all needed parameters for tutorial

	In this class all the parameters that are needed for the tutorial are stored.

*/

#pragma once
# include "opencv2/opencv.hpp"
# include "opencv2/imgcodecs.hpp"
# include "opencv2/imgproc.hpp"
#include <string>
#include <vector>
#include "Button.h"

using namespace std;

/**
	\brief	Specifies the mouse event
*/
enum class MouseEvent
{
	lclick,	///< left click
	rclick, ///< right click
	move,	///< mouse move
	none	///< empty event
};

/**
	\brief	Holds data of a mouse event
*/
struct MouseParams
{
	vector<cv::Point> poly;				///< Polygon of the clicked marker
	int markerId;						///< Id of the marker
	vector<cv::Point2f> markerCorner;	///< Four corners of the marker
	vector<string> tutText;				///< Tutorial text to display for the marker
	cv::Point clickP;					///< Mouse position
	MouseEvent event;					///< Mouse event type
};

/**
\brief Holds data of player stats
*/
struct PlayerStats
{
	string sex;						///< sex of player (male/female)
	vector<string> bonis;			///< bonis for player that are gained through different cards e.g. munchkin class
	vector<string> curses;			///< permanent curses that are a burden for the player
	vector<string> itemEffects;		///< effects of different items that are supporting the player e.g. attack with fire
	vector<string> munchClasses;	///< munchkin classes the player optained
	vector<string> munchRaces;		///< munchkin races the player optained
	int lvl;						///< level of the player
	int strength;					///< strength of a player through his equipment
	int hands;						///< how many hands player has left that are not holding an object
	int runStrength;				///< dice roll hat to be larger than x for the player to run away
	int availableClasses;			///< how many classes can the player optain
	int availableRaces;				///< how many races can the player optain
	bool carriesLargeItems;			///< wether or not the player carries a large item
	bool hasArmor;					///< wether or not the player carries an armor
	bool hasHat;					///< wether or not the player carries a hat
	bool hasShoes;					///< wether or not the player carries shoes
};

/**
	data of all needed params for the tutorial
*/
struct GameState
{
	PlayerStats player01;			///< playerStats object for data of player stats that are needed
	MouseParams mouseparams;		///< mouseParams object for data of tutorial that are needed e.g. tutorial text
	std::vector<Button> buttons;	///< buttons that are needed for the tutorial
	bool should_exit;				///< param wether or not the game should exit after exit button was pressed
	bool should_continue;			///< param for when user input is awaited and the tutorial needs to jump to a different point in logic function
	bool end_turn;					///< param to signal the player that the end of the turn is reached
	bool run_away;					///< param for when the user needs to run away and the tutorial triggers an random number as dice roll
	bool remove_card;				///< param for when the player chooses or needs to get rid of a card he has equiped
	cv::Size canvas_size;			///< param of the canvas size used for the buttons
};