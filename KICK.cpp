#include "server.hpp"

std::vector<std::string> split_(std::string string, char delimiter)
{
    std::vector<std::string>    splitted;
    std::string                 slice;
    std::istringstream          ss(string);

    while (std::getline(ss, slice, delimiter))
        splitted.push_back(slice);
    return splitted;
}

void Server::broadcastToChannel(Channel &channel, Client *kicker, std::string target, std::string comment)
{
    std::map<int, Client *> channelMembers = channel.getmembers();
    std::map<int, Client *>::iterator it;
    for (it = channelMembers.begin(); it != channelMembers.end(); it++)
    {
        Client *member = serverClients[it->first];
        sendData(member->get_client_socket(), KICK(kicker->nickname, kicker->username, getIp(kicker->clientAdress), channel.getName(), target, comment));
    }
}

void Server::kickCommand(Client *client, std::vector <std::string> &parameters)
{
    if (client->isRegistered)
    {
        if (parameters.size() < 4)
        {
            sendData(client->clientSocket, ERR_NEEDMOREPARAMS(client->nickname, "KICK"));
            return ;
        }
        else if (parameters.size() > 4 && parameters[3][0] != ':')
        {
            sendData(client->clientSocket, ERR_NEEDMOREPARAMS(client->nickname, "KICK"));   
            return ;
        }
        std::string comment = "";
        if (parameters.size() == 4)
            comment = parameters[3];
        // store users 
        std::vector<std::string> users = split_(parameters[2], ',');
        // check the channel exist;
        if (channelExist(parameters[1]) == -1)
        {
            sendData(client->clientSocket, ERR_NOSUCHCHANNEL(client->nickname, parameters[1], getIp(client->clientAdress)));
            return ;
        }
        
        for (size_t i = 0; i < users.size(); i++)
        {
            Channel *channel = this->server_channels[parameters[1].substr(1)];
            if (channel->clientExist(client->nickname) != -1)
            {
                if (channel->clientExist(users[i]) != -1) // check if the user is in the channel
                {
                    if (channel->getoperators().find(client->clientSocket)->first == client->clientSocket ) // check if operator
                    {
                        Client *clientToKick = channel->getmembers()[getClientFd(users[i])];
                        broadcastToChannel(*channel, client, users[i], parameters[3]);
                        channel->removeUser(clientToKick); // remove user from members, operators and invited of channel
                    }
                    else
                        sendData(client->clientSocket, ERR_CHANOPRIVSNEEDED2(client->nickname, parameters[1]));
                }
                else
                    sendData(client->clientSocket, ERR_USERNOTINCHANNEL(client->nickname, parameters[2], parameters[1]));
            }
            else
                sendData(client->clientSocket, ERR_NOTONCHANNEL(client->nickname, parameters[1]));
        }
    }
    else
        sendData(client->clientSocket, ERR_NOTREGISTERED(client->nickname));
}