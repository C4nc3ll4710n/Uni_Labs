#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <vector>
#include <string>

/*

struct sockaddr_in {
    short            sin_family;   // e.g. AF_INET
    unsigned short   sin_port;     // e.g. htons(3490)
    struct in_addr   sin_addr;     // see struct in_addr, below
    char             sin_zero[8];  // zero this if you want to
};

struct in_addr {
    unsigned long s_addr;  // load with inet_aton()
};
*/

#ifndef TCPSOCK_H
#define TCPSOCK_H

#define BUF_SIZE 2048

class TCPSock{
private:
    int sockfd, sockNew; 
    struct sockaddr_in address;  


public:

    void port(int port)
    {
        address.sin_port = htons(port);
    }

    int getSockfd(){
        return sockfd;
    }

    TCPSock(char *host, int port){
        //int option=1;

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        //sockNew = 0;

        if(sockfd < 0)
        {
            printf("Socket creating error.\n");
            exit(EXIT_FAILURE);
        }

        address.sin_family = AF_INET;
        address.sin_port = htons(port);
        address.sin_addr.s_addr = inet_addr(host);

        if (connect(sockfd, (struct sockaddr *)&address, sizeof(address)) != 0)
        {
            printf("Socket connecting error.\n");
            exit(EXIT_FAILURE);
        }
    }
    
    std::string bytes_to_string(const std::vector<uint8_t>& bytes) {
        return std::string(bytes.begin(), bytes.end());
    }

    char* receive(long len, char* buffer)
    {
        ssize_t bytesRecv = recv(sockfd, buffer, len - 1, MSG_WAITALL);
        if (bytesRecv <= 0) {
            perror("error recving.");
            //exit(1);
        }
        buffer[bytesRecv] = '\0';

        return buffer;
    }

    void sendRequest(const char* request)
    {
        send(sockfd, request, strlen(request), 0); 
    }

    
    ~TCPSock()
    {
        //std::cout << "FK\n";
        close(sockfd);
    }
    

};

#endif //TCPSOCK_H