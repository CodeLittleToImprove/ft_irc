/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Info.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phillymilly <phillymilly@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 17:10:16 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/23 01:33:31 by phillymilly      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

Info::Info(Server *server) : ACommand("INFO", server) {}

void	Info::execute(Client *client, Tokenizer *tokens) const
{
	parser_debugging(tokens);
}