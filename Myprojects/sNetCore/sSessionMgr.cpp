#include "sSessionMgr.h"

void sSessionMgr::AddUser(sNetUser * pUser)
{
	{
		sLock lock(this);
		m_UserList[pUser->m_Sock] = pUser;
	}
}


bool sSessionMgr::Run()
{
	return false;
}


bool sSessionMgr::DelUser(sNetUser* user)
{
	{
		sLock lock(this);
		printf("\nÇØÁ¦->%s:%d", inet_ntoa(user->m_SockAddr.sin_addr),
			ntohs(user->m_SockAddr.sin_port));

		CloseUser(user);
		map<SOCKET, sNetUser*>::iterator iter = m_UserList.find(user->m_Sock);
		if (iter != m_UserList.end())
		{
			m_UserList.erase(iter);
		}
	}
	return true;
}




bool sSessionMgr::CloseUser(sNetUser * user)
{
	if (user == nullptr) return true;
	shutdown(user->m_Sock, SD_SEND);
	closesocket(user->m_Sock);
	delete user;
	user = nullptr;
	return true;
}

sSessionMgr::sSessionMgr()
{
	CreateThread();
}

sSessionMgr::~sSessionMgr()
{
}
