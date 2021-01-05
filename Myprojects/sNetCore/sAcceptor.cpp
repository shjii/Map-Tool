#include "sAcceptor.h"
#include "sIocp.h"
#include "sSessionMgr.h"

bool sAcceptor::InitNetwork(string ip, int iPort)
{
	int iRet;
	m_Sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_Sock == INVALID_SOCKET)
	{
		Error("socket");
		return false;
	}
	// 소켓 타입 확인
	int sockType;
	int sockTypeLen = sizeof(int);
	getsockopt(m_Sock, SOL_SOCKET, SO_TYPE,
		(char*)&sockType, &sockTypeLen);
	if (sockType == SOCK_STREAM)
		printf("%s\r\n", "SOCK_STREAM.");
	else
		printf("%s\r\n", "SOCK_DGRAM");

	//송수신 버퍼 크기 확인	
	int iSendBuffer = 100000;
	setsockopt(m_Sock, SOL_SOCKET, SO_SNDBUF,
		(char*)&iSendBuffer, sockTypeLen);

	getsockopt(m_Sock, SOL_SOCKET, SO_SNDBUF,
		(char*)&sockType, &sockTypeLen);
	printf("SendBuffer=%d\n", sockType);
	getsockopt(m_Sock, SOL_SOCKET, SO_RCVBUF,
		(char*)&sockType, &sockTypeLen);
	printf("RecvBuffer=%d\n", sockType);


	int optval = 1;
	// 1개의 프로세스에서만 ip&port 재사용 가능
	// bind함수에서 오류를 얻는다.
	Check(
		setsockopt(m_Sock, SOL_SOCKET, SO_EXCLUSIVEADDRUSE,
		(char*)&optval, sizeof(optval)),
		__LINE__
	);
	// 다수 프로세스에서 ip&port 재사용 가능
	/*iRet = setsockopt(m_Sock, SOL_SOCKET, SO_REUSEADDR,
		(char*)&optval, sizeof(optval));
	if (iRet == SOCKET_ERROR)
	{
		return false;
	}*/

	linger optLinger;
	int iLingerLen = sizeof(optLinger);
	optLinger.l_onoff = 1;
	optLinger.l_linger = 1000;
	if (setsockopt(m_Sock, SOL_SOCKET, SO_LINGER,
		(char*)&optLinger, iLingerLen) != 0)
	{
		Error("SO_LINGER");
		return false;
	}

	if (InitSocket(ip, iPort) == false)
	{
		return false;
	}
	return true;
}
bool sAcceptor::InitSocket(string ip, int iPort)
{
	// 넌블로킹 소켓으로 전환
	unsigned long iMode = 1;
	ioctlsocket(m_Sock, FIONBIO, &iMode);

	SOCKADDR_IN sa;
	// 바이트 정렬 구조 	
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY);// inet_addr("192.168.0.151");
	//error C4996 : 'inet_addr' : Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS
	sa.sin_port = htons(iPort);

	//SO_REUSEADDR가 설정된 소켓이 있을 경우는 오류가 된다.
	Check(
		bind(m_Sock, (SOCKADDR*)&sa, sizeof(sa)),
		__LINE__
	);
	int iRet = listen(m_Sock, SOMAXCONN);
	if (iRet == SOCKET_ERROR)
	{
		Error("listen");
		return false;
	}
	return true;
}
bool sAcceptor::CloseSocket()
{
	closesocket(m_Sock);
	return true;
}
bool sAcceptor::DeleteNetwork()
{
	CloseSocket();
	return true;
}

bool sAcceptor::Run()
{
	while (m_bStarted)
	{
		{
			sLock lock(this);
			if (Accept() == false) break;
		}
		Sleep(1);
	}
	return false;
}
bool sAcceptor::Accept()
{
	SOCKADDR_IN clientAddr;
	int len = sizeof(clientAddr);

	SOCKET client = accept(m_Sock, (SOCKADDR*)&clientAddr, &len);
	if (client == INVALID_SOCKET)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			return false;
		}
	}
	else
	{
		AddSession(client, clientAddr);
	}
	return true;
}
bool sAcceptor::AddSession(SOCKET sock, SOCKADDR_IN addr)
{
	{
		sLock lock(this);
		sNetUser* user = new sNetUser;
		user->m_Sock = sock;
		user->m_SockAddr = addr;
		I_Session.AddUser(user);
		I_Iocp.SetBind(sock, (ULONG_PTR)user);
		user->WaitReceive();
		printf("\n접속->%s:%d",
			inet_ntoa(addr.sin_addr),
			ntohs(addr.sin_port));
	}
	return true;
}



sAcceptor::sAcceptor()
{
	// 2.2 ver
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
}

sAcceptor::~sAcceptor()
{
	WSACleanup();
}
