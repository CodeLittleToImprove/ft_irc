/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Unoper.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 15:58:27 by pschmunk          #+#    #+#             */
/*   Updated: 2025/11/13 16:12:12 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

Unoper::Unoper(Server *server) : ACommand("UNOPER", server) {}

void Unoper::execute(Client *client, Tokenizer *tokens) const
{
	if (!isRegisteredFull(client) || !hasEnoughParams(client, tokens, 1))
		return;
	if (!client->isOper())
	{
		this->_server->response(client, ERR_NOPRIVILEGES, ":Permission denied");
		return;
	}

	std::string	target_name = tokens->get_param(0);
	Client		*target = this->_server->get_client(target_name);

	if (!target)
	{
		this->_server->response(client, ERR_NOSUCHNICK, ":Target-user not found");
		return;
	}
	if (!target->isOper())
	{
		this->_server->response(client, ERR_NOTOPER, ":" + target_name + " is not an operator");
		return;
	}
	target->takeOper();
	this->_server->response(client, ERR_NOTOPER, ":" + target_name + " is no longer an operator");
}