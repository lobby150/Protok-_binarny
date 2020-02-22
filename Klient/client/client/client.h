//_________________________KLIENT - NAG£ÓWEK_________________________//
#pragma once
//_________________________BIBLIOTEKI________________________________//
#include"pch.h"
#include <cstdio>
#include <iostream>
#include <ctime>
#include <string>
#include <bitset>
#include <conio.h>
#include <thread>
#include "Zamiany.h"

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

using namespace std;

class client
{
private:
	WSADATA wsaData;
	SOCKET client_socket;
	SOCKADDR_IN dest_addr;

	char buffer[128];
	int sin_size = sizeof(sockaddr_in);
	int identyfikator = 4;
	string text;
	int zadanie;
	int buffer_size;

	bitset<3> oper;
	bitset<8> id;
	bitset<32> dlu;
	bitset<512> message;
	bitset<3> odp;
	bitset<2> uzupelnienie;

	void pack();		// zrobione
	void unpack();		// zrobione
	void read();		// zrobione
	void clean();		// zrobione
	int bitTOint(const string &a);		// zrobione

	void connection();		// (rozmowa) zrobione
	void receive_message(SOCKET client_socket, char buffer[70], SOCKADDR_IN dest_addr, int sin_size);		// zrobione
	void save_content(string txt);		// zrobione

	void demand_connection();		// zrobione
	void demand_id();				// zrobione
	void demand_invitation();		// zrobione
	void agree_inv();				// zrobione
	void disagree_inv();			// zrobione
	void demand_sending();			// zrobione
	void confirm_sending();			// zrobione
	void demand_disconnect();		// zrobione


public:
	int UDP();
};

/*_______________________KODY______________________________//
	DZIA£ANIE -> XXX
	operacja - xxx

	-> PO£¥CZ - 000 (0)
		- ¿¹danie po³¹czenia - 000 (0) [KLIENT]
		- przyjêcie po³¹czenia - 001 (1) [SERWER]
		- odmowa po³¹czenia - 010 (2) [SERWER]
		- b³¹d serwera - 011 (3) [SERWER]
		- wydanie ID - 101 (4) [SERWER]
	-> ZAPROSZENIE - 001 (1)
		- przyjêcie zaproszenia - 001 (1) [KLIENT]
		- odrzucenie zaproszenia - 010 (2) [KLIENT]
	-> WYŒLIJ - 010 (2)
		- potwierdzenie - 001 (1) [KLIENT]
	-> ZAKOÑCZ - 011 (3)
		- odpowiedŸ na zakoñczenie - 001 (1) [SERWER]
___________________________________________________________*/