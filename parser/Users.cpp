/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Users.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phillymilly <phillymilly@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:27:09 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/23 01:35:54 by phillymilly      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

Users::Users(Server *server) : ACommand("USERS", server) {}

void	Users::execute(Client *client, Tokenizer *tokens) const
{
	parser_debugging(tokens);
}