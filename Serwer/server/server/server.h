//_____________________________SERVER - NAG£ÓWEK_____________________________//
#pragma once
//_____________________________BIBLIOTEKI____________________________________//
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <ctime>
#include <string>
#include <array>
#include <vector>
#include <thread>

#include <bitset>
#include <WinSock2.h>
#include <WS2tcpip.h>

#include "Zamiany.h"

#pragma comment(lib, "Ws2_32.lib")

class server {
private:
	WSADATA wsaData;
	SOCKET server_socket;
	SOCKADDR_IN my_address, current_address;
	SOCKADDR_IN client1_address, client2_address;
	SOCKET soc;

	int sin_size = sizeof(sockaddr_in);
	char buffer[128];
	int users_counter = 0;
	std::string text;
	int buffer_size;

	std::bitset<3> oper;
	std::bitset<3> odp;
	std::bitset<32> length;
	std::bitset<512> message;
	std::bitset<2> uzupelnienie;
	std::bitset<8> id;

	void pack();
	void unpack();
	void read();
	void clean();
	int bit_to_int(const std::string &s);

	void saveContent(std::string napis);
	void acceptConnection();
	void denyConnection();
	void giveID();
	void denyID();
	void noClient();
	void denySending();
	void confirmEndingConnection();
	int rozmiar_buffera();
	int identyfikator = 4;
public:
	int UDP();
};