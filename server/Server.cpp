#include "Server.hpp"
#include "utils.cpp"

Server::Server(uint16_t port) // add password later
{
	int backlog = 5; // for now 5 change later
	_port = port;
	_server_fd = createServerSocket();
	bindServerSocket();
	listenServerSocket(backlog);
}

Server::Server(uint16_t port, std::string password) : _port(port), _password(password)
{
	this->_is_running = false;

	this->_commands["CNOTICE"] = new Cnotice(this);
	this->_commands["CPRIVMSG"] = new Cprivmsg(this);
	this->_commands["INFO"] = new Info(this);
	this->_commands["INVITE"] = new Invite(this);
	this->_commands["JOIN"] = new Join(this);
	this->_commands["KICK"] = new Kick(this);
	this->_commands["LIST"] = new List(this);
	this->_commands["MODE"] = new Mode(this);
	this->_commands["NAMES"] = new Names(this);
	this->_commands["NICK"] = new Nick(this);
	this->_commands["NOTICE"] = new Notice(this);
	this->_commands["OPER"] = new Oper(this);
	this->_commands["PASS"] = new Pass(this);
	this->_commands["PRIVMSG"] = new Privmsg(this);
	this->_commands["QUIT"] = new Quit(this);
	this->_commands["SQUIT"] = new Squit(this);
	this->_commands["USER"] = new User(this);
	this->_commands["USERS"] = new Users(this);
}

/******************************************************************************/
/*                         Static Functions                                   */
/******************************************************************************/
// void Server::addClient(int client_fd)
// {
// 	Client tmp(client_fd);
// 	//The vector now owns its own copy; tmp will be destroyed after this line
// 	_clients.push_back(tmp);

// 	// Create and register this client's poll entry
// 	pollfd pollEntry;
// 	pollEntry.fd = client_fd;
// 	pollEntry.events = POLLIN;
// 	pollEntry.revents = 0;
// 	_poll_fds.push_back(pollEntry);
// }

// void Server::removeClient(int index)
// {
// 	// never remove server_fd
// 	if (index == 0)
// 		return;

// 	int clientIndex = index - 1; // fds[0] is server
// 	// Client &client = _clients[clientIndex];

// 	// close(client.getClient_fd());
// 	// remove the client from the vector
// 	_clients.erase(_clients.begin() + clientIndex);

// 	// remove the corresponding pollfd
// 	_poll_fds.erase((_poll_fds.begin() + index));
// }

void Server::onClientMessage(std::string message)
{
	Tokenizer	tokens(message);

	std::string command = tokens.get_command();
	if (this->_commands.find(command) == this->_commands.end())
		std::cout	<< "Error! Command not found." << std::endl;
	else
		this->_commands[command]->execute(&tokens);
}

// 0. make socket nonblocking
static int make_socket_nonblocking(int fd)
{
	//fcntl stands for file control int fcntl(int fd, int cmd, ... /* arg */);
	int current_flags = fcntl(fd, F_GETFL, 0);
	// get the current file status flags for this fd, returns an integer bitmask something like O_RDONLY | O_NONBLOCK | O_APPEND
	// std::cout << "current flags: " << current_flags << std::endl;
	return fcntl(fd, F_SETFL, current_flags | O_NONBLOCK); // add to the current flags the nonblocking flag
}

// static void closeClient(std::vector<pollfd>& fds, size_t i) // not used yet
// {
// 	std::cout << "Client " << fds[i].fd << " disconnected\n";
// 	close(fds[i].fd);
// 	fds.erase(fds.begin() + i);
// }

// Add a pollfd entry by file descriptor
static void addPollfd(std::vector<pollfd>& fds, int fd, short events)
{
	struct pollfd pfd;
	pfd.fd = fd;
	pfd.events = events;
	pfd.revents = 0;
	fds.push_back(pfd);
}

// Remove a pollfd entry by file descriptor
static void removePollfd(std::vector<pollfd>& fds, int fd)
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

// 2. creating serverAddress
sockaddr_in Server::createServerAddress()
{
	// this is how a sockaddrr_in looks like
	// struct sockaddr_in
	// {
	// 	sa_family_t sin_family; // Address family (always AF_INET for IPv4)
	// 	in_port_t sin_port; // Port number (must be in network byte order)
	// 	struct in_addr sin_addr; // IPv4 address (struct with a 32-bit value)
	// 	unsigned char sin_zero[8]; // Padding (not used)
	// };
	sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress)); // just nice to do not necessary
	// specifying the address
	serverAddress.sin_family = AF_INET; // format of ipaddress
	serverAddress.sin_port = htons(_port); // converts to network byte order
	serverAddress.sin_addr.s_addr = INADDR_ANY; // accept connections on any IP
	// inet_pton(AF_INET, "0.0.0.0", &serverAddress.sin_addr); // works similar to INADDR_ANY
	return serverAddress;
}

