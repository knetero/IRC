#include "server.hpp"

void    Server::inviteCommand(Client *client, std::vector<std::string> &parameters)
{
    if (client->isRegistered)
    {
        if (parameters.size() == 3)
        {
            std::cout << server_channels.size() << std::endl;
            if (channelExist(parameters[2]) == 1)
            {
                Channel *channel = server_channels[parameters[2].substr(1)];
                if (channel->clientExist(client->nickname) == 1)
                {
                    std::cout << "invite" << channel->inviteonly << std::endl;
                    if ((channel->inviteonly && channel->getoperators()[client->clientSocket] == client) || channel->inviteonly == false)
                    {
                        if (channel->clientExist(parameters[1]) != 1)
                        {
                            if (getClientFd(parameters[1]) != -1)
                            {
                                Client *toInvite = this->serverClients[getClientFd(parameters[1])];
                                channel->add_user(toInvite, -1);
                                sendData(client->clientSocket, RPL_INVITING(client->nickname, parameters[1], parameters[2]));
                                sendData(getClientFd(parameters[1]),\
                                INVITE(client->nickname, client->username, getIp(client->clientAdress), parameters[1], parameters[2]));
                            }
                            else
                                sendData(client->clientSocket, ERR_NOSUCHNICK(client->nickname, parameters[1]));
                        }
                        else
                            sendData(client->clientSocket, ERR_USERONCHANNEL(client->nickname, parameters[1], parameters[2]));
                    }
                    else
                        sendData(client->clientSocket, ERR_CHANOPRIVSNEEDED(client->nickname, parameters[2]));
                }
                else
                    sendData(client->clientSocket, ERR_NOTONCHANNEL(client->nickname, parameters[2]));
            }
            else
                sendData(client->clientSocket, ERR_NOSUCHCHANNEL(client->nickname, parameters[2]));
        }
        else
            sendData(client->clientSocket, ERR_NEEDMOREPARAMS(client->nickname, "INVITE"));
    }
    else
        sendData(client->clientSocket, ERR_NOTREGISTERED(client->nickname));
}