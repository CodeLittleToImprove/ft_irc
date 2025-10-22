#include "Client.hpp"

Client::Client(int client_fd) : _client_fd(client_fd), _connected(true), _has_nickname(false) {}

Client::~Client()
{
	if (_connected == false)
		close(_client_fd); // is this really needed?
}

bool Client::readData()
{
	char recvBuf[512];
	int bytesReceived = recv(_client_fd, recvBuf, sizeof(recvBuf) - 1, 0);

	if (bytesReceived > 0)
	{
		recvBuf[bytesReceived] = '\0';
		_buffer += recvBuf;
		// printEscapedBuffer(buffer);
		size_t end;
		while ((end = _buffer.find("\r\n")) != std::string::npos)
		{
			std::string msg = _buffer.substr(0, end);
			_buffer.erase(0, end + 2);
			std::cout << "Message from " << _client_fd << ": " << msg << "\n";
			// handleIRCCommand(message);  // parse and respond this should be done somewhere else
		}
		return true;
	}
	else if (bytesReceived == 0)
	{
		// Client closed connection gracefully
		std::cout << "Client " << _client_fd << " closed connection." << std::endl;
		_connected = false; _connected = false; // maybe optional
		return false;
	}
	if (errno == EAGAIN || errno == EWOULDBLOCK)
	{
		// No data available now, just return true; connection is still alive
		return true;
	}
	else
	{
		// Real error
		std::cout << "Recv error on client " << _client_fd << ": " << strerror(errno) << std::endl;
		_connected = false; // maybe optional
		return false;
	}
}

void Client::closeConnection()
{
	if (_connected)
	{
		std::cout << "Closing client " << _client_fd << std::endl;
		close(_client_fd);
		_connected = false;
	}
}

int Client::getClient_fd() const
{
	return _client_fd;
}

bool Client::getConnectedStatus() const
{
	return _connected;
}

std::string Client::getNickname() const
{
	return _nickname;
}

void Client::setNickname(std::string nickname)
{
	this->_nickname = nickname;
	this->_has_nickname = true;
}