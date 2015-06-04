#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

int main(int argc, char* argv[])
{
	//----------------------
	// Initialize Winsock.
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR) {
		wprintf(L"WSAStartup failed with error: %ld\n", iResult);
		return 1;
	}
	//----------------------
	// Create a SOCKET for listening for
	// incoming connection requests.
	SOCKET SrvSocket;
	SrvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (SrvSocket == INVALID_SOCKET) {
		wprintf(L"socket failed with error: %ld\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port for the socket that is being bound.
	sockaddr_in addrServer;
	addrServer.sin_family = AF_INET;
	addrServer.sin_addr.s_addr = htonl(INADDR_ANY); //ʵ������0
	addrServer.sin_port = htons(20131);


	//���׽��ֵ�һ��IP��ַ��һ���˿���
	if (bind(SrvSocket, (SOCKADDR *)& addrServer, sizeof(addrServer)) == SOCKET_ERROR) {
		wprintf(L"bind failed with error: %ld\n", WSAGetLastError());
		closesocket(SrvSocket);
		WSACleanup();
		return 1;
	}

	//��һ������ѭ���ķ�ʽ����ͣ�ؽ��տͻ���socket����
	while (1)
	{
		//���ջ������Ĵ�С��1024���ַ�
		char recvBuf[2048 + 1];

		struct sockaddr_in cliAddr;          //����IPV4��ַ����
		int cliAddrLen = sizeof(cliAddr);
		int count = recvfrom(SrvSocket, recvBuf, 2048, 0, (struct sockaddr *)&cliAddr, &cliAddrLen);
		if (count == 0)break;//���Է��ر�
		if (count == SOCKET_ERROR)break;//����count<0
		recvBuf[count] = '\0';
		printf("client IP = %s,�˿ڣ�%d\n", inet_ntoa(cliAddr.sin_addr),ntohs(cliAddr.sin_port)); //��ʾ�������ݵ�IP��ַ
		printf("%s\n", recvBuf);
		//����udp��ֻ��һ�γɹ�
		//���յ����ַ���ת��Ϊ��д�ַ���
		strupr(recvBuf);
		if (sendto(SrvSocket, recvBuf, count, 0, (struct sockaddr *)&cliAddr, cliAddrLen)<count)
			break;
	}

	closesocket(SrvSocket);
	WSACleanup();
	return 0;
}

