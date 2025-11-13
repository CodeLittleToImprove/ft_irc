/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:24:21 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/27 17:39:08 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

Oper::Oper(Server *server) : ACommand("OPER", server) {}

void	Oper::execute(Client *client, Tokenizer *tokens) const
{
	// parser_debugging(tokens);
	if (!isRegisteredFull(client))
		return;
	if (!hasEnoughParams(client, tokens, 1))
		return;
	
	std::string target_name = tokens->getParam(0);
	Client *target = this->_server->getClient(target_name);

	if (!target)
	{
		this->_server->response(client, ERR_NOSUCHNICK, ":User not found");
		return;
	}
	if (!target->isRegistered())
	{
		this->_server->response(client, ERR_NOTREGISTERED, ":" + target_name + " is not registered yet");
		return;
	}
	if (target->isOper())
	{
		this->_server->response(client, RPL_YOUREOPER, ":" + target_name + " is already an operator");
		return;
	}
	if (tokens->getParams().size() == 1)
	{
		if (!client->isOper())
		{
			this->_server->response(client, ERR_NOPRIVILEGES, ":Permission denied!");
			return;
		}
		target->giveOper(this->_server->getOperPassword(), this->_server);
		this->_server->response(client, RPL_YOUREOPER, ":" + target_name + " is now an operator");
	}
	else
	{
		target->giveOper(tokens->getParam(1), this->_server);
		if (target->isOper())
		{
			this->_server->response(client, RPL_YOUREOPER, ":" + target_name + " is now an operator");
			return;
		}
		this->_server->response(client, ERR_PASSWDMISMATCH, ":Operator password is incorrect");
	}
}