#include "server.hpp"

std::string getComment(std::vector<std::string> parameters)
{
    std::string comment;
    for (size_t i = 3; i < parameters.size(); i++)
    {
        comment += parameters[i];
        if (i + 1 != parameters.size())
            comment += " ";
    }
    comment = comment.substr(1);
    return (comment);
}

void Server::kickCommand(int fd, std::map<int, Client> &allClients, std::string params)
{
    if (!allClients[fd].isRegistered)
    {
        std::istringstream ss(params);
        std::string        p;
        std::vector<std::string> parameters;
        while (std::getline(ss, p, ' '))
            parameters.push_back(p);
        if (parameters.size() < 3)
            sendData(fd, ERR_NOTENOUGHPARAM(allClients[fd].nickname));
        else if (parameters.size() > 3 && parameters[3][0] != ':')
            sendData(fd, ERR_NOTENOUGHPARAM(allClients[fd].nickname));
        std::string comment = "";
        if (parameters.size() > 3)
            comment = getComment(parameters);
        
    }
    else
        sendData(fd, ERR_NOTREGISTERED(allClients[fd].nickname));
}