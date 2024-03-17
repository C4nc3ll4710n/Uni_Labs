#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include "TCPSock.hpp"
#ifndef HTTPSERV_HPP
#define HTTPSERV_HPP

//!not sure if all of them are needed

#define MYPORT 80// the port users will be connecting to
#define BACKLOG 10// how many pending connections queue will hold
#define BUF_SIZE 2048

class HTTPServ
{
    TCPSock* sock;


public:
    HTTPServ()
    {
        sock = new TCPSock();
    }


    /*std::string receiveRequest()
    {
        char* buf = new char[BUF_SIZE];
        
        int res = this->sock->receive(buf, BUF_SIZE);
        std::string tmp(buf);
        std::string msg = tmp.substr(0, tmp.find("\r\n\r\n") + 1);
        memset(buf, 0, BUF_SIZE);
        delete[] buf;
        
        return msg;
    }
    */



};

#endif //HTTPSERV_HPP