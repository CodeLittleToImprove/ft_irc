/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phillymilly <phillymilly@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 18:21:05 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/23 01:34:21 by phillymilly      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

Join::Join(Server *server) : ACommand("JOIN", server) {}

void	Join::execute(Client *client, Tokenizer *tokens) const
{
	(void) client;
	parser_debugging(tokens);
}