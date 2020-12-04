#pragma once
#include <windows.h>
#include <iostream>

#pragma pack(push, 1)
typedef struct
{
	uint16_t iotype;
	uint16_t len;
	uint16_t type;
	uint32_t time;
}PACKET_HEADER;
#define PACKET_HEADER_SIZE 10
typedef struct
{
	PACKET_HEADER ph;
	int8_t msg[1024];
}UPACKET, *P_UPACKET;

struct sChatMsg
{
	int iCnt;
	char szName[32];
	char buffer[128];
};

struct sLogin
{
	char szID[16];
	char szPS[16];
};
#define sLoginSize sizeof(sLogin)
struct sLoginResult
{
	int iRet;
};
#pragma pack(pop)

enum SPACKET_TYPE
{
	PACKET_LOGIN_REQ = 10,
	PACKET_LOGIN_ACK,
	PACKET_CHAT_MSG = 100,
	PACKET_TSTATUS_VARS,
};