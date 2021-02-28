#pragma once
# include "opencv2/opencv.hpp"
# include "opencv2/imgcodecs.hpp"
# include "opencv2/imgproc.hpp"
#include <string>
#include <vector>
#include "Button.h"

using namespace std;

enum class MouseEvent
{
	lclick,
	rclick,
	move,
	none
};

struct MouseParams
{
	vector<cv::Point> poly;
	int markerId;
	vector<cv::Point2f> markerCorner;
	vector<string> tutText;
	cv::Point clickP;
	MouseEvent event;
};

struct PlayerStats
{
	string sex;
	vector<string> bonis;
	vector<string> curses;
	vector<string> itemEffects;
	vector<string> munchClasses;
	vector<string> munchRaces;
	int lvl;
	int strength;
	int hands;
	int runStrength;
	int availableClasses;
	int availableRaces;
	bool carriesLargeItems;
	bool hasArmor;
	bool hasHat;
	bool hasShoes;
};

struct GameState
{
	PlayerStats player01;
	MouseParams mouseparams;
	std::vector<Button> buttons;
	bool should_exit;
	bool should_continue;
	bool end_turn;
	bool run_away;
	bool remove_card;
	cv::Size canvas_size;
};