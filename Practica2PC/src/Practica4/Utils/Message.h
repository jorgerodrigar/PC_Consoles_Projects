#pragma once
#include <string>

enum MessageType
{
	DEFAULT,
	TEST,
	INPUT_EVENT,
	SHOOT,
	DOOR_CLOSING,
	LATERAL_SCROLL
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

struct ScrollMessage : public Message //mensaje de scroll. Indica la velocidad del scroll y su direccion (-1, 1)
{
	const float vel;
	const int dir;
	ScrollMessage(MessageType type, const float vel, const int dir) : Message(type), vel(vel), dir(dir) {};
};

