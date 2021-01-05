#include "sLoginAcceptor.h"
#include "sIocp.h"
#include "sSessionMgr.h"
bool sLoginAcceptor::AddSession(SOCKET sock, SOCKADDR_IN addr)
{
	{
		sLock lock(this);
		sChatUser* user = new sChatUser;
		user->m_Sock = sock;
		user->m_SockAddr = addr;
		I_Session.AddUser(user);
		I_Iocp.SetBind(sock, (ULONG_PTR)user);
		user->WaitReceive();
		printf("\nÁ¢¼Ó -> %s: %d", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
	}
	return true;
}