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
	SCROLL_STARTED,
	SCROLL_FINISHED,
	SET_DIRTY,
	GAME_OVER,
	DEPOSIT,
	START_ROUND,
	ACTIVATE_DOORS,
	DEACTIVATE_DOORS,
	SELECT_DOLLARS,
	CHANGE_DOOR_TIME
};

struct Message
{
	const MessageType type;
	Message(MessageType type) : type(type) {};
};

struct IDMessage : public Message // todos los mensajes que necesiten un id
{
	const char id;
	IDMessage(MessageType type, const char id) : Message(type), id(id) {};
}; 

struct DoorOpeningMessage : public Message 
{
	const char doorId, id;
	const int posX, posY;
	DoorOpeningMessage(MessageType type, const char doorId, const char id, const int posX, const int posY) : Message(type), doorId(doorId), id(id), posX(posX), posY(posY) {};
};

struct ScrollMessage : public Message //mensaje de scroll. Indica su direccion (-1, 1)
{
	const int dir;
	ScrollMessage(MessageType type, const int dir) : Message(type), dir(dir) {};
};

struct SelectDollarsMessage : public Message
{
	const char* ids;
	SelectDollarsMessage(MessageType type, const char* ids) : Message(type), ids(ids) {};
};

struct ChangeDoorTimeMessage : public Message //mensaje de cambio del tiempo que tardan en cerrarse las puertas
{
	const float time;
	ChangeDoorTimeMessage(MessageType type, const float time) : Message(type), time(time) {};
};