#include "sNetwork.h"

void main()
{
	sNetwork net;
	if (net.InitNetwork() == false)
	{
		return;
	}
	net.Run();
	net.DeleteNetwork();
}