#include "server.hpp"

std::string getRealName(std::vector<std::string> params)
{
    std::string realName;
    for (size_t i = 3; i < params.size(); i++)
    {
        realName += params.at(i);
        if (i + 1 == params.size())
            break ;
        realName += " ";
    }
    realName = realName.substr(1);
    return realName;
}

void Server::userCommand(int fd, std::map<int, Client>& allClients, std::string param)
{
    if (!param.empty())
    {
        if (allClients[fd].passSet)
        {
            if (allClients[fd].userSet)
            {
                sendData(fd, ERR_ALREADYREGISTERED(allClients[fd].nickname));
                return ;
            }
            std::istringstream ss(param);
            std::vector<std::string> params;
            std::string p;
            while (std::getline(ss, p, ' '))
                params.push_back(p);
            if (params.size() < 4)
            {
                sendData(fd, ERR_NOTENOUGHPARAM(allClients[fd].nickname));
                return ;
            }
            allClients[fd].username = params[0];
            if (params[3][0] != ':')
                allClients[fd].realname = params[3];
            else
                allClients[fd].realname = getRealName(params);
            allClients[fd].userSet = true;
            if (allClients[fd].userSet && allClients[fd].nickSet)
            {
                allClients[fd].isRegistered = true;
                welcomeMessage(fd, allClients[fd]);
            }
        }
        else
        {
            sendData(fd, ERROR(std::string("access denied: bad password")));
        }
    }
    else
        sendData(fd, ERR_NOTENOUGHPARAM(std::string("test")));
}