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
#include <fstream>
#include <netdb.h>

#include "TCPSock.hpp"
#ifndef HTTPCLI_HPP
#define HTTPCLI_HPP

//!not sure if all of them are needed

#define MYPORT 80// the port users will be connecting to
#define BACKLOG 10// how many pending connections queue will hold
#define BUF_SIZE 2048

class HTTPCli
{
    TCPSock* sock;
    char *host;
    int port;


public:
    HTTPCli(char *hst, int prt)
        :host(hst), port(prt) { sock = new TCPSock(host, port);}

    

    char* recvHEAD(const std::string& filename, char *buffer){
        //std::cout << "recvHEAD\n";
        sock = new TCPSock(host, port);
        std::string req = "HEAD /"+filename+" HTTP/1.0\r\n\r\n";
        const char* request = req.c_str();
        
        sock->sendRequest(request);
        //std::cout <<"This is request from HTTPCli:" <<request << "\n";
        

        return sock->receive(BUF_SIZE, buffer);
    }


    char* recvDATA(long len, const std::string& filename, char *buffer){
        //std::cout << "LEN FROM RECV DATA " << len << "\n";
        //std::cout << "recvDATA\n";
        sock = new TCPSock(host, port);
        std::string req = "GET /"+filename+" HTTP/1.0\r\n\r\n\r\n";
        const char* request = req.c_str();
        //std::cout << request << "\n";
        sock->sendRequest(request);
        //std::cout <<"This is request from HTTPCli GET:" <<request << "\n";

        return sock->receive(len, buffer);
    }


   
        


};

#endif //HTTPSERV_HPP