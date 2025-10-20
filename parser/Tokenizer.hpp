/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Tokenizer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/14 16:28:54 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/20 18:32:46 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <vector>
# include <iterator>
# include <algorithm>

class Tokenizer
{
	private:
		std::string					_prefix;
		std::string					_command;
		std::vector<std::string>	_params;
	public:
		Tokenizer(std::string message);
		Tokenizer	&operator=(const Tokenizer &copy);

		std::string					get_prefix();
		std::string					get_command();
		std::vector<std::string>	get_params();
		std::string					get_param(size_t pos);
};
