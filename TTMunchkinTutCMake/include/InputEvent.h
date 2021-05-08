/** \file InputEvent.h
*	\author Benjamin Lueben
*
*	\brief Class for all input events of the player through mouseklicks

*/

#pragma once

/**
	\brief enum of possible event types
*/
enum class EventType
{
	LmPress,	///< left mouse press
	LmRelease,	///< left mouse release
	RmPress,	///< right mouse press
	RmRelease,	///< right mouse release
	KeyPress,	///< keyboard key press
	KeyRelease	///< keyboard key release
};

struct InputEvent
{
	EventType type;

};