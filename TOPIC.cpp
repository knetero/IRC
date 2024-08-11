#include "server.hpp"


void Server::broadcastTopic(Channel *channel, Client *client, std::string topic)
{
    std::map<int, Client *>::iterator it;
    for (it = channel->getmembers().begin(); it != channel->getmembers().end(); it++)
    {
        sendData(it->first, TOPIC(client->nickname, client->username, getIp(client->clientAdress), channel->getName(), topic));
    }
}

void Server::topicCommand(Client *client, std::vector<std::string> &parameters)
{
    if (client->isRegistered)
    {
        if (parameters.size() >= 2)
        {
            if (server_channels.find(parameters[1].substr(1)) != server_channels.end())
            {
                Channel *channel = server_channels.find(parameters[1].substr(1))->second;
                if (channel->clientExist(client->nickname) != -1)
                {
                    if (parameters.size() == 2) // view the topic
                    {
                        if (channel->hasTopic)
                            sendData(client->clientSocket, RPL_TOPIC(client->nickname, parameters[1], channel->gettopic()));
                        else
                            sendData(client->clientSocket, RPL_NOTOPIC(client->nickname, parameters[1]));
                    }
                    else if (parameters.size() == 3) // modify topic
                    {
                        if ((channel->protectedTopic && channel->getoperators()[client->clientSocket] == client) || !channel->protectedTopic)
                        {
                            channel->gettopic() = parameters[2];
                            channel->hasTopic = true;
                            broadcastTopic(channel, client, parameters[2]);
                        }
                        else
                            sendData(client->clientSocket, ERR_CHANOPRIVSNEEDED(client->nickname, parameters[1]));
                    }
                }
                else
                    sendData(client->clientSocket, ERR_NOTONCHANNEL(client->nickname, parameters[1]));
            }
            else
                sendData(client->clientSocket, ERR_NOSUCHCHANNEL(client->nickname, parameters[1], getIp(client->clientAdress)));
        }
        else
            sendData(client->clientSocket, ERR_NEEDMOREPARAMS(client->nickname, "TOPIC"));
    }
    else
        sendData(client->clientSocket, ERR_NOTREGISTERED(client->nickname));
}