// 3. bind the socket to a port
void Server::bindServerSocket()
{
	sockaddr_in serverAddress = createServerAddress();
	int bindResult = bind(_server_fd, (struct sockaddr*)&serverAddress,
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

// 5. accept new clients
void Server::addClient(int client_fd)
{
	Client tmp(client_fd);
	//The vector now owns its own copy; tmp will be destroyed after this line
	_clients.push_back(tmp);

	std::cout << "New client connected (fd=" << client_fd << ")\n";
	// Create and register this client's poll entry
	addPollfd(_poll_fds, client_fd, POLLIN);
}

// 5. accept new clients
void Server::handleNewConnection()
{
	// Accept a new client
	sockaddr_in clientAddr;
	socklen_t len = sizeof(clientAddr);
	int client_fd = accept(_server_fd, (sockaddr*)&clientAddr, &len);

	// If no client is ready (non-blocking) or error occurred, just return
	if (client_fd < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return; // no client right now
		throw std::runtime_error(std::string("Accept failed: ") + strerror(errno));
	}
	make_socket_nonblocking(client_fd);
	addClient(client_fd);
}

// 5. during server run , can remove clients
void Server::removeClient(int index)
{
	// never remove server_fd
	if (index == 0 || index > static_cast<int>(_clients.size()))
		return;

	int clientIndex = index - 1; // fds[0] is server
	int client_fd = _clients[clientIndex].getClient_fd();

	close(client_fd);
	// remove the client object
	_clients.erase(_clients.begin() + clientIndex);

	// remove the corresponding pollfd
	removePollfd(_poll_fds, client_fd);
	std::cout << "Client disconnected (fd=" << client_fd << ")\n";
}

// void Server::handleClient(int index) // maybe add later again
// {
// 	_clients[index - 1].readData();
// }

/******************************************************************************/
/*                         Public Functions                                  */
/******************************************************************************/
void Server::run()
{
	// Add the server socket to the pollfd vector using the helper
	addPollfd(_poll_fds, _server_fd, POLLIN);
	while (true)
	{
		// int poll(struct pollfd *fds, nfds_t nfds, int timeout);
		// _poll_fds.data() -> array of pollfd structs representing all sockets
		// _poll_fds.size() -> how many pollfd structs it should check
		// int timeout -> timeout in milliseconds / -1 means "wait indefinitely" until at least one fd becomes ready
		int ready = poll(_poll_fds.data(), _poll_fds.size(), -1);
		if (ready == -1)
			throw std::runtime_error("Poll failed: " + std::string(strerror(errno)));
		for (size_t i = 0; i < _poll_fds.size(); i++)
		{
			struct pollfd& curPollEntry = _poll_fds[i];
			// Case 1: New client connection
			if (curPollEntry.fd == _server_fd && (curPollEntry.revents & POLLIN))
			{
				handleNewConnection();
				continue;
			}
			// revents check if any revents occurred and pollin signals connection ready to accept, both together means connection is ready or there is fd which has data to read

			// Case 2: Ignore server socket entries here
			if (curPollEntry.fd == _server_fd)
				continue;
			size_t clientIndex = i - 1;
			Client& curClient = _clients[clientIndex];

			// Case 3: Client disconnected or error
			if (curPollEntry.revents & (POLLHUP | POLLERR | POLLNVAL))
			{
				std::cout << "Client fd=" << curClient.getClient_fd() << " disconnected/error\n";
				removeClient(clientIndex);
				removePollfd(_poll_fds, curPollEntry.fd);
				i--;
				continue;
			}
			// Case 4: Client sent data
			if (curPollEntry.revents & POLLIN)
			{
				size_t clientIndex = i - 1;
				Client& curClient = _clients[clientIndex];
				bool stillConnected = curClient.readData();
				if (!stillConnected)
				{
					std::cout << "Client fd=" << curClient.getClient_fd() << " disconnected\n";
					removeClient(clientIndex);
					removePollfd(_poll_fds, curPollEntry.fd);
					i--;
				}
			}
		}
	}
}

int main(int ac, char **av)
{
	(void)ac;
	Server	server(200, "kaka");
	server.onClientMessage(av[1]);
	return (0);
}