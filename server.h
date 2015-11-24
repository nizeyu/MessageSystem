#ifndef SERVER_HH_
#define SERVER_HH_
#include "winsock2.h"
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
using namespace std;
class Server {
private:
    WSADATA         wsd;            //WSADATA varity
    SOCKET          sServer;        //server socket
    SOCKET          sClient;        //clint socket
    SOCKADDR_IN     addrServ;      //server address
    char            buf[64];  //buff for input
    char            sendBuf[64];//data of return
    int             retVal;         //return value
    int port;
    //initial wasa

public:
	~Server() {
    closesocket(sServer);  //close socket
    closesocket(sClient);   //close socket
    WSACleanup();           //release socket resource


	}
	Server(int port) :port(port){
	}
    int initial()
    {

        if (WSAStartup(MAKEWORD(2,2), &wsd) != 0)
        {
            cout << "WSAStartup failed!" << endl;
            return 1;
        }

        //crate socket
        sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(INVALID_SOCKET == sServer)
        {
            cout << "socket failed!" << endl;
            WSACleanup();//release resource
            return  -1;
        }

        //creat server address   is 4999
        addrServ.sin_family = AF_INET;
        addrServ.sin_port = htons(port);
        addrServ.sin_addr.s_addr = INADDR_ANY;
        //bind socket
        retVal = bind(sServer, (LPSOCKADDR)&addrServ, sizeof(SOCKADDR_IN));
        if(SOCKET_ERROR == retVal)
        {
            cout << "bind failed!" << endl;
            closesocket(sServer);   //close socket
            WSACleanup();           //release socket resource
            return -1;//Author: Xuan Li
        }
        return 0;
    }
    int listening(){
        //begin listening
        retVal = listen(sServer, 1);
        if(SOCKET_ERROR == retVal)
        {
            cout << "listen failed!" << endl;
            closesocket(sServer);   //close socket
            WSACleanup();          //release socket resource
            return -1;
        }

        //accept application from client
        sockaddr_in addrClient;
        int addrClientlen = sizeof(addrClient);
        sClient = accept(sServer,(sockaddr FAR*)&addrClient, &addrClientlen);
        if(INVALID_SOCKET == sClient)
        {
            cout << "accept failed!" << endl;
            closesocket(sServer);   //close socket
            WSACleanup();              //release socket resource
            return -1;
        }
    }
    int connection(){
        while(true){
            //recpet data from client
            ZeroMemory(buf, 64);
            retVal = recv(sClient, buf, 64, 0);
            if (SOCKET_ERROR == retVal)
            {
                cout << "recv failed!" << endl;
                closesocket(sServer);  //close socket
                closesocket(sClient);  //close socket
                WSACleanup();            //release socket resource
                return -1;
            }
            if(buf[0] == '0')
                break;
            cout << "data from client is : " << buf <<endl;

            cout << "please input data to client: " ;
            cin >> sendBuf;

            send(sClient, sendBuf, strlen(sendBuf), 0);
        }

        return 0;
    }

};
#endif

