/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommand.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 16:12:57 by pschmunk          #+#    #+#             */
/*   Updated: 2025/11/13 17:50:39 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

ACommand::ACommand(std::string name, Server *server) : _name(name), _server(server) {}

ACommand::~ACommand(void) {}

bool	ACommand::isRegisteredFull(Client *client) const
{
	if (!client)
		return (false);
	if (client->isAuthenticated() && client->isRegistered())
		return (true);
	this->_server->response(client, ERR_NOTREGISTERED, ":You are not registered yet");
	return (false);
}

bool	ACommand::hasEnoughParams(Client *client, Tokenizer *tokens, size_t num) const
{
	if (!client || !tokens)
		return (false);
	if (tokens->get_params().size() < num)
	{
		this->_server->response(client, ERR_NEEDMOREPARAMS, ":Not enough parameters");
		return (false);
	}
	return (true);
}

bool	ACommand::hasChannelAndIsInChannel(Client *client, Channel *channel, std::string channel_name) const
{
	if (!channel)
	{
		this->_server->response(client, ERR_NOSUCHCHANNEL, ":Channel " + channel_name + " doesn't exist");
		return (false);
	}
	if (!channel->isInChannel(client))
	{
		this->_server->response(client, ERR_NOTONCHANNEL, ":You are not on the channel " + channel_name);
		return (false);
	}
	return (true);
}

void	ACommand::parserDebugging(Tokenizer *tokens) const
{
	std::cout	<< "Command " << this->_name << " called!" << std::endl;
	std::cout	<< "Prefix: ";
	if (tokens->get_prefix().empty())
		std::cout	<< "None" << std::endl;
	else
		std::cout	<< tokens->get_prefix() << std::endl;
	std::cout	<< "Command: " << tokens->get_command() << std::endl;
	std::cout	<< "Parameter: ";
	if (tokens->get_params().empty())
		std::cout	<< "None" << std::endl;
	else
	{
		for (size_t i = 0; i < tokens->get_params().size(); i++)
			std::cout	<< tokens->get_params().at(i) << "|";
		std::cout	<< std::endl;
	}
}