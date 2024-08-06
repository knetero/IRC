#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include<iostream>
#include<map>
#include <vector>
#include "client.hpp"
#include <string>
#include <iostream>
#include <sstream>

class Client;
class Channel {
    private:
        std::string name;
        std::string type;
        std::string modes;
        std::string topic;
        std::string password;
        int size;
<<<<<<< HEAD
        // int limit;
        std::map<int, Client *>  members;
        std::map<int, Client *> operators;
        std::map<int, Client *>  invited_clients;
=======
        int limit;
        std::map<int, Client >  *members;
        std::map<int, Client > * operators;
        std::map<int, Client >  * invited_clients;
>>>>>>> 858be78f1c14a9e55cae2f94e6ebc520112464a2


        // Constructeur de la classe Channel
    public:
<<<<<<< HEAD
        int limit;
        bool hasTopic;
        bool protectedTopic;
        bool inviteonly;
=======
        bool otopic;
        bool inviteonly;
    // Channel(std::map<int, Client>* members,
    //              std::map<int, Client >* operators,
    //              std::map<int, Client >* kicked_clients,
    //              std::map<int, Client >* invited_clients);
>>>>>>> 858be78f1c14a9e55cae2f94e6ebc520112464a2
        void setName(std::string name);  
        void settype(std::string type);  
        void setmodes(std::string modes);  
        void settopic(std::string topic);  
        void setpassword(std::string password);
        void setSize(int size);
        void setlimit(int limit);  

        std::string getName();  
        int getSize();  
        int getlimit();  
        std::string gettype();  
        std::string getmodes();  
<<<<<<< HEAD
        std::string &gettopic();  
        std::string getpassword();

        std::map<int, Client *>  &getmembers();
        std::map<int, Client *>  &getinvited();
        std::map<int, Client *>  &getoperators();
        void add_user(Client * c , int clientsocket, int type); 
        int removeUser(Client *);
        std::string getMemberNames();
        int clientExist(std::string name);
        Channel(void);
        ~Channel(){std::cout << "channel destructor" << std::endl;}
=======
        std::string gettopic();  
        std::string getpassword();

        std::map<int, Client >  *getmembers();
        std::map<int, Client >  *getinvited();
        std::map<int, Client >  *getoperators();
        void add_user(Client * c , int clientsocket, int type); 
        std::string getMemberNames();
        Channel(void);
>>>>>>> 858be78f1c14a9e55cae2f94e6ebc520112464a2

};
std::string UpperCaseStr(std::string s);
std::string strTrim( std::string s1,  std::string set);
#endif