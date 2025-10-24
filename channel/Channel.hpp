/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phillymilly <phillymilly@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 15:21:47 by phillymilly       #+#    #+#             */
/*   Updated: 2025/10/23 18:43:20 by phillymilly      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include "../server/Client.hpp"

class Channel
{
	private:
		std::string				_name;
		std::string				_hostname;
		std::string				_topic;
		std::vector<Client *>	_clients;
	public:
		Channel();
		~Channel();
};
