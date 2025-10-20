/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACommand.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 16:12:57 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/20 18:31:23 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ACommand.hpp"

ACommand::ACommand(std::string name, Server *server) : _name(name), _server(server) {}

ACommand	&ACommand::operator=(const ACommand &copy)
{
	if (this != &copy)
	{
		this->_name		= copy._name;
		this->_server	= copy._server;
	}
	return (*this);
}