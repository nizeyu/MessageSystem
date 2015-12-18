#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include "Client.h"
#include <iostream>
#include <windows.h>
#include <sstream>
#include<vector>
#include <string>
#include <time.h>
#include <iterator>
using namespace std;
#pragma warning(disable:4996)
#pragma comment (lib, "ws2_32.lib")  //load ws2_32.dll
#define BUF_SIZE 100
vector< Client > clients;
class Server
{
private:

public:
	SOCKET servSock;
	SOCKET sAccept;
	sockaddr_in sockAddr;

	sockaddr_in clntAddr;
	HANDLE WINAPI  hThread;
	int nSize = sizeof(clntAddr);
	int id = 0;
	int init();
	// create  receive  message thread
	static void broadcast(SOCKET sAccept)
	{
		int RET = 0;
		char RecvBuffer[BUF_SIZE];
        int j = 0;
			memset(RecvBuffer, 0x00, BUF_SIZE);
			RET = recv(sAccept, RecvBuffer, BUF_SIZE, 0);
			for (auto i = clients.begin(); i != clients.end(); i++)
			{
				send(clients[j].getClientSOCKET(), RecvBuffer, strlen(RecvBuffer), 0);
				j++;
			}
	}
	static void p2p(SOCKET sAccept,int destnation)
	{
		int RET = 0;
		char RecvBuffer[BUF_SIZE];
		int j = 0;
		while (true) {
			memset(RecvBuffer, 0x00, BUF_SIZE);
			RET = recv(sAccept, RecvBuffer, BUF_SIZE, 0);
			if (RET == 0 || RET == SOCKET_ERROR)
			{
				break;
			}
			cout << "message is " << RecvBuffer << endl;

			send(clients[destnation].getClientSOCKET(), RecvBuffer, strlen(RecvBuffer), 0);  //send data to client
		}
		// closesocket(sAccept);
	}


	static void transfer(SOCKET sAccept,SOCKET sSend){

        int RET = 0;
		char RecvBuffer[BUF_SIZE];
		char file_name[BUF_SIZE];
        memset(RecvBuffer, 0x00, BUF_SIZE);
         memset(file_name, 0x00, BUF_SIZE);
        RET = recv(sAccept, RecvBuffer, BUF_SIZE, 0);
        strncpy(file_name, RecvBuffer, BUF_SIZE );
        memset(RecvBuffer, 0x00, BUF_SIZE);
        cout << "xxxxxxxxxxxxx";

        RecvBuffer[0] = '!';
        RecvBuffer[1] = '-';
        RecvBuffer[2] = '@';
        send(sSend, RecvBuffer, strlen(RecvBuffer), 0);
        memset(RecvBuffer, 0x00, BUF_SIZE);
        int length = 0;
        send(sSend, file_name, strlen(file_name), 0);
           while(length = recv(sAccept, RecvBuffer, BUF_SIZE, 0))
            {
            if (length < 0)
            {
                printf("Recieve Data From Server failed!\n");
                break;
            }
            send(sSend, RecvBuffer, strlen(RecvBuffer), 0);  //send data to client
                 memset(RecvBuffer, 0x00, BUF_SIZE);
        }
        send(sSend, RecvBuffer, strlen(RecvBuffer), 0);  //send data to client
        printf("Recieve File:\t %s Finished!\n", file_name);
	}

	static DWORD WINAPI ClientThread(LPVOID ssAccept)
	{

		SOCKET sAccept = (SOCKET)ssAccept;
		//cout<<sAccept<<endl;
		int RET = 0;
		char RecvBuffer[BUF_SIZE];
		while(1){
		RET = recv(sAccept, RecvBuffer, BUF_SIZE, 0);
		int destnation = atoi(RecvBuffer);
		if (destnation == 999) {
			Server::broadcast(sAccept);
			//return 0;
		}
		else if (destnation == 998) {
            memset(RecvBuffer, 0x00, BUF_SIZE);
            RET = recv(sAccept, RecvBuffer, BUF_SIZE, 0);
            int sss = atoi(RecvBuffer);
            cout << sss;
            SOCKET sSend = clients[sss].getClientSOCKET();
            Server::transfer(sAccept, sSend);
			//return 0;

        }
		else {
			Server::p2p(sAccept, destnation);
			//return 0;
		}
		}
	}

	void opennew() {
	     hThread = CreateThread(NULL, 0, Server::ClientThread, (LPVOID)sAccept, 0, NULL);
		Sleep(1000);
	}
	void login()
	{
			sAccept = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);

			Client c;
			srand((unsigned int)time(0));
			int recvname;
			char usename[20];
			memset(usename, 0x00, 20);
			recvname = recv(sAccept, usename, 20, 0);         //receive  username from client
			c.setClientSOCKET(sAccept);
			c.setId(id);
			id++;                  //the first client has id 1 and second has id 2 and keep on
			c.setUserName(usename);
			clients.push_back(c);
			cout << clients.size() << endl;
			int j = 0;
			string allname;
			for (auto i = clients.begin(); i != clients.end(); i++)
			{
				allname += char(clients[j].getId() + 48);
				allname += ' ';
				allname += clients[j].getUsername();                     //make a string contain id+username and send back to user
				allname += ';';
				j++;
			}
			const char * idname = new char[BUF_SIZE];
			idname = allname.c_str();
			send(sAccept, idname, BUF_SIZE, 0);          //to show all clients in the system for user to choose to whom to send

														 //    clinets[clinetNumber] = sAccept;
														 //  clinetNumber++;
														 //get client ip and port
			unsigned client_port = 0;
			client_port = ntohs(sockAddr.sin_port);
			//cout<<"client "<<client_ip<<":"<<client_port<<endl;   // some wrong in this code
			printf("accepted client IP:[%s],port:[%d]\n", inet_ntoa(clntAddr.sin_addr), ntohs(sockAddr.sin_port));

	}

};



int Server::init()
{
	printf("==========================================================\n");
	printf("|This program is wirten by Xuan Li, Zeyu Ni, Shuo Zhang  |\n");
	printf("|This program  using socket lib and tcp/ip portocol      |\n");
	printf("==========================================================\n");
	WSADATA wsaData;
	HANDLE WINAPI  thread[255];    //thread
	DWORD WINAPI dwThId[255];    // thread id number
	int index = 0;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Winsock load failed\n");
		return 0;
	}
	//create socket
	servSock = socket(AF_INET, SOCK_STREAM, 0);
	if (servSock == INVALID_SOCKET)
	{
		printf("socket failed:%d\n", WSAGetLastError());
		return 0;
	}
	//bind socket

	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;  //using ipv4 adress
	sockAddr.sin_addr.s_addr = INADDR_ANY;// local
	sockAddr.sin_port = htons(5000);  //set the port
	if (bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		printf("bind()failed:%d\n", WSAGetLastError());
		return 0;
	}
	//begin listening
	if (listen(servSock, 20) == SOCKET_ERROR)
	{
		printf("listen()failed:%d\n", WSAGetLastError());
		return 0;
	}//listen and judge weather it can work
	 //revieve the request from client
	 //4,waiting and accept client connect request...
	cout << "the server has running" << endl;
	cout << "server is  listening at port 5000" << endl;
}


	int main() {
		Server server;
		int test=server.init();
		if (test == 0) return 0;
		while (1) {
			server.login();
			server.opennew();
			if (server.hThread == NULL)
			{
				cout << "creat thread failed" << endl;
				 break;
			}
		}
    CloseHandle(server.hThread);
	//close socket
	closesocket(server.sAccept);
	closesocket(server.servSock);
	//stop using dll
	WSACleanup();

	return 0;
}
