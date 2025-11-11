/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pschmunk <pschmunk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 20:16:04 by pschmunk          #+#    #+#             */
/*   Updated: 2025/11/10 16:05:17 by pschmunk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Server.hpp"

#include "../utils/utils.hpp"

/******************************************************************************/
/*                             Constructors                                   */
/******************************************************************************/

Server::Server(uint16_t port, std::string password) : _port(port), _password(password), _oper_password("1234")
{
	int backlog = 5; // for now 5 change later
	_server_fd = createServerSocket();
	bindServerSocket();
	listenServerSocket(backlog);
	this->_is_running = false;
	this->_hostname = "127.0.0.1";
	// this->_commands["INFO"] 	= new Info(this);
	this->_commands["CAP"] = new Cap(this);
	this->_commands["INVITE"]	= new Invite(this);
	this->_commands["JOIN"]		= new Join(this);
	this->_commands["KICK"] 	= new Kick(this);
	// this->_commands["LIST"] 	= new List(this);
	this->_commands["MODE"] 	= new Mode(this);
	// this->_commands["NAMES"] 	= new Names(this);
	this->_commands["NICK"] 	= new Nick(this);
	this->_commands["NOTICE"] 	= new Notice(this);
	this->_commands["OPER"] 	= new Oper(this);
	this->_commands["PART"] 	= new Part(this);
	this->_commands["PASS"] 	= new Pass(this);
	this->_commands["PING"] 	= new Ping(this);
	this->_commands["PRIVMSG"] 	= new Privmsg(this);
	this->_commands["QUIT"] 	= new Quit(this);
	this->_commands["TOPIC"]	= new Topic(this); 
	// this->_commands["SQUIT"] 	= new Squit(this); -- not needed
	this->_commands["USER"] 	= new User(this);
	// this->_commands["USERS"] 	= new Users(this);
	this->_commands["WHO"] 		= new Who(this);
}

Server::~Server()
{
	for (std::map<int, Client *>::iterator client_it = _clients.begin(); client_it != _clients.end(); client_it++)
		delete client_it->second;
	_clients.clear();

	for (std::map<std::string, ACommand *>::iterator command_it = _commands.begin(); command_it != _commands.end(); command_it++)
		delete command_it->second;
	_commands.clear();

	for (std::vector<Channel *>::iterator channel_it = _channels.begin(); channel_it != _channels.end(); channel_it++)
		delete *channel_it;
	_channels.clear();

	// Close all client sockets first
	for (size_t i = 1; i < _poll_fds.size(); ++i)
	{
		if (_poll_fds[i].fd >= 0)
			close(_poll_fds[i].fd);
	}
	// Close server socket last
	if (_server_fd >= 0)
		close(_server_fd);
	std::cout << "All sockets closed. Server stopped." << std::endl;

}


/******************************************************************************/
/*                         Static Functions                                   */
/******************************************************************************/

// 0. make socket nonblocking
static int make_socket_nonblocking(int fd)
{
	//fcntl stands for file control int fcntl(int fd, int cmd, ... /* arg */);
	return fcntl(fd, F_SETFL, O_NONBLOCK); // add to the current flags the nonblocking flag
}

// Add a pollfd entry by file descriptor
static void addPollfd(std::vector<pollfd> &fds, int fd, short events)
{
	struct pollfd pfd;
	pfd.fd = fd;
	pfd.events = events;
	pfd.revents = 0;
	fds.push_back(pfd);
}

// Remove a pollfd entry by file descriptor
static void removePollfd(std::vector<pollfd> &fds, int fd)
{
	for (std::vector<pollfd>::iterator it = fds.begin(); it != fds.end(); ++it)
	{
		if (it->fd == fd)
		{
			fds.erase(it);
			break;
		}
	}
}

/******************************************************************************/
/*                         Private Functions                                  */
/******************************************************************************/

// 1. creating server socket
int Server::createServerSocket()
{
	int server_fd = socket(AF_INET, SOCK_STREAM, 0); // input: IPv4 protocol, TCP socket, default , also returns a fd
	if (server_fd == -1)
		throw std::runtime_error("Socket creation failed");
	int opt = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	// reuse port when server gets started to fast
	{
		close(server_fd);
		throw std::runtime_error("setsockopt failed: " + std::string(strerror(errno)));
	}
	make_socket_nonblocking(server_fd);
	return server_fd;
}

// this is how a sockaddrr_in looks like
// struct sockaddr_in
// {
// 	sa_family_t sin_family; // Address family (always AF_INET for IPv4)
// 	in_port_t sin_port; // Port number (must be in network byte order)
// 	struct in_addr sin_addr; // IPv4 address (struct with a 32-bit value)
// 	unsigned char sin_zero[8]; // Padding (not used)
// };
// 2. creating serverAddress
sockaddr_in Server::createServerAddress()
{
	sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress)); // just nice to do not necessary
	// specifying the address
	serverAddress.sin_family = AF_INET; // format of ipaddress
	serverAddress.sin_port = htons(_port); // converts to network byte order
	serverAddress.sin_addr.s_addr = INADDR_ANY; // accept connections on any IP
	return serverAddress;
}

