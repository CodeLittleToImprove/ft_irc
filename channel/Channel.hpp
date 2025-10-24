/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 15:21:47 by phillymilly       #+#    #+#             */
/*   Updated: 2025/10/24 22:11:57 by pschmunk         ###   ########.fr       */
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
		std::vector<Client *>	_clients;
		std::vector<Client *>	_op_clients;
	public:

		// Constructors
		Channel(std::string name, std::string hostname);

		// Member functions
		void	addClient(Client *client);
		void	addOpClient(Client *client);
		void	removeClient(Client *client);
		void	removeOpClient(Client *client);
		bool	isOper(std::string nickname);
		bool	isInChannel(Client *client);

		// Getters
		std::string				getName();
		std::vector<Client *>	getClients();
		std::vector<Client *>	getOpClients();
};
