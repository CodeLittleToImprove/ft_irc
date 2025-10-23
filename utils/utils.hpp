/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/15 18:44:05 by pschmunk          #+#    #+#             */
/*   Updated: 2025/10/21 19:05:53 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <vector>
# include <string>
# include <sstream>
#include <iomanip> // for setw and setfill

std::vector<std::string> split(const std::string &str);
void printEscapedBuffer(const std::string &buffer);
