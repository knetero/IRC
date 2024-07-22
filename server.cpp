#include "client.hpp"
#include "channel.hpp"
#include <cctype>
#include <cstddef>
#include "server.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "replies.hpp"
#include "convert.hpp"

Server::Server(int port, const std::string& password){
    this->port = port;

    this->password = password;
    if(!initialize()){
        std::cerr << "Failed to initialize the server" << std::endl;
        exit(1);
    }
    std::cout << "Server is running on port " << port << std::endl;
    this->isServerCommand = false;
    this->isSetNick  = false;
    this->isSetPass = false;
    this->isSetUser = false;

}        
bool Server::signal = false;
void Server::signalHandler(int signum){
    (void)signum;
    Server::signal = true;

}

int Server::acceptClient() 
{
    struct sockaddr_in clientAddress; // struct that holds the client address
    socklen_t clientAddressLength = sizeof(clientAddress); // size of the client address struct 
    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength); // accept a connection on a socket
    if (clientSocket == -1) {
        std::cerr << "Failed to accept client connection. errno: " << errno << std::endl;
        return -1;
    }
    // if (signal) {
    //     if (Server::signal) {
    //         return -1;
    //     }
    // }

    // Set the socket to non-blocking mode
    if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) < 0) {
        std::cerr << "Failed to set client socket to non-blocking mode." << errno << std::endl;
        close(clientSocket);
    }

    std::cout << "Client connected FD: " << clientSocket << std::endl;
    return clientSocket;
}

bool Server::initialize()
{
    // Create a socket
    // AF_INET means we are using IPv4
    // SOCK_STREAM means we are using a TCP connection
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create a socket. errno: " << errno << std::endl;
        return false;
    }
    // declare the struct for the server address
    struct sockaddr_in serverAddress; // struct that holds the server address
    serverAddress.sin_family = AF_INET; // means we are using IPv4
    serverAddress.sin_addr.s_addr = INADDR_ANY; // means we will accept connections from any IP address
    serverAddress.sin_port = htons(port); // means we will accept connections on this port

    //set the socket to reuse the address
    // SO_REUSEADDR means that the port can be reused immediately after the socket is closed
    // SOL_SOCKET is the level of the socket
    int enable = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == -1) { //
        perror("setsockopt");
        exit(1);
    }
    // set the socket to non-blocking mode
    if(fcntl(serverSocket, F_SETFL, O_NONBLOCK) < 0) { // F_SETFL means set file status flags
        perror("fcntl");
        exit(1);
    }
    // Bind the socket to the IP address and port
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) { 
        std::cerr << "Failed to bind to port " << port << ". errno: " << errno << std::endl;
        return false;
    }
    // Mark the socket for listening in
    if (listen(serverSocket, 5) == -1) { // 5 is the maximum length to which the queue of pending connections for serverSocket may grow
        std::cerr << "Failed to listen on socket. errno: " << errno << std::endl;
        return false;
    }
    return true;
}
void Server::send_private_message(int clientSocket, const std::string& data, std::map<int, Client>& clients_Map){
    size_t spacePos = data.find(' ');
    if (spacePos != std::string::npos) {
        std::string target = data.substr(0, spacePos);
        std::string message = data.substr(spacePos + 1);
        if(message[0] != ':') {
            sendError(clientSocket, ERR_NOTEXTTOSEND(clients_Map[clientSocket].nickname));
            return;
        }
        std::string senderNickname;
        std::string senderUsername;
        for (std::map<int, Client>::iterator it = clients_Map.begin(); it != clients_Map.end(); ++it) { 
            if (it->first == clientSocket) {
                senderNickname = it->second.nickname;
                senderUsername = it->second.username;
                break;
            }
        }
        int recipientFd = -1;
        for (std::map<int, Client>::iterator it = clients_Map.begin(); it != clients_Map.end(); ++it) {
            if (it->second.nickname == target) {
                recipientFd = it->first;
                break;
            }
        }
        if (recipientFd == -1) {
            sendError(clientSocket, ERR_NOSUCHNICK(clients_Map[clientSocket].nickname, target));
        }
        else {
            std::string messageToSend = ":" + senderNickname + "!~" + senderUsername + "@" + "localhost PRIVMSG " + target + message + "\r\n";
            send(recipientFd, messageToSend.c_str(), messageToSend.size(), 0);
        }
    }
    else {
        sendError(clientSocket, ERR_NOTEXTTOSEND(clients_Map[clientSocket].nickname));
        return;
    }
}

