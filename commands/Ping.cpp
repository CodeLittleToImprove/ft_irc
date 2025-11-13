#include "ACommand.hpp"
//Incoming PING → respond with PONG
Ping::Ping(Server *server) : ACommand("PING", server)
{
}

void Ping::execute(Client *client, Tokenizer *tokens) const
{
	if (!hasEnoughParams(client, tokens, 1))
		return;

	std::string payload = tokens->getParams()[0];
	if (!payload.empty() && payload[0] == ':')
		payload = payload.substr(1);
	_server->sendRaw(client, PONG + payload + CRLF);
}
