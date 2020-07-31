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
	CHANGE_DOOR_TIME,
	MONEY_INSERTED
};

///mensaje por defecto. Contiene el tipo
struct Message
{
	const MessageType type;
	Message(MessageType type) : type(type) {};
};

/// todos los mensajes que necesiten un id
struct IDMessage : public Message
{
	const char id;
	IDMessage(MessageType type, const char id) : Message(type), id(id) {};
}; 

///mensaje enviado a bandido / cliente por las puertas al abrirse con la posicion en la que deben setearse.
///DoorId : identificador de la puerta que envia el mensaje
///characterId: identificador que selecciona la puerta que indica si va a ser cliente o bandido. O -> cliente, 1 -> bandido
struct DoorOpeningMessage : public Message 
{
	const char doorId, characterId;
	const int posX, posY;
	DoorOpeningMessage(MessageType type, const char doorId, const char characterId, const int posX, const int posY) : Message(type), doorId(doorId), characterId(characterId), posX(posX), posY(posY) {};
};

///mensaje de scroll. Indica su direccion (-1, 1)
struct ScrollMessage : public Message
{
	const int dir;
	ScrollMessage(MessageType type, const int dir) : Message(type), dir(dir) {};
};

///contiene un array de ids con los ids de los dolares que deben activarse
struct SelectDollarsMessage : public Message
{
	const char* ids;
	SelectDollarsMessage(MessageType type, const char* ids) : Message(type), ids(ids) {};
};

///mensaje de cambio del tiempo que tardan en cerrarse las puertas
struct ChangeDoorTimeMessage : public Message
{
	const float time;
	ChangeDoorTimeMessage(MessageType type, const float time) : Message(type), time(time) {};
};