bool Server::sendError(int clientSocket, const std::string& errorMessage){ //
     if (!sendMessage(clientSocket, errorMessage)) { //
        std::cerr << "Error sending message: " << errorMessage << std::endl;
        return false;
    }
    return true;
}

bool Server::isCommand(const std::string& data)
{
    std::string command = data;

    if(command.find(" ") != std::string::npos)
        command = command.substr(0, command.find(" "));
    command.erase(0, command.find_first_not_of(" \n\r\t\f\v")); // remove leading whitespace
    command.erase(command.find_last_not_of(" \n\r\t\f\v") + 1);


    command = toUpperCase(command);

    if(command == "NICK" || command == "USER" || command == "PASS" || command == "PRIVMSG" || command == "INVITE" || command == "JOIN" 
        || command == "PART" || command == "MODE" || command == "TOPIC"|| command == "NAMES" || command == "LIST" 
        || command == "KICK" || command == "QUIT" || command == "PING" || command == "MODE" || command == "TOPIC" || command == "NOTICE")
    {
        return true;
    }
    return false;
}

void Server::notifyClients(int clientSocket, std::map<int, Client>& clients_Map, std::string oldNick){
    std::string nickChangeMsg = RPL_NICKCHANGE(oldNick, clients_Map[clientSocket].nickname);
    for(std::map<int, Client>::iterator it = clients_Map.begin(); it != clients_Map.end(); ++it) {
        if(it->first != clientSocket) {
            send(it->first, nickChangeMsg.c_str(), nickChangeMsg.size(), 0);
        }
    }
}

std::vector<std::string> Server::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) { // getline reads characters from the input stream and places them into a string until the delimeter is found
        if (!item.empty()) {
            elems.push_back(item); 
        }
    }
    return elems;
}

void Server::check_user(int clientSocket, const std::string&d , std::map<int, Client>& clients_Map, Client& client){
    std::string data = d;
    toUpperCase(data);
    std::vector<std::string> parts = split(data, ' '); // split the string into parts
    if (parts.empty()) return;

    std::string command = parts[0];
    parts.erase(parts.begin());

    if (parts.size() < 4) {
        sendError(clientSocket, ERR_NOTENOUGHPARAM(clients_Map[clientSocket].nickname));
        return;
    }
    std::string username = parts[0];
    std::string realname = parts[4];        
    clients_Map[clientSocket].username = username;
    clients_Map[clientSocket].realname = realname;


    client.userSet = true;
}

bool Server::sendMessage(int fd, const std::string& message) {
    std::string formattedMessage = message;
    if (formattedMessage.back() != '\n') { 
        formattedMessage += "\r\n";
    }

    ssize_t bytesSent = send(fd, formattedMessage.c_str(), formattedMessage.length(), 0);
    
    if (bytesSent == -1) {
        std::cerr << "Failed to send message: " << std::strerror(errno) << std::endl;
        return false;
    }
    return true;
}

void Server::sendWelcomeMessages(int clientSocket, std::map<int, Client>& clients_Map) {
    std::string welcomeMsg = clients_Map[clientSocket].nickname + " :Welcome to the Server, " + 
                             clients_Map[clientSocket].nickname + "!<" + clients_Map[clientSocket].username + ">@127.0.0.1";
    sendMessage(clientSocket, welcomeMsg);

    std::string yourHostMsg = clients_Map[clientSocket].nickname + " :Your host is " + 
                              ">@127.0.0.1" + ", running version " + "1.0";
    sendMessage(clientSocket, yourHostMsg);

    std::string createdMsg = clients_Map[clientSocket].nickname  + " :This server was created " + 
                             "2024" + " and is running version " + "1.0";
    sendMessage(clientSocket, createdMsg);
}

void Server::check_Quit(int clientSocket, const std::string& data, std::map<int, Client>& clients_Map)
{
    std::string reason = "Quit";
    size_t reasonIndex = data.find(" ");
    if (reasonIndex != std::string::npos) {
        reason = data.substr(reasonIndex + 1);
    }


    //until we have channels ready to notify the clients that shares the same channel

    // std::string quitMessage = ":" + clients_Map[clientSocket].nickname + "!" + clients_Map[clientSocket].username + "@localhost QUIT :" + reason;    
    // for (std::map<int, Client>::iterator it = clients_Map.begin(); it != clients_Map.end(); ++it) {
    //     if (it->first != clientSocket) {
    //         sendMessage(it->first, quitMessage);
    //     }
    // }
    std::cout << "Server acknowledges QUIT command." << std::endl;
    close(clientSocket);
    clients_Map.erase(clientSocket);

    //delete client from the channel
}

