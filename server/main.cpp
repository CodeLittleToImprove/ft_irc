#include "Server.hpp"

int main()
{
	uint16_t listenPort = 8080; // typical uint16_t is used for tdp and udp ports
	Server server(listenPort);
	server.run();
	return 0;
}
