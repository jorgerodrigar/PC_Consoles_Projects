#pragma once
#include <string>
#include <SDL_events.h>

enum MessageType
{
	DEFAULT,
	TEST,
	INPUT_EVENT
};

struct Message
{
	MessageType type;
	Message(MessageType type) : type(type) {};
};

struct TestMessage: public Message
{
	std::string test = "testing";
	TestMessage(MessageType type) : Message(type) {};
};

struct InputEventMessage : public Message 
{
	const SDL_Event event;
	InputEventMessage(MessageType type, const SDL_Event event) : Message(type), event(event) {};
};

