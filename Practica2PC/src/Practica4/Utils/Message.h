#pragma once
#include <string>

enum MessageType
{
	DEFAULT,
	TEST,
	INPUT_EVENT,
	SHOOT,
	START_EVENT,
	DOOR_OPENING,
	DOOR_CLOSED,
	LATERAL_SCROLL,
	SET_DIRTY,
	GAME_OVER,
	DEPOSIT,
	START_ROUND
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

struct DoorOpeningMessage : public Message 
{
	const char doorId, id;
	DoorOpeningMessage(MessageType type, const char doorId, const char id) : Message(type), doorId(doorId), id(id) {};
};

struct StartEventMessage : public Message
{
	const char id;
	StartEventMessage(MessageType type, const char id) : Message(type), id(id) {};
};

struct DoorClosedMessage : public Message
{
	const char id;
	DoorClosedMessage(MessageType type, const char id) : Message(type), id(id) {};
};

struct ScrollMessage : public Message //mensaje de scroll. Indica la velocidad del scroll y su direccion (-1, 1)
{
	const float vel;
	const int dir;
	ScrollMessage(MessageType type, const float vel, const int dir) : Message(type), vel(vel), dir(dir) {};
};

struct SetDirtyMessage : public Message
{
	SetDirtyMessage(MessageType type) : Message(type) {};
};

struct GameOverMessage : public Message
{
	GameOverMessage(MessageType type) : Message(type) {};
};

struct DepositMessage : public Message
{
	const char doorId;
	DepositMessage(MessageType type, const char doorId) : Message(type), doorId(doorId) {};
}; 

struct StartRoundMessage : public Message
{
	StartRoundMessage(MessageType type) : Message(type) {};
};