bool Server::check_Nick(int clientSocket, std::string value, std::map<int, Client>& clients_Map){
    if (value.empty()) {
        sendError(clientSocket, ERR_NONICKNAME(clients_Map[clientSocket].nickname));
        return false;
    }
    if (value[0] == '#' || value[0] == ':' || (isdigit(value[0]))) {
        sendError(clientSocket, ERR_ERRONEUSNICK(value));
        return false;
    }
    return true;
}



void Server::parse_commands(int clientSocket, const std::string& data, std::map<int, Client>& clients_Map){

    // Channel defaultChannel;
    // Channel nameChannel("server_Channels");
    if(isCommand(data) == false){
        isServerCommand = true;
        sendError(clientSocket, ERR_CMDNOTFOUND(clients_Map[clientSocket].nickname, data));
        return;
    }
    Client &client = clients_Map[clientSocket];
    size_t spacePos = data.find(' ');
    if (spacePos != std::string::npos) 
    {
        std::string command = data.substr(0, spacePos);
        std::string upcommand = toUpperCase(command);
        std::string value = data.substr(spacePos + 1);

        value.erase(std::remove(value.begin(), value.end(), '\r'), value.end());
        value.erase(std::remove(value.begin(), value.end(), '\n'), value.end());
        if(upcommand == "NICK") {
            if(!check_Nick(clientSocket, value, clients_Map))
                return;
            for(std::map<int, Client>::iterator it = clients_Map.begin(); it != clients_Map.end(); ++it) {
                if(it->second.nickname == value) {
                    sendError(clientSocket, ERR_NICKINUSE(clients_Map[clientSocket].nickname));
                    return;
                }
            }
            std::string oldNick = clients_Map[clientSocket].nickname;
            clients_Map[clientSocket].nickname = value;
            if(!oldNick.empty()) {
                notifyClients(clientSocket, clients_Map, oldNick);
            }
            isServerCommand = true;
            client.nickSet = true;
        }
        else if(upcommand == "USER") {
            if(client.userSet) {
                sendError(clientSocket, ERR_NOTREGISTERED(clients_Map[clientSocket].nickname));
                return;
            }
            check_user(clientSocket, data, clients_Map, client);
            isServerCommand = true;
        }
        else if (upcommand == "PASS")
        {
            if(client.passSet) {
                sendError(clientSocket, ERR_ALREADYREGISTERED(clients_Map[clientSocket].nickname));
                return;
            }
            isServerCommand = true;
            if (value != password) {
                sendError(clientSocket, ERR_INCORPASS(clients_Map[clientSocket].nickname));
                return;
            }
            client.passSet = true;
        }
        else if(upcommand == "PRIVMSG")
        {
            if(!client.passSet || !client.nickSet || !client.userSet){
                sendError(clientSocket, ERR_NOTREGISTERED(clients_Map[clientSocket].nickname));
                return;
            }
            isServerCommand = true;
            send_private_message(clientSocket, value, clients_Map);
        }
        else if(upcommand == "QUIT")
        {
            check_Quit(clientSocket, data, clients_Map);
            isServerCommand = true;
        }
        else if (upcommand == "JOIN")
        {
           join(value, clientSocket, clients_Map);
            // join(value);
        }
        else if (upcommand == "MODE")
        {
           mod(value, clientSocket,  clients_Map);
        }
        if(client.passSet && client.userSet && client.nickSet) {
            client.isRegistered = true;
            if(client.welcome_msg == 1)
                sendWelcomeMessages(clientSocket, clients_Map);
            client.welcome_msg++;
        }
    }
    else {
        sendError(clientSocket, ERR_NOTENOUGHPARAM(clients_Map[clientSocket].nickname));
        return;
    }
}

std::string Server::toUpperCase(std::string& str) {
    for (size_t i = 0; i < str.length(); ++i) {
        str[i] = std::toupper(static_cast<unsigned char>(str[i]));
    }
    return str;
}



Server::~Server(){
    // shutdown();
}

/************************************************JOIN***************************************************************/

