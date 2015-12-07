#include <stdio.h>
#include <WinSock2.h>
#include <windows.h>
#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32.lib")  //load  ws2_32.dll

#define BUF_SIZE 100
HANDLE hMutex;


DWORD WINAPI Rec(LPVOID ipParameter)
{
	SOCKET sockClient = (SOCKET)ipParameter;
	char revBuf[BUF_SIZE];
	int byte = 0;

	while (1)
	{

		WaitForSingleObject(hMutex, INFINITE);
		memset(revBuf, 0, sizeof(revBuf));
		byte = recv(sockClient, revBuf, BUF_SIZE , 0);//???????????  
       
		printf("Message form server: %s\n", revBuf);
		if (byte <= 0)
		{
			break;
		}

		 
		memset(revBuf, 0, BUF_SIZE);  //reset the buffer area
		
		ReleaseMutex(hMutex);

	}
	printf("sdadad");
	closesocket(sockClient);//??socket,??????  
	
	return 0;
}

int main() {
	//initialization dll;
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//send request to server program
	sockaddr_in sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");// the server ip address is localhost
	sockAddr.sin_port = htons(5000);
	char sendBuf[BUF_SIZE];


	while (1) {
		//create socket
		SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
	
		HANDLE WINAPI hThread = CreateThread(NULL, 0, Rec, (LPVOID)sock, 0, NULL);
		if (hThread == NULL)
		{
			CloseHandle(hThread);
		}
		printf("Input a string: ");
		gets_s(sendBuf);
		send(sock, sendBuf, strlen(sendBuf), 0);
		
		//get what the user input and send is to server





		closesocket(sock);  //close socket
	}

	WSACleanup();  //stop using dll
	return 0;
}
