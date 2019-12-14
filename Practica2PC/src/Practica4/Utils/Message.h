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
	SCROLL_FINISHED,
	SET_DIRTY,
	GAME_OVER,
	DEPOSIT,
	START_ROUND,
	ACTIVATE_DOORS,
	DEACTIVATE_DOORS,
	SELECT_DOLLARS
};

struct Message
{
	const MessageType type;
	Message(MessageType type) : type(type) {};
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

struct ScrollMessage : public Message //mensaje de scroll. Indica su direccion (-1, 1)
{
	const int dir;
	ScrollMessage(MessageType type, const int dir) : Message(type), dir(dir) {};
};

struct DepositMessage : public Message
{
	const char doorId;
	DepositMessage(MessageType type, const char doorId) : Message(type), doorId(doorId) {};
}; 

struct SelectDollarsMessage : public Message
{
	const char* ids;
	SelectDollarsMessage(MessageType type, const char* ids) : Message(type), ids(ids) {};
};


