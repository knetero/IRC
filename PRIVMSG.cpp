#include "server.hpp"

void Server::privmsgCommand(Client *client, std::vector<std::string> &parameters)
{
    if (client->isRegistered)
    {
        if (parameters.size() == 3)
        {
            std::vector<std::string>    targets;
            std::istringstream          ss(parameters[1]);
            std::string                 target;
            while (getline(ss, target, ','))
                targets.push_back(target);
            for (int i = 0; i < (int)targets.size(); i++)
            {
                if (targets[i][0] == '#')
                {
                    if (server_channels.find(targets[i].substr(1)) == server_channels.end())
                    {
                        sendData(client->clientSocket, ERR_NOSUCHCHANNEL(client->nickname, targets[i]));
                        return ;
                    }
                    else
                    {
                        Channel *channel = server_channels[targets[i].substr(1)];
                        if (channel->clientExist(client->nickname) != -1)
                        {
                            std::map<int, Client *> members = channel->getmembers();
                            std::map<int, Client *>::iterator it;
                            for (it = members.begin(); it != members.end(); it++)
                            {
                                if (it->second->get_client_socket() != client->get_client_socket())
                                    sendData(it->second->get_client_socket(), \
                                    PRIVMSG(client->nickname, client->username, getIp(client->clientAdress), targets[i], parameters[2]));
                            }
                        }
                        else
                            sendData(client->get_client_socket(), ERR_CANNOTSENDTOCHAN(client->nickname, targets[i]));
                    }
                }
                else
                {
                    if (serverClients.find(getClientFd(parameters[1])) != serverClients.end())
                    {
                        sendData(getClientFd(parameters[1]), \
                        PRIVMSG(client->nickname, client->username, getIp(client->clientAdress), targets[i], parameters[2]));
                    }
                    else
                        sendData(client->clientSocket, ERR_NOSUCHNICK(client->nickname, parameters[1]));
                }
            }
        }
        else
            sendData(client->get_client_socket(), ERR_NOTENOUGHPARAM(client->nickname));
    }
    else
        sendData(client->get_client_socket(), ERR_NOTREGISTERED(client->nickname));
}