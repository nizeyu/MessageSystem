#include <stdio.h>
#include <WinSock2.h>
#include <windows.h>
#pragma warning(disable:4996)
#pragma comment(lib, "ws2_32.lib")  //load  ws2_32.dll

#define BUF_SIZE 100
HANDLE hMutex;
void Send(SOCKET sockClient)
{
	char sendBuf[BUF_SIZE];
	int byte = 0;

	while (1)
	{
		WaitForSingleObject(hMutex, INFINITE);
		printf("Input a string: ");
		gets_s(sendBuf);
		byte = send(sockClient, sendBuf, strlen(sendBuf) + 1, 0);//服务器从客户端接受数据  
		if (byte <= 0)
		{
			break;
		}
		Sleep(1000);
		ReleaseMutex(hMutex);

	}
	closesocket(sockClient);//关闭socket,一次通信完毕  
}

void Rec(SOCKET sockClient)
{

	char revBuf[BUF_SIZE];
	int byte = 0;

	while (1)
	{
		WaitForSingleObject(hMutex, INFINITE);

		byte = recv(sockClient, revBuf, strlen(revBuf) + 1, 0);//服务器从客户端接受数据  
		if (byte <= 0)
		{
			break;
		}

		printf("Message form server: %s\n", revBuf);
		memset(revBuf, 0, BUF_SIZE);  //reset the buffer area
		Sleep(1000);
		ReleaseMutex(hMutex);

	}
	closesocket(sockClient);//关闭socket,一次通信完毕  
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

	
	while (1) {
		//create socket
		SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
		connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
		HANDLE hThread1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Rec, (LPVOID)sock, 0, 0);
		if (hThread1 != NULL)
		{
			CloseHandle(hThread1);
		}

		HANDLE hThread2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Send, (LPVOID)sock, 0, 0);

		if (hThread2 != NULL)
		{
			CloseHandle(hThread2);
		}
		//get what the user input and send is to server
		
		
		

		
		closesocket(sock);  //close socket
	}

	WSACleanup();  //stop using dll
	return 0;
}
