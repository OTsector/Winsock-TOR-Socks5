
#pragma comment(lib, "wininet.lib")
#include <windows.h> 
int main(){


	int sockett1;
	int recievedsiz;
	char bufferrecv[256];
	char headerbuf[256 * 2];
	WSADATA wsadat;
	if (WSAStartup(MAKEWORD(2, 2), &wsadat) == 0)
	{


		if ((sockett1 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) != INVALID_SOCKET)
		{
			struct sockaddr_in addressinfo;
			addressinfo.sin_family = AF_INET;
			addressinfo.sin_port = htons(9050);
			addressinfo.sin_addr.s_addr = inet_addr("127.0.0.1");



			if (connect(sockett1, (struct sockaddr *)&addressinfo, sizeof(addressinfo)) == 0)
			{
				char socks5buf[] =
				{
					0x05,
					0x01,
					0x00
				};


				send(sockett1, socks5buf, sizeof(socks5buf), 0);


				recievedsiz = recv(sockett1, bufferrecv, 256, 0);
				if (recievedsiz > 0)
				{


					if (bufferrecv[0] != 0x05) {
						goto end_connection;
					}

					if (bufferrecv[1] == 0x02) {
						goto end_connection;
					}
					if (bufferrecv[1] == 0x00)
					{


						char domainon[] = "memes.onion";
						short portdomain = 80; // might be 443 also 
						int onionsize = strlen(domainon);
						char bufferstep2socks5[256] = { 0x05,0x01,0x00,0x03 };
						int n = 4;
						bufferstep2socks5[n] = strlen(domainon);
						n++;
						RtlCopyMemory(&bufferstep2socks5[n], domainon, onionsize);
						n += onionsize;
						bufferstep2socks5[n] = (portdomain >> 8) & 0xFF; n++;
						bufferstep2socks5[n] = portdomain & 0xFF;
						if (send(sockett1, bufferstep2socks5, n + 1, 0) == SOCKET_ERROR) {
							goto end_connection;
						}
						memset(bufferrecv, '\0', 256);
						recievedsiz = recv(sockett1, bufferrecv, 10, 0);
						if (recievedsiz > 0)
						{
							wsprintf(headerbuf, "POST / HTTP/1.1\r\n Host: memes.onion\r\n\r\n");
							if (send(sockett1, headerbuf, strlen(headerbuf), 0) == SOCKET_ERROR) {
								goto end_connection;
							}
							RtlSecureZeroMemory(bufferrecv, sizeof(bufferrecv));



							while (recv(sockett1, bufferrecv, 256, 0) > 0) {

// do stuff


							}

						}

						goto end_connection;
					}
				}
			}

		end_connection:

			closesocket(sockett1);

		}
		WSACleanup();
	}
	}
