#include "Server.hpp"

int main()
{
	uint16_t listenPort = 8080; // typical uint16_t is used for tdp and udp ports
	Server server(listenPort);
	server.run();
	return 0;
}

// int main(int ac, char **av)
// {
// 	(void)ac;
// 	Server	server(347858, "kaka");
// 	server.onClientMessage(av[1]);
// 	return (0);
// }