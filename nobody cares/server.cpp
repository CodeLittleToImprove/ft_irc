/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tbui-quo <tbui-quo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 16:11:14 by tbui-quo          #+#    #+#             */
/*   Updated: 2025/04/24 17:44:30 by tbui-quo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "Template.hpp"

#include <iostream>
#include <string.h> // for strcpy
#include <sys/socket.h> // works without but its for portability
#include <stdlib.h> // for exit
#include <netinet/in.h> // for htons
#include <unistd.h> // for close
#include <arpa/inet.h> // for inet_ntoa


int main(void)
{
	int listen_fd;
	int server_fd;
	int portNum = 1500;
	bool isExit = false;
	int bufsize = 1024;
	char buf[bufsize];

	struct sockaddr_in serverAddr; // could be zero initialized before used
	// memset(&serverAddr, 0, sizeof(serverAddr));
	socklen_t size;

	// init socket
	listen_fd = socket(AF_INET, SOCK_STREAM, 0); //af_inet means IPv4, sockstream means tcp, 0 default
	if (listen_fd < 0)
	{
		std::cout << "Error establishing connection" << std::endl;
		exit(1);
	}
	std::cout << "Server Socket connection created" << std::endl;
	serverAddr.sin_family = AF_INET; // Set address family to IPv4.
	serverAddr.sin_addr.s_addr = htons(INADDR_ANY); //  Converts the unsigned integer hostlong from host byte order to network byte order (for 32-bit ip.
	// Tell the kernel that this server socket should listen on any network interface available
	// INADDR_ANY represents all available local interfaces equals to wildcard address 0.0.0.0
	serverAddr.sin_port = htons(portNum); // Convert host-endian port (portNum) to network byte order and store

	// binding socket
	// bind() associates the socket with the local address and port specified in
	// bind expects a pointer to a generic address structure
	if (bind(listen_fd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
	{
		std::cout << "Error binding socket." << std::endl;
		exit(1);
	}
	size = sizeof(serverAddr);
	std::cout << "Looking for clients" << std::endl;
	listen(listen_fd, 1); // puts the socket into passive listening mode with a backlog (queue) of 1 pending connection.


	int clientCount = 1;
	server_fd = accept(listen_fd, (struct sockaddr*)&serverAddr, &size); // accept() blocks until a client connects;

	// first check if it is valid or not
	if (server_fd < 0)
		std::cout << "=> Error on accepting..." << std::endl;

	while (server_fd > 0) // means here is an accepted connection
	{
		strcpy(buf, "=> Server connected...\n");
		send(server_fd, buf, bufsize, 0); // sends message to client
		std::cout << "=> Connected with the client #" << clientCount << ", you are good to go..." << std::endl;
		std::cout << "\n=> Enter # to end the connection\n" << std::endl;
		std::cout << "Client: ";
		do
		{
			recv(server_fd, buf, bufsize, 0);
			std::cout << buf << " ";
			if (*buf == '#')
			{
				*buf = '*';
				isExit = true;
			}
		}
		while (*buf != '*'); // terminator symbol

		do
		{
			std::cout << "\nServer: ";
			do
			{
				std::cin >> buf;
				send(server_fd, buf, bufsize, 0);
				if (*buf == '#')
				{
					send(server_fd, buf, bufsize, 0);
					*buf = '*';
					isExit = true;
				}
			}
			while (*buf != '*');

			std::cout << "Client: ";
			do
			{
				recv(server_fd, buf, bufsize, 0); // reads message which comes out of the pipe
				std::cout << buf << " ";
				if (*buf == '#')
				{
					*buf = '*';
					isExit = true;
				}
			}
			while (*buf != '*');
		}
		while (!isExit);
		std::cout << "\n\n=> Connection terminated with IP " << inet_ntoa(serverAddr.sin_addr);
		close(server_fd);
		std::cout << "\nGoodbye..." << std::endl;
		isExit = false;
		exit(1);
	}

	close(listen_fd);
	return 0;
}
