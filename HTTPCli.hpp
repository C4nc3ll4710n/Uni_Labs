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


public:
    HTTPCli(char *host, int port)
    {
        sock = new TCPSock(host, port);
    }

    

    char* recvHEAD(){
        const char* request("HEAD /NOWA.txt HTTP/1.1\r\n\n");
        //const char* request("GET /NOWA.txt HTTP/1.1\r\nHost: 127.0.0.1:8000\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8\r\n");


        //const char* mb("\n");
        //std::cout << request << "\n";
        sock->sendRequest(request);
        //sock->sendRequest(mb);
        //std::cout << "recvHeadcheck\n";

        return sock->receive(BUF_SIZE);
    }


    char* recvDATA(long len){
        const char* request("GET / HTTP/1.1\r\n");
        const char* mb("\n");
        //std::cout << request << "\n";
        sock->sendRequest(request);
        sock->sendRequest(mb);

        return sock->receive(len);
    }

    /*void writeToFile() {
        char* p = this->recvDATA();
        std::ofstream file("output.txt");
        file.write(p, BUF_SIZE);
        file.close();
    }*/

   
        


};

#endif //HTTPSERV_HPP