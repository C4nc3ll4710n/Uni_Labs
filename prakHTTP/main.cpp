#include "HTTPCli.hpp"
#include "TCPSock.hpp"
#include <cerrno>
#include "thread.h"
#include "mutex.h"
#include <sstream>

mutex fileWriteMutex;



int getContentLength(char* resp) {
    const char *pch = strstr(resp, "ength: ");
    if (pch != NULL) {
        pch += strlen("ength: ");
        int contentLength = atoi(pch);
        return contentLength;
    }

    return -1;

}

struct threadArgs{
    HTTPCli* cli;
    long chSize;
    long respLn;
    long contSize;
    unsigned long id;
    std::string filename;
    std::ofstream& out;


    threadArgs(HTTPCli* clint, long chSz, long rspLn, long cntSz, unsigned long tid, std::string flnm, std::ofstream& outp)
        :cli(clint), chSize(chSz), filename(flnm), respLn(rspLn), contSize(cntSz), id(tid), out(outp){};

};


void writeToFile(void *arg) {
    threadArgs ppair = *static_cast<threadArgs*>(arg);
    std::cout << "SIZES!" << ppair.chSize << " " << ppair.respLn<< "\n";
    char p[ppair.chSize+ppair.respLn];
    
    ppair.cli->recvDATA(ppair.chSize+ppair.respLn, ppair.filename, p);
    char *ans = p+ppair.respLn;
    
    for (int i=0; i<ppair.chSize; i++){
        std::cout << ans[i] << "\n";
    }

    fileWriteMutex.lock();

    std::cout << ppair.out.is_open() << "TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT\n";
    ppair.out.write(ans, ppair.chSize);
    

    fileWriteMutex.unlock();

}

void writeToFileMul(void *arg) {
    
    threadArgs ppair = *static_cast<threadArgs*>(arg);
    //std::cout << ppair.out.is_open() << "ISOPENISOPENISOPEN\n";
    
    char p[ppair.contSize+ppair.respLn];
    
    
    /*for (int i=0; i<ppair.chSize; i++){
        std::cout << ans[i] << "\n";
    }*/
    
    fileWriteMutex.lock();
        ppair.cli->recvDATA(ppair.contSize+ppair.respLn, ppair.filename, p);
        char *ans = p+ppair.respLn + ppair.id*ppair.chSize; 
        ans[ppair.chSize] = '\0';
        
        //std::cout << "pos: " << ppair.id*ppair.chSize << "\n";
        /*for (int i=0; i<ppair.chSize; i++){
            std::cout << ans[i] << "\n";
        }*/
        ppair.out.seekp(ppair.id*ppair.chSize);
        (ppair.out).write(ans, ppair.chSize);

    fileWriteMutex.unlock();

}




std::string pathToDomain(char *path){
    std::string name = path;
    size_t n = name.find('/');
    if (n != std::string::npos)
        name = name.substr(n+2);
    size_t m = name.find('/');
    if (m != std::string::npos)
        name = name.substr(0, m);
    return name;
}

std::string pathToFilename(char *path){
    std::string name = path;
    size_t n = name.rfind('/');
    if (n != std::string::npos)
        name = name.substr(n+1);
    return name;
}


std::string getIPFromURL(const std::string& url) {
        if (url =="localhost"){
            return "127.0.0.1";
        }
        else{
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

    }

int main(int argc, char **argv)
{
    if (argc != 2 && argc != 4) {
        fprintf(stderr, "usage: %s <URL> [<threads>]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    std::cout << pathToDomain(argv[1]) << "\n";

    size_t len = getIPFromURL(pathToDomain(argv[1])).length();
    char* host = new char[len + 1];
    strncpy(host, getIPFromURL(pathToDomain(argv[1])).c_str(), len + 1);

    std::string filename = pathToFilename(argv[1]);

    //std::cout << "\n";
    int port = 80;
    HTTPCli client(host, port);
    

    
    char resp[BUF_SIZE];
    client.recvHEAD(filename, resp);
    //std::cout << resp<< "\n";
    //parseTable(resp);
    int respLen = 0;
    for (int i=0; i<BUF_SIZE; i++){
        if (resp[i]=='\0')
            break;
        //std::cout << "r: " <<resp[i] << "\n";
        respLen+=1;
    }
    //std::cout << "RESPLEN: " << respLen << "\n";


    int contLen = getContentLength(resp);
    //contLen = 5;

    
    //std::cout << "CONTENT LENGTH:"<<contLen << "\n";
    if (contLen == -1) {
        std::cerr << "Error: Content-Length header not found" << std::endl;
        exit(EXIT_FAILURE);
    }
    
    std::ofstream file;
    file.open("output.txt");
    //std::cout << file.is_open() << "ISOPENISOPENISOPEN\n";
    //std::ofstream& out = file; 

    if (argc == 2) { // -j 5
        //std::cout << "CONTLEN: "<<contLen << "\n";
        //std::cout << "RESPLEN: " << respLen << "\n";
        threadArgs trAgr(&client, contLen, respLen, contLen, 1, filename, file);
        //std::cout << "RESPLN: " << trAgr.respLn << "\n";
        thread t(writeToFile, static_cast<void*>(&trAgr));
        std::cout << "error" << 11 << "\n";
        t.join();
        file.close();

        return 0;
    }

    int threadsNum = atoi(argv[3]);
    if (threadsNum <= 0) {
        std::cerr << "Error: Invalid number of threads" << std::endl;
        exit(EXIT_FAILURE);
    }

    int chunkSize = contLen / threadsNum;
    //std::cout << chunkSize << "\n";
    //std::cout << file.is_open() << "!!!!!!!!ISOPENISOPENISOPEN\n";

    //std::vector<thread> t(threadsNum);
    thread t[threadsNum];
    //std::cout << "error" << 1<< "\n";
    for(int i = 0; i < threadsNum; ++i)
    {
        threadArgs *trAgr = new threadArgs(&client, chunkSize, respLen, contLen, i, filename, file);
        thread thr(writeToFileMul, reinterpret_cast<void*>(trAgr));
		t[i] = thr;
		thr.joinable(false);
        //t.emplace_back(writeToFileMul, reinterpret_cast<void*>(trAgr));
    }
    //std::cout << file.is_open() << "ISOPENISOPENISOPEN\n";
    //std::cout << "error" << 2 << "\n";

    for (int i = 0; i < threadsNum; ++i) {
        if(t[i].joinable())
            t[i].join();
    }
    //std::cout << "error" << 100 << "\n";
    file.close();
    return 0;

}