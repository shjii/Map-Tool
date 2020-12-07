#include "sUser.h"
sUser::sUser()
{
	iRecvSize = 0;
	iSendSize = 0;
	ZeroMemory(recvBuf, sizeof(char) * 10000);
}
sUser::~sUser()
{

}