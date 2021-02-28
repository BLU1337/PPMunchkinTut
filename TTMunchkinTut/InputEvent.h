#pragma once

enum class EventType
{
	LmPress,
	LmRelease,
	RmPress,
	RmRelease,
	KeyPress,
	KeyRelease
};

struct InputEvent
{
	EventType type;

};