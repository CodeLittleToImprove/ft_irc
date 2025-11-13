#include "Server.hpp"

int main(int argc, const char *argv[])
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <server_password>" << std::endl;
		return -1;
	}
	uint16_t listen_port = 0;
	std::string port_str = argv[1];
	std::istringstream ss(port_str);
	int temp_port;

	if (!(ss >> temp_port) || !ss.eof())
	{
		std::cerr << "Error: Port must be a valid, whole number." << std::endl;
		return -1;
	}
	if (temp_port < 1 || temp_port > 65535)
	{
		std::cerr << "Error: Port number must be between 1 and 65535." << std::endl;
		return -1;
	}
	listen_port = static_cast<uint16_t>(temp_port);
	std::cout << "listen_Port from argv: " << listen_port << std::endl;
	std::string password = argv[2];
	std::cout << "password from argv: " << password << std::endl;
	Server server(listen_port, password);
	try
	{
		server.run();
	}
	catch (const std::exception &e)
	{
		std::cerr << "Fatal Server Error: " << e.what() << std::endl;
	}
	return 0;
}
