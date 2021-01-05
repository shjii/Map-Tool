#pragma once
#include "sNetUser.h"
#include "sThread.h"

class sSessionMgr : public sNetSingleton<sSessionMgr>,
					public sThread
{
private:
	friend class sNetSingleton<sSessionMgr>;
public:
	map<SOCKET, sNetUser*> m_UserList;
public:
	virtual void AddUser(sNetUser* pUser);
	virtual bool Run() override;
	virtual bool DelUser(sNetUser* user);
	virtual bool CloseUser(sNetUser* user);
public:
	sSessionMgr();
	virtual ~sSessionMgr();
};
#define I_Session sSessionMgr::GetInstance()

