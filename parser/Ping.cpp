#include "ACommand.hpp"
//Incoming PING → respond with PONG
Ping::Ping(Server *server) : ACommand("PING", server)
{
}

void Ping::execute(Client *client, Tokenizer *tokens) const
{
	if (!has_enough_params(client, tokens, 1))
		return;

	std::string payload = tokens->get_params()[0];
	if (!payload.empty() && payload[0] == ':')
		payload = payload.substr(1);
	// std::cout << "debug payload: " << payload << std::endl;
	_server->sendRaw(client, PONG + payload + CRLF);
}
