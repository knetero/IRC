#include "server.hpp"

bool nickAlreadyInUse(std::string nickname, std::map<int, Client>& allClients, int fd)
{
    std::map<int, Client>::iterator it;
    for (it = allClients.begin(); it != allClients.end(); it++)
    {
        if (it->second.nickname == nickname && it->second.get_client_socket() != fd)
            return (true);
    }
    return (false);
}

bool isValidNick(std::string nickname)
{
    if (nickname[0] == '#' || nickname[0] == ':')
        return (false);
    // if (isdigit(nickname[0]))
    //     return (false);
    // if (nickname.find(" "))
    //     return (false);
    return true;
}

void Server::broadcastToChannels(int fd, std::string nickname, Client &c)
{
    std::cout << c.joinedChannels.size() << std::endl;
    for (size_t i = 0; i < c.joinedChannels.size(); i++)
    {
        std::map<int, Client >::iterator it;
        for (it = c.joinedChannels[i].getmembers()->begin(); it != c.joinedChannels[i].getmembers()->end(); it++)
        {
            sendData(it->second.get_client_socket(),\
            NICK(c.nickname, c.username, getIp(adresses[fd]), nickname));
        }
    }
}

void Server::nickCommand(int fd, std::map<int, Client>& allClients, std::string param)
{
    if (!param.empty())
    {
        if (allClients[fd].passSet)
        {
            if (nickAlreadyInUse(param, allClients, fd)) // check if the username is already used.
                sendData(fd, ERR_NICKINUSE(param));
            else if (allClients[fd].nickname == param) // check if it is already my nickname
                return ;
            else if (!isValidNick(param)) // check the synthax of the nickname
                sendData(fd, ERR_ERRONEUSNICKNAME(param));
            else if (allClients[fd].isRegistered) // check if already registred
            {
                if (allClients[fd].nickname != param) // client wants to change the nick
                {
                    broadcastToChannels(fd, param, allClients[fd]);
                    allClients[fd].nickname = param;
                }
            }
            else
            {
                allClients[fd].nickname = param;
                allClients[fd].nickSet = true;
                if (allClients[fd].nickSet && allClients[fd].userSet)
                {
                    allClients[fd].isRegistered = true;
                    welcomeMessage(fd, allClients[fd]);
                }
            }
        }
        else
        {
            sendData(fd, ERROR(std::string("access denied: bad password")));
        }
    }
    else
        sendData(fd, ERR_NONICKNAMEGIVEN);
}