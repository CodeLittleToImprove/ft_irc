/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 15:21:47 by phillymilly       #+#    #+#             */
/*   Updated: 2025/10/28 14:39:15 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "../server/Client.hpp"
# include <algorithm>

class Channel
{
	private:
		std::string				_name;
		std::string				_hostname;
		std::string				_topic;
		std::string				_key;
		unsigned int			_user_limit;
		unsigned int			_num_users;
		std::vector<Client *>	_clients; // only client objects which are in the channel
		std::vector<Client *>	_op_clients;
		std::vector<Client *>	_invited_clients;
		bool					_invite_only;
		bool					_topic_restriction;
		bool					_has_key;
		bool					_has_user_limit;
		bool					_is_empty;

	public:

		// Constructors
		Channel(std::string name, std::string hostname);

		// Member functions
		void	addClient(Client *client);
		void	addOpClient(Client *client);
		void	removeClient(Client *client);
		void	removeOpClient(Client *client);
		void	inviteClient(Client *client);
		void	changeUserNum(std::string mode);
		bool	isChOper(std::string nickname);
		bool	isInChannel(Client *client);
		bool	isInviteOnly();
		bool	isInvited(Client *client);
		bool	checkKey(std::string key);
		bool	hasUserLimit();
		void	broadcast(Client *sender, std::string command, std::string target, std::string message);

		// Getters
		std::string				getName();
		std::vector<Client *>	getClients();
		std::vector<Client *>	getOpClients(); //not used
		std::vector<Client *>	getInvitedClients();
		unsigned int			getUserNum();
		unsigned int			getUserLimit();
		bool					isKeyRequired() const;
		std::string				getClientNames();
		std::string				getTopic() const;
		std::string				getHostname() const;
		bool					isEmpty() const;
		std::string				getChannelOpNickNames() const;
		std::string				getModes() const;

		// Setters
		void	setInviteOnly(char mode);
		void	setKey(char mode, std::string key);
		void	setUserLimit(bool toggle, int num);
		void	setRestriction(char mode);
};
