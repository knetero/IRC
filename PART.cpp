#include "server.hpp"

void Server::broadcastPart(Client *client, Channel *channel, std::string message)
{
    if (!message.empty())
        message = ":" + message;
    std::map<int, Client *>::iterator it;
    for (it = channel->getmembers().begin(); it != channel->getmembers().end(); it++)
    {
        Client *c = it->second;
        sendData(c->clientSocket, PART(client->nickname, client->username, getIp(client->clientAdress), channel->getName(), message));
    }
}

void                        Server::part(Client *client, std::vector<std::string> &parameters)
{
    if (client->isRegistered)
    {
        if (parameters.size() > 1)
        {
            std::istringstream          ss(parameters[1]);
            std::string                 target;
            std::vector<std::string>    targets;
            while (std::getline(ss, target, ','))
                targets.push_back(target);
            for (size_t i = 0; i < targets.size(); i++)
            {
                if (channelExist(targets[i]) == 1)
                {
                    Channel *channel = this->server_channels.find(targets[i].substr(1))->second;
                    if (channel->clientExist(client->nickname) == 1)
                    {
                        broadcastPart(client, channel, parameters[2]);
                        channel->removeUser(client);
                        channel->setSize(channel->getSize() - 1);
                        if (channel->getSize() == 0)
                            removeChannel(channel);
                    }
                    else
                        sendData(client->clientSocket, ERR_NOTONCHANNEL(client->nickname, targets[i]));
                }
                else
                    sendData(client->clientSocket, ERR_NOSUCHCHANNEL(client->nickname, targets[i]));
            }
        }
        else
            sendData(client->clientSocket, ERR_NEEDMOREPARAMS(client->nickname, "PART"));
    }
    else
        sendData(client->clientSocket, ERR_NOTREGISTERED(client->nickname));
}
