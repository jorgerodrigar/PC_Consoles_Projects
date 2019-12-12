#pragma once
#include <string>

enum MessageType
{
	DEFAULT,
	TEST,
	INPUT_EVENT,
	SHOOT,
	DOOR_CLOSING
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

struct ShootMessage : public Message //mensaje de disparo. Lleva el identificador de la puerta que ha sido disparada
{
	const char id;
	ShootMessage(MessageType type, const char id) : Message(type), id(id) {};
};

struct DoorClosingMessage : public Message //mensaje de disparo. Lleva el identificador de la puerta que ha sido disparada
{
	const char id;
	DoorClosingMessage(MessageType type, const char id) : Message(type), id(id) {};
};

