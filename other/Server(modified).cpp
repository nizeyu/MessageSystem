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
#pragma warning(disable:4996)
#pragma comment (lib, "ws2_32.lib")  //load ws2_32.dll
#define BUF_SIZE 100

using namespace std;
// create  receive  message thread
vector< Client > clients;
DWORD WINAPI ClientThread(LPVOID ssAccept)
{
	SOCKET sAccept = (SOCKET)ssAccept;
	//cout<<sAccept<<endl;
	int RET = 0;
	char RecvBuffer[BUF_SIZE];
	while (true) {
		memset(RecvBuffer, 0x00, BUF_SIZE);
		RET = recv(sAccept, RecvBuffer, BUF_SIZE, 0);
		if (RET == 0 || RET == SOCKET_ERROR)
		{
			break;
		}
		cout << "message is " << RecvBuffer << endl;
		int destnation = RecvBuffer[0];
		send(clients[destnation].getClientSOCKET(), RecvBuffer, strlen(RecvBuffer), 0);  //send data to client
	}
	// closesocket(sAccept);

	return 0;
}
int main() {
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
	SOCKET        servSock, sAccept;

	servSock = socket(AF_INET, SOCK_STREAM, 0);
	if (servSock == INVALID_SOCKET)
	{
		printf("socket failed:%d\n", WSAGetLastError());
		return 0;
	}
	//bind socket
	sockaddr_in sockAddr;
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
	sockaddr_in clntAddr;
	int nSize = sizeof(clntAddr);
	char client_ip[20];
	unsigned client_port = 0;
	char buffer[BUF_SIZE] = { 0 };  //create buffer area
	cout << "the server has running" << endl;

	cout << "server is  listening at port 5000" << endl;
	int id = 0;
	while (1) {
		sAccept = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);

		if (sAccept == INVALID_SOCKET)
		{
			printf("accept()failed:%d\n", WSAGetLastError());
			break;
		}
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
		client_port = ntohs(sockAddr.sin_port);
		//cout<<"client "<<client_ip<<":"<<client_port<<endl;   // some wrong in this code
		printf("accepted client IP:[%s],port:[%d]\n", inet_ntoa(clntAddr.sin_addr), ntohs(sockAddr.sin_port));
		HANDLE WINAPI  hThread = CreateThread(NULL, 0, ClientThread, (LPVOID)sAccept, 0, NULL);
		if (hThread == NULL)
		{
			cout << "creat thread failed" << endl;
			break;
		}
		Sleep(1000);
		CloseHandle(hThread);
		//  int strLen = recv(sAccept, buffer, BUF_SIZE, 0);  //
		//  printf("Message form client: %s\n", buffer);
		//  printf("Input a string: ");
		// gets(buffer);
		//send(sAccept, buffer, strlen(buffer), 0);  //send data to client


		//close socket
		memset(buffer, 0, BUF_SIZE);  //reset buffer area

	}
	//close socket
	closesocket(sAccept);
	closesocket(servSock);
	//stop using dll
	WSACleanup();

	return 0;
}
