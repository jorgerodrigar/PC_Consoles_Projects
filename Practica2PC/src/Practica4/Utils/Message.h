#pragma once
#include <string>
#include <SDL_events.h>

enum MessageType
{
	DEFAULT,
	TEST,
	INPUT_EVENT,
	SHOOT
};

struct Message
{
	const MessageType type;
	Message(MessageType type) : type(type) {};
};

struct TestMessage: public Message
{
	const std::string test = "testing";
	TestMessage(MessageType type) : Message(type) {};
};

struct InputEventMessage : public Message 
{
	const SDL_Event event;
	InputEventMessage(MessageType type, const SDL_Event event) : Message(type), event(event) {};
};

struct ShootMessage : public Message //mensaje de disparo. Lleva el identificador de la puerta que ha sido disparada
{
	const unsigned char id;
	ShootMessage(MessageType type, const unsigned char id) : Message(type), id(id) {};
};

