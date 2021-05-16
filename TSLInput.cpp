#include<stdio.h>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define BUFLEN 20	//Max length of buffer
#define PORT 8888	//The port on which to listen for incoming data

#include "TSLInput.h"

TSLInput::TSLInput()
{
	struct sockaddr_in server;
	WSADATA wsa;

	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");

	//Create a socket
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}
	printf("TSL socket created.\n");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	//Bind
	if (bind(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	puts("Bind done");
}

TSLInput::~TSLInput()
{
	closesocket(s);
	WSACleanup();
}

TallyState TSLInput::listen() {
	char buf[BUFLEN];
	int slen, recv_len;
	struct sockaddr_in si_other;

	slen = sizeof(si_other);

	//keep listening for data
	printf("Waiting for data...");
	fflush(stdout);

	//clear the buffer by filling null, it might have previously received data
	memset(buf, '\0', BUFLEN);

	//try to receive some data, this is a blocking call
	if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr*)&si_other, &slen)) == SOCKET_ERROR)
	{
		printf("recvfrom() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	//print details of the client/peer and the data received
	printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));

	return this->parse_packet(buf);
}

TallyState TSLInput::parse_packet(char* buf)
{
	// TSL v3.1 documentation: https://tslproducts.com/media/1959/tsl-umd-protocol.pdf
	// Tally 1 = preview
	// Tally 2 = program

	TallyState state;

	state.id = buf[0] & 0b01111111;
	printf("TSL address: %u\n", state.id);

	char tallystate = buf[1] & 0b00000011;
	printf("TSL state: %u\n", tallystate);

	state.program = buf[1] & 0b00000010 ? true : false;
	state.preview = buf[1] & 0b00000001 ? true : false;

	if (state.program) {
		printf("Program\n");
	}
	if (state.preview) {
		printf("Preview\n");
	}
	if (!state.preview && !state.program) {
		printf("Off air\n");
	}

	char label[17];
	memcpy(label, buf + 2, 16);
	label[16] = 0; // Add 0 terminator
	printf("TSL label: %s\n", label);

	state.label.assign(label);

	printf("Packet parsed\n");

	return state;
}
