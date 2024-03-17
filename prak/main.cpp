#include "HTTPCli.hpp"
#include "TCPSock.hpp"
#include <cerrno>
#include "thread.h"
#include <sstream>
#include "cURLparse.hpp"



int getContentLength(char* resp) {
    const char *pch = strstr(resp, "ength: ");
    if (pch != NULL) {
        pch += strlen("ength: ");
        int contentLength = atoi(pch);
        return contentLength;
    }

    return -1;

}

void writeToFile(void *arg) {
    std::pair<HTTPCli*, int> ppair = *static_cast<std::pair<HTTPCli*, int>*>(arg);
    std::cout << "error" << 13<< "\n";
    char* p = ppair.first->recvDATA(ppair.second);
    std::cout << "error" << 3<< "\n";
    std::ofstream file("output.txt");
    std::cout << "error" << 4<< "\n";
    file.write(p, ppair.second);
    file.close();
}



std::string getIPFromURL(const std::string& url) {
        size_t start = url.find("//") + 2;
        size_t end = url.find('/', start);
        std::string host;
        if (end == std::string::npos) {
            host = url.substr(start);
        } else {
            host = url.substr(start, end - start);
        }


        struct hostent *he;
        struct in_addr **addr_list;

        if ((he = gethostbyname(host.c_str())) == NULL) {  // get the host info
            herror("gethostbyname");
            return "";
        }

        addr_list = (struct in_addr **)he->h_addr_list;

        //Return the first one;
        return inet_ntoa(*addr_list[0]);

    }

int main(int argc, char **argv)
{
    if (argc != 2 && argc != 4) {
        fprintf(stderr, "usage: %s <URL> [<threads>]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    

    //const std::string host_str(argv[1]);
    /*for (int i = 0; i < 20; i++) {
        std::cout << getIPFromURL(host_str).c_str() << "\n";
    }*/
    
    //char* host = const_cast<char*>( getIPFromURL(host_str).c_str());
    char* host = argv[1];
    /*for (int i = 0; i < 20; i++) {
        std::cout << host[i];
    }*/
    std::cout << "\n";
    int port = 8000;
    HTTPCli client(host, port);
    
    char resp[BUF_SIZE];
    memset(resp, 0, BUF_SIZE);
    strcpy(resp, client.recvHEAD());
    for (int i = 0; i < BUF_SIZE; i++) {
        std::cout << resp[i];
    }
    parseTable(resp);


    int contLen = getContentLength(resp);

    std::pair<HTTPCli*, int> oh = {&client, contLen};
    std::cout << "CONTENT LENGTH:"<<contLen << "\n";
    if (contLen == -1) {
        std::cerr << "Error: Content-Length header not found" << std::endl;
        exit(EXIT_FAILURE);
    }

   if (argc == 4) { // -j 5
        int threadsNum = atoi(argv[3]);
        if (threadsNum <= 0) {
            std::cerr << "Error: Invalid number of threads" << std::endl;
            exit(EXIT_FAILURE);
        }

        int chunkSize = contLen / threadsNum;

        thread t[threadsNum];
        std::cout << "error" << 1<< "\n";
        for (int i = 0; i < threadsNum; ++i) {
            t[i] = thread(writeToFile, static_cast<void*>(&oh));
        }
        std::cout << "error" << 2 << "\n";

        for (int i = 0; i < threadsNum; ++i) {
            t[i].join();
        }
    } else {

        std::cout << "error" << 10 << "\n";
        thread t(writeToFile, static_cast<void*>(&client));
        std::cout << "error" << 11 << "\n";
        t.join();
    }


    return 0;

}