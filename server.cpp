#include "server.hpp"
#include <cstddef>
#include <cstdlib>

Server::Server(int port, const std::string& password){
    this->port = port;
    this->password = password;
    if(!initialize()){
        std::cerr << "Failed to initialize the server" << std::endl;
        // shutdown();
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

int Server::acceptClient(){
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength = sizeof(clientAddress);
    std::cout << "Waiting for client connection" << std::endl;
    int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);

    if(fcntl(clientSocket, F_SETFL, O_NONBLOCK) < 0) {
        perror("fcntl");
        exit(1);
    }
    if (clientSocket == -1) {
        std::cerr << "Failed to accept the client's connection. errno: " << errno << std::endl;
        return -1;
    }
    std::cout << clientSocket  << " : Client connected" << std::endl;
    return  clientSocket;
}

bool Server::initialize()
{
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Failed to create a socket. errno: " << errno << std::endl;
        return false;
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    int enable = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == -1) {
        perror("setsockopt");
        exit(1);
    }
    if(fcntl(serverSocket, F_SETFL, O_NONBLOCK) < 0) {
        perror("fcntl");
        exit(1);
    }
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Failed to bind to port " << port << ". errno: " << errno << std::endl;
        return false;
    }

    if (listen(serverSocket, 5) == -1) {
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
            sendError(clientSocket, "", "", 412, "No text to send\r\n");
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
            sendError(clientSocket, senderNickname, "PRIVMSG", 401, "No such nick/channel");
        }
        else {
            std::string messageToSend = ":" + senderNickname + "!~" + senderUsername + "@" + "localhost PRIVMSG " + target + message + "\r\n";
            send(recipientFd, messageToSend.c_str(), messageToSend.size(), 0);
        }
    }
    else {
        sendError(clientSocket, "", "", 412, "No text to send\r\n");
        return;
    }
}

void Server::sendError(int clientSocket, const std::string& nick,  const std::string& command, int errorCode, const std::string& errorMessage){
    std::string error = "Error(" + std::to_string(errorCode) +  ")" + nick + " " + command + " :" + errorMessage + "\r\n";
    send(clientSocket, error.c_str(), error.size(), 0);
}

void Server::enter_pass(int clientSocket, const std::string& data){
   send(clientSocket, "Enter server password: using PASS command", 16, 0);
   std::string command = data;

    if(command.find(" ") != std::string::npos)
        command = command.substr(0, command.find(" "));
    command.erase(0, command.find_first_not_of(" \n\r\t\f\v"));
    command.erase(command.find_last_not_of(" \n\r\t\f\v") + 1);

    if(command != "PASS") {
        sendError(clientSocket, "", "", 461, "");
        return;
    }
}

bool Server::isCommand(const std::string& data)
{
    std::string command = data;

    if(command.find(" ") != std::string::npos)
        command = command.substr(0, command.find(" "));
    command.erase(0, command.find_first_not_of(" \n\r\t\f\v"));
    command.erase(command.find_last_not_of(" \n\r\t\f\v") + 1);



    if(command == "NICK" || command == "USER" || command == "PASS" || command == "PRIVMSG" || command == "INVITE" || command == "JOIN" 
        || command == "PART" || command == "MODE" || command == "TOPIC"|| command == "NAMES" || command == "LIST" 
        || command == "KICK" || command == "QUIT" || command == "PING" || command == "MODE" || command == "TOPIC" || command == "NOTICE")
    {
        return true;
    }
    return false;
}

void Server::parse_commands(int clientSocket, const std::string& data, std::map<int, Client>& clients_Map){

    if(isCommand(data) == false){
        isServerCommand = true;
        sendError(clientSocket, clients_Map[clientSocket].username, "", 421, "Unknown command\r\n");
        return;
    }
    Client &client = clients_Map[clientSocket];
    size_t spacePos = data.find(' ');
    if (spacePos != std::string::npos) 
    {
        std::string command = data.substr(0, spacePos);
        std::string value = data.substr(spacePos + 1);

        value.erase(std::remove(value.begin(), value.end(), '\r'), value.end());
        value.erase(std::remove(value.begin(), value.end(), '\n'), value.end());
        if(command == "NICK") {
            if(client.nickSet) {
                sendError(clientSocket, "", "", 462, "You may not reregister\r\n");
                return;
            }
            if(value == "") {
                sendError(clientSocket, "", "NICK", 431, "No nickname given\r\n");
                return;
            }
            for(std::map<int, Client>::iterator it = clients_Map.begin(); it != clients_Map.end(); ++it) {
                if(it->second.nickname == value) {
                    sendError(clientSocket, "", "NICK", 433, "Nickname is already in use\r\n");
                    return;
                }
            }
            isServerCommand = true;
            client.nickSet = true;
            clients_Map[clientSocket].nickname = value;
        }
        else if(command == "USER") {
            if(client.userSet) {
                sendError(clientSocket, "", "USER", 462, "You may not reregister\r\n");
                return;
            }
            isServerCommand = true;
            client.userSet = true;
            clients_Map[clientSocket].username = value;
        }
        else if (command == "PASS")
        {
            if(client.passSet) {
                sendError(clientSocket, "", "PASS", 462, "You may not reregister\r\n");
                return;
            }
            isServerCommand = true;
            if(value != password) {
                sendError(clientSocket, "", "PASS", 464, "Password incorrect\r\n");
                return;
            }
            client.passSet = true;
        }
        else if(command == "PRIVMSG")
        {
            if(!client.checkRegistration()){
                sendError(clientSocket, "", "PRIVMSG", 451, "You have not registered\r\n");
                return;
            }
            isServerCommand = true;
            send_private_message(clientSocket, value, clients_Map);
        }
        // else if(command == "QUIT"){
        //     isServerCommand = true;
        //     send(clientSocket, "QUIT\r\n", 6, 0);
        //     close(clientSocket);
        //     clients_Map.erase(clientSocket);
        // }
    }
    else {
        sendError(clientSocket, clients_Map[clientSocket].username, "", 461, "Not enough parameters\r\n");
        return;
    }
}


Server::~Server(){
    // shutdown();
}
