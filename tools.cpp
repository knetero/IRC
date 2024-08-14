#include "server.hpp"

std::string Server::getIp(struct sockaddr_in addr)
{
    return std::string(inet_ntoa(addr.sin_addr));
}

std::string Server::getServerIp(void)
{
    char            hostname[256];
    struct hostent  *host_entry;
    char            *ipAdress;

    gethostname(hostname, sizeof(hostname));
    host_entry = gethostbyname(hostname);
    ipAdress = inet_ntoa(*((struct in_addr *)host_entry->h_addr_list[0]));
    return (ipAdress);
}

void Server::getDate(char **buf)
{
	time_t now = time(0);
	struct tm tstruct;
		
	tstruct = *localtime(&now);
	if (*buf)
	{
		strftime(*buf, 100, "%a %b %d %Y at %X (+01)", &tstruct);	
	}
}

void Server::welcomeMessage(int fd, Client *client)
{
    sendData(fd, RPL_WELCOME(client->nickname, client->username, getIp(client->clientAdress)));
    sendData(fd, RPL_YOURHOST(client->nickname, getIp(client->clientAdress)));
    sendData(fd, RPL_CREATED(client->nickname, this->startdate));
}

std::vector<std::string> Server::splitParameters(std::string parametersString)
{
    std::vector<std::string>    parameters;
    std::istringstream          ss(parametersString);
    std::string                 parameter;
    std::string                 tmp;
    tmp = "";
    while (std::getline(ss, parameter, ' '))
    {
        if (parameter[0] == ':')
            tmp = parameter;
        else if (!tmp.empty())
        {
            tmp += " ";
            tmp += parameter;
        }
        else if (!parameter.empty())
            parameters.push_back(parameter);
    }
    if (!tmp.empty())
    {
        tmp = tmp.substr(1);
        parameters.push_back(tmp);
    }
    return (parameters);
}

int Server::getClientFd(std::string target)
{
    std::map<int, Client *>::iterator it;
    for (it = this->serverClients.begin(); it != this->serverClients.end(); it++)
    {
        if (it->second->nickname == target)
            return (it->first);
    }
    return (-1);
}

std::string Server::toUpperCase(std::string& str) {
    for (size_t i = 0; i < str.length(); ++i) {
        str[i] = std::toupper(static_cast<unsigned char>(str[i]));
    }
    return str;
}

void Server::sendData(int fd, std::string data)
{
    send(fd, data.c_str(), data.size(), 0);
}

int Server::channelExist(std::string channelName)
{
    if (channelName[0] == '#')
        channelName = channelName.substr(1);
    else
        return -1;
    if (this->server_channels.find(channelName) != this->server_channels.end())
    {
        return 1;
    }
    return -1;
}

std::string Server::to_str(size_t value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}

std::string Server::getHostName()
{
    char hostname[255];
    gethostname(hostname, sizeof(hostname));
    return (hostname);
}

void    Server::removeChannel(Channel *channel)
{
    std::map<std::string, Channel *>::iterator it;
    for (it = this->server_channels.begin(); it != this->server_channels.end(); it++)
    {
        if (it->second == channel)
        {
            delete it->second;
            server_channels.erase(it);
            break ;
        }
    }
}

void    Server::removeClientFromServer(Client *client)
{
    this->serverClients.erase(client->clientSocket); // remove client from the server map;
    for (size_t i = 0; i < this->clientSockets.size(); i++) // remove client from pollfds vector
    {
        if (clientSockets[i].fd == client->clientSocket)
            clientSockets.erase(clientSockets.begin() + i);
    }

    for (size_t i = 0; i < client->joinedChannels.size(); i++) // remove user from all the joined channels;
    {
        client->joinedChannels[i]->removeUser(client);
        client->joinedChannels[i]->setSize(client->joinedChannels[i]->getSize() - 1);
        if (client->joinedChannels[i]->getSize() == 0)
        {

            removeChannel(client->joinedChannels[i]);
        }
    }
    close(client->clientSocket); // close the client fd
    delete client;
}