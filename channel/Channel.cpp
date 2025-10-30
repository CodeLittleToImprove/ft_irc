/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/24 20:22:50 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/28 14:39:24 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/******************************************************************************/
/*                              Constructors                                  */
/******************************************************************************/

Channel::Channel(std::string name, std::string hostname)
: _name(name), _hostname(hostname), _num_users(0), _invite_only(false),  _restriction(false), _has_key(false), _is_empty(true) {}

/******************************************************************************/
/*                            Member Functions                                */
/******************************************************************************/

void Channel::addClient(Client *client)
{
	this->_clients.push_back(client);
	_is_empty = false;
}

void Channel::addOpClient(Client *client)
{
	// std::vector<Client *>::iterator it = std::find(_clients.begin(), _clients.end(), client);
	// if (it == _clients.end())
	// 	this->_clients.push_back(client); //causes duplicates
	// this->_op_clients.push_back(client);

	// Add to _op_clients if not already there
	if (std::find(_op_clients.begin(), _op_clients.end(), client) == _op_clients.end())
		_op_clients.push_back(client);
}

void Channel::removeClient(Client *client)
{
	std::cout << "[DEBUG] removeClient() actually deleting client fd=" << client->getClient_fd() << std::endl;
	std::vector<Client *>::iterator it = std::find(_clients.begin(), _clients.end(), client);
	if (it == _clients.end())
		std::cout	<< "Client not found." << std::endl;
	else
		_clients.erase(it);
	this->removeOpClient(client);
	if (_clients.empty())
		_is_empty = true;
}

void Channel::removeOpClient(Client *client)
{
	std::vector<Client *>::iterator it = std::find(_op_clients.begin(), _op_clients.end(), client);
	if (it == _op_clients.end())
		std::cout	<< "Client not found." << std::endl;
	else
		_op_clients.erase(it);
}

void Channel::inviteClient(Client *client)
{
	this->_invited_clients.push_back(client);
}

void Channel::changeUserNum(std::string mode)
{
	if (mode == "add")
		this->_num_users++;
	else if (mode == "sub")
	{
		if (this->_num_users > 0) // should pre probably bigger than 0 otherwise it would not compile
			this->_num_users--;
	}
}

bool Channel::isChOper(std::string nickname)
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

bool Channel::isInviteOnly()
{
	return (this->_invite_only);
}

bool Channel::isInvited(Client *client)
{
	std::vector<Client *>::iterator it = std::find(_invited_clients.begin(), _invited_clients.end(), client);
	if (it == _invited_clients.end())
		return (false);
	else
		return (true);
}

bool Channel::checkKey(std::string key)
{
	if (this->_has_key)
	{
		if (this->_key == key)
			return (true);
		return (false);
	}
	return (true);
}

bool Channel::hasUserLimit()
{
	return (this->_user_limit);
}

void Channel::broadcast(Client *sender, std::string command, std::string target, std::string message)
{
	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		Client *member = *it;
		if (member == sender)
			continue;
		member->request(sender, command, target,message);
	}
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

std::vector<Client *> Channel::getInvitedClients()
{
	return (this->_invited_clients);
}

unsigned int Channel::getUserNum()
{
	return (this->_num_users);
}

unsigned int Channel::getUserLimit()
{
	return (this->_user_limit);
}

bool Channel::isKeyRequired() const
{
	return (this->_has_key);
}

std::string	Channel::getClientNames()
{
	std::string names;

	for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		Client *client = *it;
		if (!names.empty())
			names += " ";
		// Prefix with @ if operator
		bool is_channel_operator = (std::find(_op_clients.begin(), _op_clients.end(), client) != _op_clients.end());
		if (is_channel_operator)
			names += "@";
		names += client->getNickname();
	}

	return names;
}

std::string Channel::getTopic() const
{
	return this->_topic;
}

std::string	Channel::getHostname() const
{
	return this->_hostname;
}

bool Channel::isEmpty() const
{
	return _clients.empty();
}

/******************************************************************************/
/*                                 Setters                                    */
/******************************************************************************/

void Channel::setInviteOnly(char mode)
{
	if (mode == '+')
		this->_invite_only = true;
	else if (mode == '-')
		this->_invite_only = false;
}

void Channel::setKey(char mode, std::string key)
{
	if (mode == '+')
		this->_has_key = true;
	else if (mode == '-')
		this->_has_key = false;
	this->_key = key;
}

void Channel::setUserLimit(bool mode, int num)
{
	this->_has_user_limit = mode;
	if (this->_has_user_limit)
		this->_user_limit = num;
}

void Channel::setRestriction(char mode)
{
	if (mode == '+')
		this->_restriction = true;
	else if (mode == '-')
		this->_restriction = false;
}