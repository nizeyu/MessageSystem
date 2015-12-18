/*********************************************************************************
 *Copyright    Xuan Li, Zeyu Ni, Shuo Zhang
 *FileName:  server.cpp
 *Author:  Xuan Li, Zeyu Ni, Shuo Zhang
 *Version:  1.0
 *Date:  2015.12.2
 *Description:  The server program can:
                1. receive message from client and sent to the appointed client
                2. receive broadcasting request client and broadcast the message to all users online.
                3. sent file to client
 *Others:  none
 **********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>   //for socket
#include "Client.h"
#include <iostream>     //input and output
#include <windows.h>
#include <sstream>
#include <vector>
#include <string>
#include <time.h>
#include <iterator>

#pragma warning(disable:4996)
#pragma comment (lib, "ws2_32.lib")  //load ws2_32.dll
using namespace std;
#define BUF_SIZE 100

/**
 * @Class：Server
 * @description: This class can complete most of the work for server, include: message sending and receiving, broadcast,users' log in.
                 The method object include:
                 1."broadcast" for accepting the request from client and sending to all users.
                 2."p2p" for transmitting message to client
                 3."opennew" for opening different thread to different clients
                 4."ClientThread" for handle different works, inclide: transmit message, broadcast and transfer file.
 * @author: Xuan Li, Zeyu Ni, Shuo Zhang
 * @version: 1.0,
 * @date: 12/3/2015
 */
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

	/**
    * @function: broadcast
    * @description: handle users broadcast request and sent the message to all users
    * @return: void
    * @author: Xuan Li, Zeyu Ni, Shuo Zhang
    * @version: 1.0
    * @date: 12/6/2015
    */
	static void broadcast(SOCKET sAccept)
	{
		int RET = 0;
		char RecvBuffer[BUF_SIZE];//create receive buffer
        int j = 0;
			memset(RecvBuffer, 0x00, BUF_SIZE);//clear 0
			RET = recv(sAccept, RecvBuffer, BUF_SIZE, 0);//receive message from client
			for (auto i = clients.begin(); i != clients.end(); i++)//loop to sent the message to all users online
			{
				send(clients[j].getClientSOCKET(), RecvBuffer, strlen(RecvBuffer), 0);
				j++;
			}
	}

	/**
    * @function: p2p
    * @description: receiving message from client and sent to appointed client
    * @return: void
    * @author: Xuan Li, Zeyu Ni, Shuo Zhang
    * @version: 1.0
    * @date: 12/6/2015
    */
	static void p2p(SOCKET sAccept,int destnation)
	{
		int RET = 0;
		char RecvBuffer[BUF_SIZE];
		int j = 0;//?????
		while (true) {
			memset(RecvBuffer, 0x00, BUF_SIZE);//clear 0 for buffer
			RET = recv(sAccept, RecvBuffer, BUF_SIZE, 0);//receive message from client
			if (RET == 0 || RET == SOCKET_ERROR)//if has no message of error message, jump out loop
			{
				break;
			}
			cout << "message is " << RecvBuffer << endl;//output the message

			send(clients[destnation].getClientSOCKET(), RecvBuffer, strlen(RecvBuffer), 0);  //send data to client
		}
	}

	/**
    * @function: transfer
    * @description: receiving file from client and sent the file to appointed client
    * @return: void
    * @author: Xuan Li, Zeyu Ni, Shuo Zhang
    * @version: 1.0
    * @date: 12/6/2015
    */
	static void transfer(SOCKET sAccept,SOCKET sSend){

        int RET = 0;
		char RecvBuffer[BUF_SIZE];
		char file_name[BUF_SIZE];
        memset(RecvBuffer, 0x00, BUF_SIZE);//clear 0 for receiving buffer
         memset(file_name, 0x00, BUF_SIZE);//clear 0 for file name
        RET = recv(sAccept, RecvBuffer, BUF_SIZE, 0);//receiving file name from client
        strncpy(file_name, RecvBuffer, BUF_SIZE );//copy the receiving buffer to file name
        memset(RecvBuffer, 0x00, BUF_SIZE);//clear o for receiving buffer
        cout << "xxxxxxxxxxxxx";

        RecvBuffer[0] = '!';
        RecvBuffer[1] = '-';
        RecvBuffer[2] = '@';//set receiving buffer as "!-@"
        send(sSend, RecvBuffer, strlen(RecvBuffer), 0);//send "!-@" to client indicate allowing file transfer
        memset(RecvBuffer, 0x00, BUF_SIZE);//clear 0 for receiving buffer
        int length = 0;
        send(sSend, file_name, strlen(file_name), 0);//send
           while(length = recv(sAccept, RecvBuffer, BUF_SIZE, 0))
            {
            if (length < 0)
            {
                printf("Recieve Data From Server failed!\n");
                break;
            }
            send(sSend, RecvBuffer, strlen(RecvBuffer), 0);  //send file to client
                 memset(RecvBuffer, 0x00, BUF_SIZE);
        }
        send(sSend, RecvBuffer, strlen(RecvBuffer), 0);  //send file to client
        printf("Recieve File:\t %s Finished!\n", file_name);
	}

	/**
    * @function: ClientThread
    * @description: handle users broadcast request, sent the message to all users, and file transfer to appointed CtC
    * @return: ClientThread
    * @author: Xuan Li, Zeyu Ni, Shuo Zhang
    * @version: 1.0
    * @date: 12/6/2015
    */
	static DWORD WINAPI ClientThread(LPVOID ssAccept)
	{

		SOCKET sAccept = (SOCKET)ssAccept;
		//cout<<sAccept<<endl;
		int RET = 0;
		char RecvBuffer[BUF_SIZE];
		while(1){
		RET = recv(sAccept, RecvBuffer, BUF_SIZE, 0);
		int destnation = atoi(RecvBuffer);
		if (destnation == 999) {//if client input "999", conduct broadcast
			Server::broadcast(sAccept);
			//return 0;
		}
		else if (destnation == 998) {//if client input "999", conduct file transfer
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

    /**
    * @function: opennew
    * @description: opening different thread to different clients
    * @return: void
    * @author: Xuan Li, Zeyu Ni, Shuo Zhang
    * @version: 1.0
    * @date: 12/6/2015
    */
	void opennew() {
	     hThread = CreateThread(NULL, 0, Server::ClientThread, (LPVOID)sAccept, 0, NULL);

		Sleep(1000);

		//  int strLen = recv(sAccept, buffer, BUF_SIZE, 0);  //
		//  printf("Message form client: %s\n", buffer);
		//  printf("Input a string: ");
		// gets(buffer);
		//send(sAccept, buffer, strlen(buffer), 0);  //send data to client


		//close socket
	}

	/**
    * @function: login
    * @description: handle users login in following steps:
                    1.receive username from users and store in Client
                    2.send the name and ID list of online users to client
    * @return: void
    * @author: Xuan Li, Zeyu Ni, Shuo Zhang
    * @version: 1.0
    * @date: 12/2/2015
    */
	void login()
	{
			sAccept = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);

			Client c;
			srand((unsigned int)time(0));
			int recvname;//create for receive socket
			char usename[20];//create to record username
			memset(usename, 0x00, 20);//clear memory for username
			recvname = recv(sAccept, usename, 20, 0);//receive  username from client//clinets[clinetNumber] = sAccept;
			c.setClientSOCKET(sAccept);//record the new username to accept
			c.setId(id);//create ID for every user automatically: clinetNumber++;
			id++;//the first client has id 1 and second has id 2 and keep on
			c.setUserName(usename);//record the new username
			clients.push_back(c);//new user record to name&ID list
			cout << clients.size() << endl;
			int j = 0;
			string allname;//create string to record name&ID list
			for (auto i = clients.begin(); i != clients.end(); i++)
			{
				allname += char(clients[j].getId() + 48);
				allname += ' ';
				allname += clients[j].getUsername();
				allname += ';';
				j++;
			}//make a string contain id+username and send back to user
			//to show all clients in the system for user to choose to whom to send
			const char * idname = new char[BUF_SIZE];
			idname = allname.c_str();
			send(sAccept, idname, BUF_SIZE, 0);//send the name&ID list to client
			unsigned client_port = 0;
			client_port = ntohs(sockAddr.sin_port);//get client ip and port
			printf("accepted client IP:[%s],port:[%d]\n", inet_ntoa(clntAddr.sin_addr), ntohs(sockAddr.sin_port));
	}
};

/**
 * @Method: init
 * @description: initiation in the following step
                 1.initial DLL, set IPv4, IP address of server, port number and sending buffer.
                 2.binding the socket with IP and port
                 3.going to listening state
 * @author: Xuan Li, Zeyu Ni, Shuo Zhang
 * @version: 1.0
 * @date: 12/2/2015
 */
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

	//bind socket with IP and port
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;  //using ipv4 adress
	sockAddr.sin_addr.s_addr = INADDR_ANY;// set IP Address
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
	 //receive the request from client
	 //waiting and accept client connect request...
	cout << "the server has running" << endl;
	cout << "server is  listening at port 5000" << endl;
}

	int main() {
		Server server;
		int test=server.init();
		if (test == 0) return 0;
		while (1) {
			server.login();
			cout << "123123";
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
