/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:25:24 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/24 22:37:30 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

Pass::Pass(Server *server) : ACommand("PASS", server) {}

void	Pass::execute(Client *client, Tokenizer *tokens) const
{

	parser_debugging(tokens);
	// Check if client is already authenticated

	if (client->isAuthenticated())
	{
		_server->response(client, ERR_ALREADYREGISTRED, ":You are already authenticated from pass");
		return;
	}
	// Check if message has enough parameters
	if (!has_enough_params(client, tokens, 1))
		return;


	client->authenticate(tokens->get_params()[0]);

	if (!client->isAuthenticated())
	{
		_server->response(client,ERR_PASSWDMISMATCH, ":Incorrect password");
		client->closeConnection("disconnected due to authentication failure");
	}
	// user filled nickname, usernameregistered
	std::cout << "pre if registered" << std::endl;
	if (client->is_registered()) // this gets not trigger yet
		_server->response(client, RPL_WELCOME, ":Welcome to the Internet Relay Network " + client->getNickname() + "!");
	//need to add host- and username later

}

