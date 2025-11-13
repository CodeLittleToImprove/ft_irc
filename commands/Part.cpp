#include "ACommand.hpp"

Part::Part(Server *server) : ACommand("PART", server){}

void Part::execute(Client *client, Tokenizer *tokens) const
{
	// parser_debugging(tokens);
	if (!client->isAuthenticated() && !client->isRegistered())
	{
		this->_server->response(client, ERR_NOTREGISTERED, ":You are not registered yet");
		return;
	}

	if (!hasEnoughParams(client, tokens, 1))
		return;

	std::string channel_name = tokens->getParams()[0];
	Channel *channel = _server->getChannel(channel_name);

	if (!hasChannelAndIsInChannel(client, channel, channel_name))
		return;
	std::string reason = "Leaving";
	if (tokens->getParams().size() > 1)
		reason = tokens->getParam(1);
	channel->broadcast(client, "PART", channel->getName(), reason);
	client->request(client, "PART", channel->getName(), reason);
	channel->removeClient(client);
	if (channel->isEmpty())
		_server->removeChannel(channel_name);
}
