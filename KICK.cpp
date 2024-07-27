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

int channelExist(std::map<std::string, Channel> &channels, std::string channelName)
{
    if (channelName[0] == '#')
        channelName = channelName.substr(1);
    else
        return -1;
    if (channels.find(channelName) != channels.end())
    {
        return 1;
    }
    return -1;
}

void Server::broadcastToChannel(Channel &channel, int operatorFd, std::string target, std::string comment)
{
    std::map<int, Client> *channelMembers = channel.getmembers();
    std::map<int, Client>::iterator it;
    for (it = channelMembers->begin(); it != channelMembers->end(); it++)
    {
        int memberFd = it->first;
        Client *member = serverClients[memberFd];
        sendData(memberFd, KICK(member->nickname, member->username, getIp(adresses[operatorFd]), channel.getName(), target, comment));
    }
}

void Server::kickCommand(int fd, std::vector <std::string> &parameters)
{
    if (this->serverClients[fd]->isRegistered)
    {
        if (parameters.size() < 3)
        {
            sendData(fd, ERR_NOTENOUGHPARAM(this->serverClients[fd]->nickname));
            return ;
        }
        else if (parameters.size() > 3 && parameters[3][0] != ':')
        {
            sendData(fd, ERR_NOTENOUGHPARAM(this->serverClients[fd]->nickname));
            return ;
        }
        std::string comment = "";
        if (parameters.size() == 3)
            comment = parameters[2];
        // store users 
        std::vector<std::string> users = split_(parameters[1], ',');
        // check the channel exist;
        if (channelExist(this->server_channels, parameters[0]) == -1)
        {
            sendData(fd, ERR_NOSUCHCHANNEL(this->serverClients[fd]->nickname, parameters[0]));
            return ;
        }
        for (size_t i = 0; i < users.size(); i++)
        {
            if (this->server_channels[parameters[0].substr(1)].clientExist(users[i]) != -1) // check if the user is in the channel
            {
                if (server_channels[parameters[0].substr(1)].getoperators()->find(fd) != server_channels[parameters[0].substr(1)].getoperators()->end()) // check if operator
                {
                    broadcastToChannel(server_channels[parameters[0].substr(1)], fd, users[i], parameters[2]);
                    server_channels[parameters[0].substr(1)].getmembers()->erase(server_channels[parameters[0].substr(1)].getmembers()->find(getClientFd(users[i]))); // remove user from members of channel
                }
                else
                    sendData(fd, ERR_CHANOPRIVSNEEDED2(this->serverClients[fd]->nickname, parameters[0]));
            }
            else
                sendData(fd, ERR_USERNOTINCHANNEL(this->serverClients[fd]->nickname, parameters[1], parameters[0]));
        }
    }
    else
        sendData(fd, ERR_NOTREGISTERED(this->serverClients[fd]->nickname));
}