int Server::check_properties(Channel channel, std::string mdp, int clientsocket, std::map<int, Client>& clients_Map)
{
    if (channel.getmodes().find("k") != std::string::npos) {
        if (channel.getpassword().compare(mdp) != 0 || mdp.empty())
        {
            sendError(clientsocket, ERR_BADCHANNELKEY(clients_Map[clientsocket].nickname, channel.getName()));
            return(1);
        }
    }
        if (channel.getmodes().find("l") != std::string::npos && channel.getSize() - channel.getlimit() <= 0)
        {
            sendError(clientsocket, ERR_CHANNELISFULL(clients_Map[clientsocket].nickname, channel.getName()));
            return(1);
        }
        if(channel.getmodes().find("i") != std::string::npos )
        {
            if (channel.getinvited()->find(clientsocket) == channel.getinvited()->end())
            {
                sendError(clientsocket, ERR_INVITEONLYCHAN(clients_Map[clientsocket].nickname, channel.getName()));
                return(1);
            }
        }
    return (0);     
}
void Server::join(std::string value, int clientsocket, std::map<int, Client>& clients_Map)
{
    std::istringstream iss(value);
    std::map<std::string, std::string> map_channels;
    std::string channels;
    std::string ch_name;
    std::string password;
    std::string p;
    std::string ch;
    std::map<int, Client> members;
std::map<int, Client> operators;
std::map<int, Client> kicked_clients;
std::map<int, Client> invited_clients;
    std::getline(iss, channels, ' ');
    std::stringstream ss(strTrim( channels , " ")); 
        password = value.substr( value.find(" ") + 1,value.size());
        std::stringstream pa(strTrim( password , " "));
    
    if (strTrim(value, " ").empty())
        sendError(clientsocket, ERR_NEEDMOREPARAMS(clients_Map[clientsocket].nickname, "JOIN"));
    if(value.find(" ") != std::string::npos)
    {
        while (getline(pa, p, ',') && getline(ss, ch, ',') )
        {
            if(ch[0] != '#')
            {
                sendError(clientsocket, ERR_BADCHANMASK(ch));
                while((getline(ss, ch, ',') && ch[0] != '#' ));
            }
            if(ch[0] == '#')
            {
                map_channels[ch] = p; 
            }
        }
    }
        while (getline(ss, ch, ','))
        {
            if(ch[0] != '#')
            {
                sendError(clientsocket, ERR_BADCHANMASK(ch));
                while((getline(ss, ch, ',') && ch[0] != '#' ));
            }
            if(ch[0] == '#')
            {
                map_channels[ch] = "-1"; 
            }
        }
    std::map<std::string, std::string>::iterator it;
    std::map<std::string, std::string>::iterator element;
    std::string msg;
    for (it = map_channels.begin(); it != map_channels.end(); ++it) {
        std::map<std::string, Channel>::iterator element = server_channels.find(it->first.substr(1));
        if (element != server_channels.end()) {
            if (check_properties( element->second,  map_channels[it->first], clientsocket, clients_Map) == 0)
            {
                if (element->second.getmembers()->find(clientsocket)->first != clientsocket)
                {
                    element->second.add_user(clients_Map[clientsocket], clientsocket, 0);
                    element->second.setSize(element->second.getSize()+1);
                    msg = ":" + clients_Map[clientsocket].nickname + "!" + clients_Map[clientsocket].username + "@" + "127.0.0.1" + " JOIN " + "#" +element->second.getName();
                    sendMessage(clientsocket, msg);
                    // sendError(clientsocket, RPL_JOIN(clients_Map[clientsocket].nickname, element->second.getName()));
                    sendError(clientsocket, RPL_NAMREPLY(clients_Map[clientsocket].nickname, element->second.getName(), element->second.getMemberNames()));
                    sendError(clientsocket, RPL_ENDOFNAMES(clients_Map[clientsocket].nickname, element->second.getName()));
                    if (!element->second.gettopic().empty())
                        sendError(clientsocket, RPL_TOPIC(clients_Map[clientsocket].nickname, element->second.getName(), element->second.gettopic()));
                }
            }
            else
            {
                return;
            }
        }
        else {
            Channel chn = Channel();
            chn.setName(it->first.substr(1));
            if (map_channels[it->first].compare("-1") != 0)
            {
                chn.setpassword(map_channels[it->first]);
                chn.setmodes("k");
            }
                server_channels.insert (std::make_pair(it->first.substr(1),chn));
                server_channels[it->first.substr(1)].add_user(clients_Map[clientsocket], clientsocket, 1);
                server_channels[it->first.substr(1)].add_user(clients_Map[clientsocket], clientsocket, 0);
                chn.setSize(chn.getSize()+1);
                msg = ":" + clients_Map[clientsocket].nickname + "!" + clients_Map[clientsocket].username + "@" + "127.0.0.1" + " JOIN " + "#" +chn.getName();
                sendMessage(clientsocket, msg);
                sendError(clientsocket, RPL_NAMREPLY(clients_Map[clientsocket].nickname, chn.getName(), chn.getMemberNames()));
                sendError(clientsocket, RPL_ENDOFNAMES(clients_Map[clientsocket].nickname, chn.getName()));
                if (!chn.gettopic().empty())
                    sendError(clientsocket, RPL_TOPIC(clients_Map[clientsocket].nickname, chn.getName(), chn.gettopic()));
        }
    }
     std::map<int, Client>::iterator itt;
     for (itt = server_channels.find("chan")->second.getmembers()->begin(); itt != server_channels.find("chan")->second.getmembers()->end(); ++itt) {
                std::cout << itt->first << "**"<< std::endl;
        }
}