// 3. bind the socket to a port
void Server::bindServerSocket()
{
	sockaddr_in serverAddress = createServerAddress();
	int bindResult = bind(_server_fd, (struct sockaddr *) &serverAddress,
		sizeof(serverAddress));
	if (bindResult == -1)
		throw std::runtime_error(std::string("Bind failed: ") + std::strerror(errno));
}

// 4. listen for incoming connections
void Server::listenServerSocket(size_t backlog)
{
	// listening to the assigned socket
	int listenResult = listen(_server_fd, backlog); // n = how many pending connections can be queued
	if (listenResult == -1)
		throw std::runtime_error(std::string("Listen failed: ") + std::strerror(errno));
}

// 5. helper function accept new clients
void Server::addClient(int client_fd)
{
	try
	{
		Client *client = new Client(client_fd, _password);
		std::pair<std::map<int, Client *>::iterator, bool> insertSuccess;
		insertSuccess = _clients.insert(std::make_pair(client_fd, client));

		if (!insertSuccess.second)
		{
			std::cout << "[DEBUG] Warning: client with fd " << client_fd << " already exists!" << std::endl;
			delete client;
			return;
		}
		std::cout << "[DEBUG]: New client connected (fd=" << client_fd << ")\n";
		// Create and register this client's poll entry
		addPollfd(_poll_fds, client_fd, POLLIN);
	}
	catch (const std::exception &e)
	{
		if (client_fd >= 0)
			close (client_fd);
		std::cerr << "Failed to add client fd=" << client_fd << ": " << e.what() << std::endl;
	}
}

// 5. accept new clients
void Server::handleNewConnection()
{
	sockaddr_in clientAddr;
	socklen_t len = sizeof(clientAddr);
	int client_fd = accept(_server_fd, (sockaddr *) &clientAddr, &len);

	if (client_fd < 0)
		throw std::runtime_error(std::string("Accept failed: ") + strerror(errno));
	make_socket_nonblocking(client_fd);
	addClient(client_fd);
}

// 5. during server run, can remove clients
void Server::removeClient(int client_fd)
{
	std::map<int, Client *>::iterator it = _clients.find(client_fd);
	if (it == _clients.end())
		return;

	// remove the corresponding pollfd
	removePollfd(_poll_fds, client_fd);

	if (close(client_fd) == -1)
		std::cout << "[DEBUG]Warning: failed to close fd " << client_fd << ": " << strerror(errno) << std::endl;
	delete it->second; // free the client object
	_clients.erase(it);
	std::cout << "[DEBUG] Client disconnected (fd=" << client_fd << ")\n";
}

void Server::handleAdminInput()
{
	char buffer[256];
	int bytes = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);
	if (bytes <= 0)
		return;

	buffer[bytes] = '\0';
	std::string input(buffer);

	if (input == "exit\n")
	{
		std::cout << "Shutting down server..." << std::endl;
		_is_running = false;
	}
	else
		std::cout << "Unknown command: " << input;
}

/******************************************************************************/
/*                          Public Functions                                  */
/******************************************************************************/

Client *Server::get_client(int client_fd)
{
	std::map<int, Client *>::iterator it = _clients.find(client_fd);
	if (it == _clients.end())
	{
		throw std::runtime_error("Client not found");
	}
	return it->second;
}

Client *Server::get_client(std::string nickname)
{
	std::map<int, Client*>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); it++)
	{
		Client *current_client = it->second;
		if (current_client->getNickname() == nickname)
			return (current_client);
	}
	return (NULL);
}

Channel *Server::get_channel(std::string name)
{
	for (size_t i = 0; i < _channels.size(); i++)
	{
		if (_channels.at(i)->getName() == name)
			return (_channels.at(i));
	}
	return (NULL);
}

std::string Server::get_hostname()
{
	return (this->_hostname);
}

std::string	Server::getOperPassword()
{
	return (this->_oper_password);
}

std::vector<Channel*> Server::getJoinedChannelsByClient(Client *client)
{
	std::vector<Channel*> result;
	for (std::vector<Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++)
	{
		Channel *channel = *it;
		if (channel->isInChannel(client))
		{
			// std::cout << "[DEBUG]: "<< client->getUsername() << " is in " << channel->getName() << std::endl;
			result.push_back(channel);
		}
	}
	return (result);
}

void Server::onClientMessage(int client_fd, std::string message)
{

	Tokenizer tokens(message);
	Client *client = get_client(client_fd);
	if (!client) // client not existing anymore
		return;

	std::string command = tokens.get_command();
	std::cout << "[DEBUG]: command in onClient: " << message << std::endl;
	if (this->_commands.find(command) == this->_commands.end())
		std::cout << "[DEBUG] Error! Command not found." << std::endl;
	else
		this->_commands[command]->execute(client, &tokens);
}

