

#include "server.hpp"
#include "replies.hpp"
#include "convert.hpp"



void Server::send_info(Channel *chName, std::string msg)
{
      std::map<int, Client *>::iterator itt;

            for (itt = chName->getmembers().begin(); itt != chName->getmembers().end(); ++itt) {

               sendData(itt->first, msg);
            }

}

void Server::mode(std::string value, Client *client)
{
    if (!client->isRegistered)
    {
        sendData(client->clientSocket, ERR_NOTREGISTERED(client->nickname));
        return ;
    }
    std::istringstream iss(value);
    std::string channel;
    std::string msg;
    std::vector<std::string> args;
    std::vector<std::string> modes;
    std::string modestr;
    std::string modeargs;
    std::string ss;
    int k = -1;
    int m = -1;

    while (std::getline(iss, channel, ' ')) {  
        if (strTrim( channel , " ") != "")
        {
            args.push_back(strTrim( channel , " "));
            std::cout<< "----"<< channel<<std::endl;
        }
    }
        if (args.empty())
        {
            sendData(client->clientSocket, ERR_NEEDMOREPARAMS(client->nickname, "MODE"));
            return;
        }
        else if (!args[0].empty() && args[0][0] == '#' && server_channels.find(args[0].substr(1)) != server_channels.end())
        {
                std::cout << args[0]<< "waaa tfuu"<< args.size() << "  " << server_channels.find(args[0].substr(1))->first << ""<< server_channels.find(args[0].substr(1))->second->getmodes()<< std::endl;
            if (args.size() <= 1 && server_channels.find(args[0].substr(1)) != server_channels.end() )
            {
                if( !server_channels.find(args[0].substr(1))->second->getmodes().empty())
                {
                    msg = ":127.0.0.1 324 " + serverClients[client->clientSocket]->nickname + " #" + server_channels.find(args[0].substr(1))->second->getName() +" +"+ server_channels.find(args[0].substr(1))->second->getmodes()+"\r\n";
                    sendData(client->clientSocket, msg);
                }
                else
                    return;
            }
            else if (!args[1].empty() && (args[1][0] == '-' || args[1][0] == '+'))
            {
                int i = 0;
                int j = 0;
                std::string s;
                while (args[1][i] && (args[1][i] == '+' || args[1][i] == '-'))
                {
                    j = i +1 ;
                    while(args[1][j] && (args[1][j] != '+' && args[1][j] != '-'))
                        j++;                                                                    //join #c1,#c2,#c3,c4,#c5 1 22 333 44
                                                                                                //MODE #c1 +sdjsjf-sfhf 10

                    s = args[1].substr(i  , j);
                    modes.push_back(s);
                    std::cout << s<< std::endl;
                    i = i+j;
                }
                i = 0;
                while ((size_t)i < modes.size())      
                {
                    j = 1;
                    while(modes[i][j])
                    {
                        if (server_channels.find(args[0].substr(1)) == server_channels.end())
                            sendData(client->clientSocket, ERR_NOSUCHCHANNEL(serverClients[client->clientSocket]->nickname, args[0]));
                        else if (server_channels.find(args[0].substr(1))->second->getoperators().find(client->clientSocket)->first != client->clientSocket)
                                sendData(client->clientSocket, ERR_CHANOPRIVSNEEDED(serverClients[client->clientSocket]->nickname, args[0]) );
                        else if(server_channels.find(args[0].substr(1)) != server_channels.end() && isalpha(modes[i][j]) && modes[i][0] == '-')
                        {
                            if (server_channels.find(args[0].substr(1))->second->getoperators().find(client->clientSocket)->first == client->clientSocket && server_channels.find(args[0].substr(1))->second->getmodes().find(modes[i][j]) != std::string::npos)
                            {
                                ss = server_channels.find(args[0].substr(1))->second->getmodes();
                                if (k == -1)
                                    modestr.append("-");
                                k++;
                                modestr.append(1, modes[i][j]);
                                ss.erase(std::remove(ss.begin(), ss.end(), modes[i][j]), ss.end());
                                server_channels.find(args[0].substr(1))->second->setmodes(ss);
                                if (modes[i][j] == 'l')
                                    server_channels.find(args[0].substr(1))->second->limit = -1;
                                else if (modes[i][j] == 't')
                                    server_channels.find(args[0].substr(1))->second->protectedTopic = false;
                                else if (modes[i][j] == 'i')
                                    server_channels.find(args[0].substr(1))->second->inviteonly = false;
                            }
                            else if (modes[i][j] == 'o' && args.size() >= 3)
                            {
                                if (k == -1)
                                    modestr.append("-");
                                k++;
                                modestr.append(1, modes[i][j]);
                                if (modes[i][j] == 'o' && getClientFd(args[2]) == -1)
                                    sendData(client->clientSocket, ERR_NOSUCHNICK(serverClients[client->clientSocket]->nickname, args[2]));
                                else if (modes[i][j] == 'o' &&  get_nick(args[0], args[2]) != -1)
                                    server_channels.find(args[0].substr(1))->second->removeOperator(serverClients[get_nick(args[0], args[2])]);
                                else if (modes[i][j] == 'o' && get_nick(args[0], args[2]) == -1)
                                    sendData(client->clientSocket, ERR_USERNOTINCHANNEL(serverClients[client->clientSocket]->nickname ,args[2] , args[0]));
                                modeargs.append(" "+args[2]+" ");
                                args.erase(args.begin() + 2);
                            }
                        }
                        else if (server_channels.find(args[0].substr(1)) != server_channels.end() && modes[i][0] == '+' && isalpha(modes[i][j])) 
                        {   
                           
                            if (modes[i][j] == 'l')
                            {
                                if(args.size() >= 3)
                                {
                                    if (convert(args[2]) != -1)
                                    {
                                        if (m == -1)
                                            modestr.append("+");
                                        m++;
                                        modestr.append(1, modes[i][j]);
                                        modeargs.append(" "+args[2]+" ");
                                        server_channels.find(args[0].substr(1))->second->setlimit(cast_int(args[2]));
                                        if (server_channels.find(args[0].substr(1))->second->getmodes().find(modes[i][j]) == std::string::npos)
                                        {
                                            ss = server_channels.find(args[0].substr(1))->second->getmodes() + modes[i][j];
                                            server_channels.find(args[0].substr(1))->second->setmodes(ss);
                                        }

                                    }
                                    args.erase(args.begin() + 2);
                                } 
                                else
                                    sendData(client->clientSocket, ERR_NEEDMOREPARAMS(client->nickname, "MODE"));

                            }
                            else if (modes[i][j] == 'k')
                            {
                                if ( args.size() >= 3)
                                {
                                      if (m == -1)
                                        modestr.append("+");
                                    m++;
                                         modestr.append(1, modes[i][j]);
                                        modeargs.append(" "+args[2]+" ");
                                    server_channels.find(args[0].substr(1))->second->setpassword(args[2]);
                                     if (server_channels.find(args[0].substr(1))->second->getmodes().find(modes[i][j]) == std::string::npos)
                                     {
                                        ss = server_channels.find(args[0].substr(1))->second->getmodes() + modes[i][j];
                                        server_channels.find(args[0].substr(1))->second->setmodes(ss);

                                     }
                                        args.erase(args.begin() + 2);
                                }
                                else
                                    sendData(client->clientSocket, ERR_NEEDMOREPARAMS(client->nickname, "MODE"));
                            }
                            else if (modes[i][j] == 't' || modes[i][j] == 'i')
                            {
                                  if (m == -1)
                                    modestr.append("+");
                                m++;
                                 if (server_channels.find(args[0].substr(1))->second->getmodes().find(modes[i][j]) == std::string::npos)
                                 {
                                    ss = server_channels.find(args[0].substr(1))->second->getmodes() + modes[i][j];
                                    server_channels.find(args[0].substr(1))->second->setmodes(ss);
                                 }
                                    modestr.append(1, modes[i][j]);
                                if (modes[i][j] == 't' )
                                    server_channels.find(args[0].substr(1))->second->protectedTopic = true;
                                else
                                    server_channels.find(args[0].substr(1))->second->inviteonly = true;

                            }
                            else if (modes[i][j] == 'o')
                            {
                                if (args.size() < 3)
                                    sendData(client->clientSocket, ERR_NEEDMOREPARAMS(client->nickname, "MODE"));
                                else
                                {
                                    if(getClientFd(args[2]) == -1)
                                        sendData(client->clientSocket, ERR_NOSUCHNICK(serverClients[client->clientSocket]->nickname ,args[2]));
                                    else if (get_nick(args[0], args[2]) != -1)
                                    {
                                            if (m == -1)
                                                modestr.append("+");
                                            m++;
                                            modestr.append(1, modes[i][j]);
                                            modeargs.append(" "+args[2]+" ");
                                            server_channels.find(args[0].substr(1))->second->add_user(serverClients[get_nick(args[0], args[2])] ,1 );
                                    }  
                                    else
                                        sendData(client->clientSocket, ERR_USERNOTINCHANNEL(serverClients[client->clientSocket]->nickname, args[2], args[0]));
                                    args.erase(args.begin() + 2);
                                }
                            }
                            

                        }
                        j++;
                    }
                    i++;
                                                
                }
                if (!modestr.empty() && (modestr != "-" && modestr != "+"))
                {
                    msg = ":" + serverClients[client->clientSocket]->nickname + "!" + serverClients[client->clientSocket]->username + "@" + getIp(client->clientAdress) + " MODE " + "#" +server_channels.find(args[0].substr(1))->second->getName() +" "+modestr +" "+ modeargs +"\r\n";
                    send_info(server_channels.find(args[0].substr(1))->second, msg);
                }
            }

        }
        else
        {
            // std::cout << "error channel doesnt exist "<< args[0]<< std::endl;
            sendData(client->clientSocket,ERR_NOSUCHCHANNEL(serverClients[client->clientSocket]->nickname, args[0]));
            return;
        }
    return;
}