/************************************************MODE***************************************************************/
int Server::get_nick(std::string chName, std::string nickname)
{
      std::map<int, Client>::iterator itt;
      (void)nickname;
      std::cout << "no nicknamvve" << std::endl;
            for (itt = server_channels.find(chName.substr(1))->second.getmembers()->begin(); itt != server_channels.find(chName.substr(1))->second.getmembers()->end(); ++itt) {
                std::cout << itt->first << std::endl;

                // if (!itt->second.nickname.empty()  && itt->second.nickname  == nickname)
                // {
                //     std::cout<< "first"<< std::endl;
                //     return (itt->first);
                // }
            }
            // std::cout << "no nickname" << std::endl;
            return (-1);
}




// void send_Mode( int clientsocket, std::map<int, Client>&  clients_Map, std::string chname, char mode, std::string sign , int witharg, std::string args)
// {
//             std::map<int, Client>::iterator itt;
//             std::string arg = "";
//             std::string str1(1, mode);;
//             if (witharg != 0)
//                 arg = args;
//             for (itt = clients_Map.begin(); itt != clients_Map.end(); ++itt) {
//                 if (clients_Map[clientsocket].clientSocket != clientsocket)
//                 {
//                     sendError(clientsocket ,RPL_SENDMODEIS(clients_Map[clientsocket].nickname, chname, str1, sign, arg));
//                 }
//             }
// }


