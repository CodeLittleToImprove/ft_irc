/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 20:22:50 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/24 22:11:45 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/******************************************************************************/
/*                              Constructors                                  */
/******************************************************************************/

Channel::Channel(std::string name, std::string hostname) : _name(name), _hostname(hostname) {}

/******************************************************************************/
/*                            Member Functions                                */
/******************************************************************************/

void Channel::addClient(Client *client)
{
	this->_clients.push_back(client);
}

void Channel::addOpClient(Client *client)
{
	std::vector<Client *>::iterator it = std::find(_clients.begin(), _clients.end(), client);
	if (it == _clients.end())
		this->_clients.push_back(client);
	this->_op_clients.push_back(client);
}

void Channel::removeClient(Client *client)
{
	std::vector<Client *>::iterator it = std::find(_clients.begin(), _clients.end(), client);
	if (it == _clients.end())
		std::cout	<< "Client not found." << std::endl;
	else
		_clients.erase(it);
}

void Channel::removeOpClient(Client *client)
{
	std::vector<Client *>::iterator it = std::find(_op_clients.begin(), _op_clients.end(), client);
	if (it == _op_clients.end())
		std::cout	<< "Client not found." << std::endl;
	else
		_op_clients.erase(it);
}

bool Channel::isOper(std::string nickname)
{
	for (size_t i = 0; i < this->_op_clients.size(); i++)
	{
		if (nickname == this->_op_clients.at(i)->getNickname())
			return (true);
	}
	return (false);
}

bool Channel::isInChannel(Client *client)
{
	std::vector<Client *>::iterator it = std::find(_clients.begin(), _clients.end(), client);
	if (it == _clients.end())
		return (false);
	return (true);
}

/******************************************************************************/
/*                                 Getters                                    */
/******************************************************************************/

std::string Channel::getName()
{
	return (this->_name);
}

std::vector<Client *> Channel::getClients()
{
	return (this->_clients);
}

std::vector<Client *> Channel::getOpClients()
{
	return (this->_op_clients);
}