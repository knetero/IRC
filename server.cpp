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
void Server::send_private_message(int clientSocket, const std::string& data, std::map<int, Client >& clients_Map){
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

void Server::notifyClients(int clientSocket, std::map<int, Client >& clients_Map, std::string oldNick){
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

void Server::check_user(int clientSocket, const std::string&d , std::map<int, Client >& clients_Map, Client& client){
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

void Server::sendWelcomeMessages(int clientSocket, std::map<int, Client >& clients_Map) {
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

void Server::check_Quit(int clientSocket, const std::string& data, std::map<int, Client >& clients_Map)
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

bool Server::check_Nick(int clientSocket, std::string value, std::map<int, Client >& clients_Map){
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



void Server::parse_commands(int clientSocket, const std::string& data, std::map<int, Client >& clients_Map){

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
        //       std::map<int, Client >::iterator itt;
    
        //    for (itt = server_channels.find("chan")->second.getmembers()->begin(); itt != server_channels.find("chan")->second.getmembers()->end();++itt) {
        //                      std::cout << itt->second.nickname << " client nickname" << std::endl;
        //                     std::cout << "hellllll" << std::endl;
        //                 }
        }

        // else if (upcommand == "MODE")
        // {
        //    mod(value, clientSocket,  clients_Map);
        // }
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