void Server::mod(std::string value, int clientsocket, std::map<int, Client>&  clients_Map)
{
   (void)clientsocket;
//    (void)value;
   (void)clients_Map;
    std::istringstream iss(value);
    std::string channel;
    std::vector<std::string> args;
    std::vector<std::string> modes;
    std::string ss;


    while (std::getline(iss, channel, ' ')) {  
        if (strTrim( channel , " ") != "")
        {
            args.push_back(strTrim( channel , " "));
            // std::cout << "|"<< strTrim( channel , " ") << "|"<< std::endl;
        }
    } 
        std::cout << " fffff : ||||| " << args.size() << "***"  << std::endl;
            // std::map<int, Client>::iterator itt;
            // // for (itt = server_channels.find(args[0].substr(1))->second.getoperators()->begin(); itt != server_channels.find(args[0].substr(1))->second.getoperators()->end(); ++itt) {
            //     if (server_channels.find(args[0].substr(1))->second.getoperators().find(clientsocket)->first == clientsocket)
            //     {
            //         std::cout << "*** ***"<< clientsocket<< std::endl;
            //         std::cout << "***** "<< server_channels.find(args[0].substr(1))->second.getoperators().find(clientsocket)->first << std::endl;
            //     }
            // }
        //   return ;
    // for (size_t i = 1; i < args.size() - 1; ++i) {}
        if (!args[0].empty() && args[0][0] == '#' && server_channels.find(args[0].substr(1)) != server_channels.end())
        {
            if (!args[1].empty() && (args[1][0] == '-' || args[1][0] == '+'))
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
                            sendError(clientsocket, ERR_NOSUCHCHANNEL(clients_Map[clientsocket].nickname, args[0]));
                        else if(server_channels.find(args[0].substr(1)) != server_channels.end() && server_channels.find(args[0].substr(1))->second.getmodes().find(modes[i][j]) != std::string::npos && isalpha(modes[i][j]) )
                        {
                            if (modes[i][0] == '-' && server_channels.find(args[0].substr(1))->second.getoperators()->find(clientsocket)->first == clientsocket)
                            {
                                ss = server_channels.find(args[0].substr(1))->second.getmodes();
                                ss.erase(std::remove(ss.begin(), ss.end(), modes[i][j]), ss.end());
                                server_channels.find(args[0].substr(1))->second.setmodes(ss);
                                // std::string arg;
                                // send_Mode( clientsocket,  clients_Map,args[0], modes[i][j],  "-" , 0,arg);
                            }
                        }
                        else if (server_channels.find(args[0].substr(1))->second.getoperators()->find(clientsocket)->first != clientsocket)
                                sendError(clientsocket, ERR_CHANOPRIVSNEEDED(clients_Map[clientsocket].nickname, args[0]) );
                        else if (server_channels.find(args[0].substr(1)) != server_channels.end() && server_channels.find(args[0].substr(1))->second.getmodes().find(modes[i][j]) == std::string::npos && modes[i][0] == '+' && server_channels.find(args[0].substr(1))->second.getoperators()->find(clientsocket)->first == clientsocket && isalpha(modes[i][j])) 
                        {   
                            if (modes[i][j] == 'l')
                            {
                                if(!args[2].empty())
                                {
                                    if (cast_int(args[2]) != -1)
                                    {
                                        std::cout << "args[2]     "<<cast_int(args[2]) << " |||||| " << std::endl;
                                        server_channels.find(args[0].substr(1))->second.setlimit(cast_int(args[2]));
                                        args.erase(std::next(args.begin(), 2));
                                        ss = server_channels.find(args[0].substr(1))->second.getmodes() + modes[i][j];
                                        server_channels.find(args[0].substr(1))->second.setmodes(ss);

                                    }
                                    else
                                        std::cout << " Error on limit number"<< std::endl;
                                } 
                                else
                                    std::cout << " Error on number of args"<< std::endl;

                            }
                            else if (modes[i][j] == 'k')
                            {
                                if ( !args[2].empty())
                                {
                                    server_channels.find(args[0].substr(1))->second.setpassword(args[2]);
                                    ss = server_channels.find(args[0].substr(1))->second.getmodes() + modes[i][j];
                                    server_channels.find(args[0].substr(1))->second.setmodes(ss);
                                        std::cout << "args[2]     "<< args[2] << " |||||| " << std::endl;
                                }
                                else
                                    std::cout <<  " Error on number of args" << std::endl;;
                            }
                            else if (modes[i][j] == 't' || modes[i][j] == 'i')
                            {
                                ss = server_channels.find(args[0].substr(1))->second.getmodes() + modes[i][j];
                                server_channels.find(args[0].substr(1))->second.setmodes(ss);
                            }
                            else if (modes[i][j] == 'o' && !args[2].empty() &&  get_nick(args[0], args[2]) != -1)
                            {
                                std::cout <<  " Error 1" << std::endl;
                                server_channels[args[0].substr(1)].add_user(clients_Map[get_nick(args[0], args[2])], get_nick(args[0], args[2]), 1);
                                std::cout <<  " Error 2" << std::endl;;
                                ss = server_channels.find(args[0].substr(1))->second.getmodes() + modes[i][j];
                                std::cout <<  " Error 3" << std::endl;;
                                server_channels.find(args[0].substr(1))->second.setmodes(ss);                                                                                                                                                                                                                                                                                                                                                                                                  
                            }


                        }
                        j++;
                    }
                    i++;
                                                
                }
            }
            else if (args.size() <= 1 && server_channels.find(args[0].substr(1)) != server_channels.end())
                sendError(clientsocket, RPL_CHANNELMODEIS(clients_Map[clientsocket].nickname, args[0], server_channels.find(args[0].substr(1))->second.getmodes()) );

        }
        else
        {
            // std::cout << "error channel doesnt exist "<< args[0]<< std::endl;
            sendError(clientsocket,ERR_NOSUCHCHANNEL(clients_Map[clientsocket].nickname, args[0]));
            return;
        }
        std::cout << " after : |||||" << server_channels.find(args[0].substr(1))->second.getmodes() << "||||||" << std::endl;
    return;
}

