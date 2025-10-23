#include "Server.hpp"

int main (int argc, const char * argv[])
{
	if (argc < 2)
	{
		std::cout << "default password is password" << std::endl;
		uint16_t listenPort = 8080; // typical uint16_t is used for tdp and udp ports
		std::string password = "password";
		Server server(listenPort, password);
		server.run();
	}
	else if (argc == 3)
	{
		uint16_t listenPort = atoi(argv[1]);
		std::string password = argv[2];
		std::cout << "password from argv: "<< password << std::endl;
		Server server(listenPort, password);
		server.run();
	}
	return 0;
}

// int main()
// {
// 	uint16_t listenPort = 8080; // typical uint16_t is used for tdp and udp ports
// 	Server server(listenPort);
// 	server.run();
// 	return 0;
// }

// int main(int ac, char **av)
// {
// 	(void)ac;
// 	Server	server(347858, "kaka");
// 	server.onClientMessage(av[1]);
// 	return (0);
// }