/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:22:25 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/28 14:53:18 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

Mode::Mode(Server *server) : ACommand("MODE", server) {}

void Mode::execute(Client *client, Tokenizer *tokens) const
{
	parser_debugging(tokens);

	if (!is_registered_full(client) || !has_enough_params(client, tokens, 1))
		return;
	
	std::string	channel_name = tokens->get_param(0);
	Channel		*channel = this->_server->get_channel(channel_name);

	// // ensures the channel exists and the client issuing the command is a member of it
	if (!hasChannelAndIsInChannel(client, channel, channel_name))
		return;
	// if (tokens->get_params().size() == 1)
	// {
	// 	this->_server->response(client, RPL_CHANNELMODEIS, channel_name + )
	// }
	if (tokens->get_params().size() == 1)
	{
		std::string current_modes = channel->getModes();
		// std::cout << "DEBUG currentmodes: " << current_modes << std::endl;
		this->_server->response(client, RPL_CHANNELMODEIS, channel_name + " " + current_modes);
		return;
	}
	if (!client->isOper() && !channel->isChOper(client->getNickname()))
	{
		this->_server->response(client, ERR_NOPRIVILEGES, ":Permission denied");
		return;
	}
	// mode is called with no extra arguments

	// attempt to retrieve optional mode flag (e.g., "+i", "-o", "+k")
	std::string mode_flag = tokens->get_param(1);
	std::string param = "";
	if (mode_flag.length() != 2 && mode_flag[0] != '-' && mode_flag[0] != '+')
	{
		this->_server->response(client, ERR_UNKNOWNMODE, ":Unknown mode");
		return;
	}
	if (mode_flag == "+k" || mode_flag == "+o" || mode_flag == "+l")
	{
		if (tokens->get_params().size() < 3)
		{
			this->_server->response(client, ERR_NEEDMOREPARAMS, ":not enough parameters");
			return;
		}
		param = tokens->get_param(2);
	}
	switch (mode_flag[1])
	{
		case 'i':
			channel->setInviteOnly(mode_flag[0]);
			break;
		case 't':
			channel->setRestriction(mode_flag[0]);
			break;
		case 'k':
			channel->setKey(mode_flag[0], param);
			break;
		case 'o':
		{
			Client *target = this->_server->get_client(param);
			if (!channel->isInChannel(target))
			{
				this->_server->response(client, ERR_USERNOTINCHANNEL,
				                        ":Target-user is not on the channel " + channel_name);
				return;
			}
			if (channel->isChOper(param))
			{
				if (mode_flag[0] == '+')
				{
					this->_server->response(client, ERR_ALREADYOPER, ":User is already channel operator");
					return;
				}
				channel->removeOpClient(target);
			}
			else
			{
				if (mode_flag[0] == '-')
				{
					this->_server->response(client, ERR_CHANOPRIVSNEEDED, ":User is not an operator");
					return;
				}
				channel->addOpClient(target);
			}
			break;
		}
		case 'l':
			if (mode_flag[0] == '+')
			{
				if (param.find_first_not_of("0123456789") != std::string::npos)
				{
					this->_server->response(client, ERR_UNKNOWNMODE, ":Userlimit has to be a number");
					return;
				}
				int limit = std::atoi(param.c_str());
				if (limit < 1)
				{
					this->_server->response(client, ERR_UNKNOWNMODE, ":Userlimit has to be greater than 0");
					return;
				}
				channel->setUserLimit(true, limit);
			}
			channel->setUserLimit(false, 0);
			break;
		default:
			this->_server->response(client, ERR_UNKNOWNMODE, ":Unknown mode");
			return;
	}
	std::string msg = !param.empty() ? " " + param : "";
	client->request(client, this->_name, channel->getName(), mode_flag + msg);
}
