#include "Client.hpp"

Client::Client(int client_fd)
	: _client_fd(client_fd), _connected(true)
{
}

Client::~Client()
{
	if (_connected == false)
		close(_client_fd); // is this really needed?
}

void Client::readData()
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
	}
	else if (bytesReceived == 0)
	{
		std::cout << "Client " << _client_fd << " closed connection." << std::endl;
		_connected = false;
	}
	else // this case is for do nothing and wait for data
	{
		std::cout << "Recv error on client " << _client_fd << ": " << strerror(errno) << std::endl;
		_connected = false;
	}
}

bool Client::getConnectedStatus() const
{
	return _connected;
}

std::string Client::getNickname() const
{
	return _nickName;
}