void Server::response(Client *client, std::string code, std::string message) // server - to client communication
{
	if (!client)
		return;

	std::string code_str = code.empty() ? "" : code + ' ';
	std::string nickname = client->getNickname();
	std::string nickname_str = nickname.empty() ? "unregistered " : nickname + ' ';
	std::string response = ':' + this->_hostname + ' ' + code_str + nickname_str + message + CRLF;
	// printEscapedBuffer(response);
	send(client->getClient_fd(), response.c_str(), response.length(), 0);
}

void Server::sendRaw(Client *client, const std::string &raw)
{
	send(client->getClient_fd(), raw.c_str(), raw.length(), 0);
}

void Server::add_channel(Channel *channel)
{
	this->_channels.push_back(channel);
}

void Server::remove_channel(const std::string &channel_name)
{
	std::vector<Channel*>::iterator it = _channels.begin();

	while (it != _channels.end())
	{
		Channel *channel = *it;
		if (channel && channel->getName() == channel_name)
		{
			std::cout << "[DEBUG][Server] Removing empty channel: " << channel_name << std::endl;
			delete channel;
			_channels.erase(it);
			return;
		}
		else
			++it;
	}
	std::cout << "[DEBUG][Server] Tried to remove non-existent channel: " << channel_name << std::endl;
}

void Server::removeIfDisconnected(Client *client, int client_fd, size_t &i, const std::string &context)
{
	// std::cout << "[DEBUG] removeIfDisconnected called for fd=" << client_fd << std::endl;
	if (!client->getConnectedStatus())
	{
		for (std::vector<Channel*>::iterator it = _channels.begin(); it != _channels.end();)
		{
			Channel* channel = *it;
			if (channel->isInChannel(client))
			{
				channel->broadcast(client, "QUIT", "", ":Client disconnected");
				channel->removeClient(client);
				if (channel->isEmpty())
				{
					std::cout << "[DEBUG][Server] Removing empty channel: " << channel->getName() << std::endl;
					delete channel;
					it = _channels.erase(it); // points the next valid iterator
					continue; // skip ++it
				}
			}
			++i; // default case when not deleting
		}
		this->removeClient(client_fd);
		i--;
	}
}

void Server::handleClientEvent(pollfd &entry, size_t &i)
{
	int client_fd = entry.fd;
	std::map<int, Client *>::iterator it = _clients.find(entry.fd);
	if (it == _clients.end())
		return;
	Client *curClient = it->second;

	// Disconnected or error event (not in my control)
	if (entry.revents & (POLLHUP | POLLERR | POLLNVAL))
	{
		std::cout << "[DEBUG] Client fd=" << curClient->getClient_fd() << " disconnected/error\n";
		removeClient(client_fd);
		i--;
		return;
	}
	// client has sent data
	if (entry.revents & POLLIN)
	{
		std::vector<std::string> messages = curClient->readData();
		removeIfDisconnected(curClient, client_fd, i, "after read");
		if (_clients.find(client_fd) == _clients.end()) // no such client, probably disconnected
			return;

		for (size_t i = 0; i < messages.size(); i++)
			onClientMessage(client_fd, messages[i]);
	}
}

void Server::handlePollEvents()
{
	// Iterate through all file descriptors we’re monitoring
	for (size_t i = 0; i < _poll_fds.size(); i++)
	{
		pollfd &curPollEntry = _poll_fds[i];
		// --- Case 0: Handle STDIN (admin commands like "exit") ---
		if (curPollEntry.fd == STDIN_FILENO && (curPollEntry.revents & POLLIN))
		{
			handleAdminInput();
			if (!_is_running)
				break;
			continue;
		}
		// Case 1: New client connection
		// revents check if any revents occurred and pollin signals connection ready to accept, both together means connection is ready or there is fd which has data to read
		if (curPollEntry.fd == _server_fd && (curPollEntry.revents & POLLIN))
		{
			handleNewConnection();
			continue;
		}
		// Case 2: Ignore server socket entries here
		if (curPollEntry.fd == _server_fd)
			continue;
		// Case 3: Current client has events
		handleClientEvent(curPollEntry, i);
	}
}

void Server::removeClientFromServer(Client *client)
{
	removeClient(client->getClient_fd());
}

void Server::run()
{
	std::cout << "[DEBUG] Server started" << std::endl;
	// Add the server socket to the pollfd vector using the helper
	addPollfd(_poll_fds, _server_fd, POLLIN);
	// Add stdin ot the pollfd vector
	addPollfd(_poll_fds, STDIN_FILENO, POLLIN);
	this->_is_running = true;
	while (_is_running)
	{
		// int poll(struct pollfd *fds, nfds_t nfds, int timeout);
		// _poll_fds.data() -> array of pollfd structs representing all sockets
		// _poll_fds.size() -> how many pollfd structs it should check
		// int timeout -> timeout in milliseconds / -1 means "wait indefinitely" until at least one fd becomes ready
		int ready = poll(_poll_fds.data(), _poll_fds.size(), -1);
		if (ready == -1)
		{
			if (errno == EINTR)
				continue;
			throw std::runtime_error("Poll failed: " + std::string(strerror(errno)));
		}
		handlePollEvents();
	}
}
