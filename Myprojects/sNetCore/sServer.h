#pragma once
#include "sSessionMgr.h"
#include "sPacketPool.h"
class sServer : public sNetSingleton<sServer>, public sThread
{
	friend class sNetSingleton < sServer>;
public:
	sPacketPool m_RecvPacketPool;
	sPacketPool m_SendPacketPool;
	sPacketPool m_SendBroadcastPacketPool;
public:
	typedef void (sServer::*CallFuction)(sPacket& t);
	typedef map<int, CallFuction>::iterator FunctionIterator;
	map<int, CallFuction> m_fnExecutePacket;
public:
	virtual  bool MakePacket(UPACKET& packet, char* msg, int iLen, uint16_t type);
	virtual  bool SendData(sNetUser& user, UPACKET& msg);
	virtual  bool Broadcastting();
	virtual  bool Run() override;
	virtual  bool Init();
public:
	void ChatMsg(sPacket& t);
	void MoveAction(sPacket& t);
	void Login(sPacket& t);
public:
	sServer();
	virtual ~sServer();
};
#define I_Server sServer::GetInstance()

