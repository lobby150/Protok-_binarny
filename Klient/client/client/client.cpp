//______________________KLIENT - CPP______________________//

#include"pch.h"
#include "client.h"

/*_____________________KODY_______________________________//
	DZIAŁANIE -> XXX
	operacja - xxx

	-> POŁĄCZ - 000 (0)
		- żądanie połączenia - 000 (0) [KLIENT]
		- przyjęcie połączenia - 001 (1) [SERWER]
		- odmowa połączenia - 010 (2) [SERWER]
		- błąd serwera - 011 (3) [SERWER]
		- wydanie ID - 101 (5) [SERWER]
	-> ZAPROSZENIE - 001 (1)
		- przyjęcie zaproszenia - 001 (1) [KLIENT]
		- odrzucenie zaproszenia - 010 (2) [KLIENT]
	-> WYŚLIJ - 010 (2)
		- potwierdzenie - 001 (1) [KLIENT]
	-> ZAKOŃCZ - 011 (3)
		- odpowiedź na zakończenie - 001 (1) [SERWER]
___________________________________________________________*/

int client::UDP()
{
	// INICJALIZUJEMY WINSOCK

	WSAStartup(MAKEWORD(2, 2), &wsaData);						// winsock version 2.2
	client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);	// rodzina adresów IPv4 / gniazdo datagramowe / UDP
	dest_addr.sin_family = AF_INET;								// rodzina IPv4
	dest_addr.sin_port = htons(28027);							// host to network short / nr portu
	inet_pton(AF_INET, "127.0.0.1", &dest_addr.sin_addr);		// host lokalny
	sockaddr_in current_address;
	memset(&current_address, 0, sizeof(current_address));
	inet_pton(AF_INET, "127.0.0.1", &current_address.sin_addr);
	current_address.sin_port = htons(28027);
	// CONNECTION ATTEMPT

	cout << "Attempting to connect...\n";
	demand_connection();
	sendto(client_socket, buffer, 6, 0, (sockaddr*)&dest_addr, sin_size);

	// RECEIVING INFORMATION/MESSAGES

	thread RECEPTION(&client::receive_message, this, client_socket, buffer, dest_addr, sin_size);
	RECEPTION.detach();

	while (1)
	{
		if (oper == 0 && odp == 0) {}

		if (oper == 0 && odp == 1)
		{
			cout << "-NAWIAZANO POLACZENIE-\n";
			cout << "-ZADAMY ID-\N";
			demand_id();
			sendto(client_socket, buffer, 6, 0, (sockaddr*)&dest_addr, sin_size);
		}

		if (oper == 0 && odp == 2)
		{
			cout << "-ODMOWA NAWIĄZANIA POŁĄCZENIA-\n";
			cout << "-CO ROBIMY? \n [1] sprobuj ponownie \n2[2] Rozlacz\n";

			int reaction;
			reaction = _getch();

			if (reaction == '1')
			{
				cout << "Attempting to reconnect...\n";
				clean();
				demand_connection();
				pack();
				sendto(client_socket, buffer, 6, 0, (sockaddr*)&dest_addr, sin_size);
			}
			if (reaction == '2')
			{
				cout << "Demanding disconnect...\n";
				clean();
				demand_disconnect();
				pack();
				sendto(client_socket, buffer, 6, 0, (sockaddr*)&dest_addr, sin_size);
			}
		}

		if (oper == 0 && odp == 3)
		{
			cout << "-ODMOWA NAWIĄZANIA POŁĄCZENIA-\n";
			cout << "-CO ROBIMY? \n [1] sprobuj ponownie \n2[2] Rozlacz\n";

			int reaction;
			reaction = _getch();

			if (reaction == '1')
			{
				cout << "Attempting to reconnect...\n";
				clean();
				demand_connection();
				pack();
				sendto(client_socket, buffer, 6, 0, (sockaddr*)&dest_addr, sin_size);
			}
			if (reaction == '2')
			{
				cout << "Demanding disconnect...\n";
				clean();
				demand_disconnect();
				pack();
				sendto(client_socket, buffer, 6, 0, (sockaddr*)&dest_addr, sin_size);
			}
		}

		if (oper == 0 && odp == 4) {}

		if (oper == 0 && odp == 5)
		{
			cout << "PRZYDZIELONO ID\n";
			cout << "CO ROBIMY?\n [1] Zaproszenie do rozmowy\n [2] Czekaj\n";

			int reaction;
			reaction = _getch();

			if (reaction == '1')
			{
				cout << "Zapraszanie innego uzytkownia do rozmowy!\n";
				clean();
				demand_invitation();
				pack();
				sendto(client_socket, buffer, 6, 0, (sockaddr*)&dest_addr, sin_size);

				while (oper == 1 && odp == 0) {}
			}
			if (reaction == '2')
			{
				clean();

				while (oper != 1 && odp == 0) {}
			}
		}

		if (oper == 1 && odp == 0)
		{
			cout << "-OTRZYMANO ZAPROSZENIE DO ROZMOWY-\N";
			cout << "CO ROBIMY?\n [1] Przyjmij\n [2] Odrzuc\n [3] Rozlacz-nieaktywne\n";

			int reaction;
			reaction = _getch();

			if (reaction == '1')
			{
				cout << "-PRZYJMOWANIE ZAPROSZENIA-\n";
				clean();
				agree_inv();
				pack();
				sendto(client_socket, buffer, 6, 0, (sockaddr*)&dest_addr, sin_size);
				connection();

				// while (oper == 1 && odp == 1) {}
			}
			if (reaction == '2')
			{
				cout << "-ODRZUCANIE ZAPROSZENIA-\n";
				clean();
				disagree_inv();
				pack();
				sendto(client_socket, buffer, 6, 0, (sockaddr*)&dest_addr, sin_size);

				while (oper == 1 && odp == 2) {}
			}
			/*
			if (reaction == '3')
			{
				cout << "-ŻĄDANIE ZAKOŃCZENIA POŁĄCZENIA";
				clean();
				demand_disconnect();
				pack();
				sendto(client_socket, buffer, 6, 0, (sockaddr*)&dest_addr, sin_size);
			}
			*/
		}
		if (oper == 1 && odp == 1) {
			std::cout << "<----uzytkownik przyjal zaproszenie--->\n";

			connection();
		}
		if (oper == 1 && odp == 2)
		{
			cout << "-USER ODRZUCIL ZAPROSZENIE-\n";
			cout << "CO ROBIMY?\n [1] zapros ponownie\n [2] Rozlacz\n";

			int reaction;
			reaction = _getch();

			if (reaction == '1')
			{
				cout << "Trwa ponowne zapraszanie...\n";
				clean();
				demand_invitation();
				pack();
				sendto(client_socket, buffer, 6, 0, (sockaddr*)&dest_addr, sin_size);
			}
			if (reaction == '2')
			{
				cout << "-ŻĄDANIE ZAKOŃCZENIA POŁĄCZENIA-\n";
				clean();
				demand_disconnect();
				pack();
				sendto(client_socket, buffer, 6, 0, (sockaddr*)&dest_addr, sin_size);
			}
		}

		if (oper == 1 && odp == 3)
		{
			cout << "-NIE UDALO SIE ZAPROSZENIE-\n";
			cout << "CO ROBIMY?\n [1] zapros ponownie\n [2] Rozlacz\n";

			int reaction;
			reaction = _getch();

			if (reaction == '1')
			{
				cout << "Trwa ponowne zapraszanie...\n";
				clean();
				demand_invitation();
				pack();
				sendto(client_socket, buffer, 6, 0, (sockaddr*)&dest_addr, sin_size);
			}
			if (reaction == '2')
			{
				cout << "-ŻĄDANIE ZAKOŃCZENIA POŁĄCZENIA-\n";
				clean();
				demand_disconnect();
				pack();
				sendto(client_socket, buffer, 6, 0, (sockaddr*)&dest_addr, sin_size);
			}
		}



		int i = sizeof(dest_addr);
	}
	closesocket(client_socket);				// ZAMKNIĘCIE GNIAZDA
	WSACleanup();							// SPRZĄTANKO
	cout << "Koniec" << endl;
	return 1;
}
// =================================================================
void client::pack()
{
	string pom;

	pom = oper.to_string();
	pom = pom + odp.to_string();
	pom = pom + dlu.to_string();
	pom = pom + id.to_string();
	pom = pom + message.to_string();
	pom = pom + uzupelnienie.to_string();
	//pom = pom + id.to_string();

	for (int i = 0; i < 70; i++)
	{
		string spakowane = pom.substr(i * 8, 8);		// przesyłanie po bajcie
		buffer[i] = bitTOint(spakowane);
	}
}
// ================================================================
void client::unpack()
{
	string pom;

	for (int i = 0; i < 70; i++)
	{
		pom += charto2(buffer[i]);
	}

	oper = bitTOint(pom.substr(0, 3));
	odp = bitTOint(pom.substr(3, 3));
	dlu = bitTOint(pom.substr(6, 32));
	id = bitTOint(pom.substr(552, 8));
	message = bitTOint(pom.substr(38, 512));
	uzupelnienie = bitTOint(pom.substr(550, 2));
	//id = bitTOint(pom.substr(552, 8));
}
// ================================================================
void client::read()
{
	string result;
	string byte1;
	string pom = message.to_string();

	cout << pom;

	for (int i = 0; i < pom.size(); i = i + 8)
	{
		byte1 = pom.substr(i, 8);
		result = result + bitTOchar(byte1);
	}

	cout << result;
}
// =================================================================
void client::clean()
{
	oper = 0;
	odp = 0;
	dlu = 0;
	message = 0;
	uzupelnienie = 0;
	id = 0;
}
// =================================================================
int client::bitTOint(const string &a)
{
	// ZAMIANA NA BITÓW NA INTY

	int l = 0;

	for (int i = a.size() - 1, p = 1; i >= 0; i--, p *= 2)
	{
		if (a[i] == '1')
		{
			l = l + p;
		}
	}

	return l;
}
// ==================================================================
void client::receive_message(SOCKET client_socket, char buffer[70], SOCKADDR_IN dest_addr, int sin_size)
{
	int test = 0;
	while (true)
	{
		recvfrom(client_socket, buffer,128, 0, (sockaddr*)&dest_addr, &sin_size);


		// UNPACKING PACKAGE

		string pom1;
		bitset<8> temporary;

		for (int i = 0; i < 70; i++)
		{
			pom1 = pom1 + charto2(buffer[i]);
		}


		int size = 0;
		string pom2 = "";
		char sign;

		oper = bitTOint(pom1.substr(0, 3));
		odp = bitTOint(pom1.substr(3, 3));
		dlu = size = bitTOint(pom1.substr(6, 32));
		id = bitTOint(pom1.substr(38, 8));
		message = bitTOint(pom1.substr(46, 512));
		uzupelnienie = bitTOint(pom1.substr(558, 2));
		//id = bitTOint(pom1.substr(552, 8));

		if (oper == 2 && odp == 0)
		{
			for (int i = 46; i < 46 + size; i++)
			{
				pom2 = pom2 + pom1[i];
			}

			cout << "\n Rozmowca napisal: \n";

			for (int j = 0; j < size; j = j + 8)
			{
				sign = from2to10(pom2.substr(j, 8));
				cout << sign;
			}
			cout << endl;
		}
		this_thread::sleep_for(chrono::milliseconds(500));

	}

}
// ===================================================================
void client::connection()
{

	cout << "Say hi! Napisz\"hej\"\n";
	int test = 0;
	while (true)
	{
		demand_sending();
		sendto(client_socket, buffer, buffer_size/8, 0, (sockaddr*)&dest_addr, sin_size);

		if (oper == 2 && odp == 0)
		{
			// ROZPAKOWANIE PAKIETU

			string pom1;
			bitset<8> temporary;

			for (int i = 0; i < 70; i++)
			{
				pom1 = pom1 + charto2(buffer[i]);
			}



			int size = 0;
			string pom2 = "";
			char sign;

			oper = bitTOint(pom1.substr(0, 3));
			odp = bitTOint(pom1.substr(3, 3));
			dlu = size = bitTOint(pom1.substr(6, 32));
			id = bitTOint(pom1.substr(38, 8));
			message = bitTOint(pom1.substr(46, 512));
			uzupelnienie = bitTOint(pom1.substr(558, 2));
			//id = bitTOint(pom1.substr(552, 8));




			clean();
			confirm_sending();
			pack();
			sendto(client_socket, buffer, 6, 0, (sockaddr*)&dest_addr, sin_size);

			if (oper == 2 && odp == 1) {
				std::cout << "Dostarczono\n";
			}

			if (oper == 2 && odp == 2) {
				std::cout << "22\n";
			}

			if (oper == 3 && odp == 0) {
			}

			if (oper == 3 && odp == 1) {
				std::cout << "Zezwolono na rozlaczenie\n";
				std::cout << "Trwa rozlaczanie\n";
				break;
			}
		}
		test++;

	}
	demand_disconnect();
	pack();
	sendto(client_socket, buffer, 6, 0, (sockaddr*)&dest_addr, sin_size);
}
// ======================================================================
void client::save_content(string txt)
{
	string binaryText = "";

	for (int i = 0; i < txt.size(); i++)
	{
		binaryText = binaryText + charto2(txt[i]);
	}

	message.reset();

	for (int i = binaryText.size() - 1; i >= 0; i--)
	{
		if (binaryText[i] == '1')
		{
			message.set(511 - i, 1);
		}
	}
}
// ====================================================================
void client::demand_connection()
{
	// ŻĄDANIE NAWIĄZANIA POŁĄCZENIA

	oper = 0;
	odp = 0;
	dlu = 0;
	message = 0;
	uzupelnienie = 0;
	id = identyfikator;
	pack();
}
// ==================================================================
void client::demand_id()
{
	// ŻĄDANIE PRZYDZIELENIA IDENTYFIKATORA

	oper = 000;
	odp = 100;
	dlu = 0;
	message = 0;
	uzupelnienie = 0;
	id = identyfikator;
	pack();
}
// =================================================================
void client::demand_invitation()
{
	// WYSŁANIE ZAPROSZENIA

	oper = 1;
	odp = 0;
	dlu = 0;
	message = 0;
	uzupelnienie = 0;
	id = identyfikator;
	pack();
}
// ==================================================================
void client::agree_inv()
{
	// PRZYJĘCIE ZAPROSZENIA

	oper = 1;
	odp = 1;
	dlu = 0;
	message = 0;
	uzupelnienie = 0;
	id = identyfikator;
	pack();
}
// ==================================================================
void client::disagree_inv()
{
	// ODRZUCENIE ZAPROSZENIA

	oper = 1;
	odp = 2;
	dlu = 0;
	message = 0;
	uzupelnienie = 0;
	id = identyfikator;
	pack();
}
// =================================================================
void client::demand_sending()
{
	// WYSŁANIE WIADOMOŚCI

	getline(cin, text);

	if (text == "#")
	{
		// clean();
		demand_disconnect();
		// pack();
	}
	else
	{
		save_content(text);
		oper = 2;
		odp = 0;
		dlu = text.size() * 8;		// ROZMIAR
		buffer_size = 3 + 3 + 32 + 8 + 2 + (text.size() * 8);
	



		uzupelnienie = 0;			// UZUPELNIENIE
		id = identyfikator;
		//std::cout << "<WYSYLAM> DZIALANIE: " << dzialanie << " ODPOWIEDZ:" << odpowiedz << " DLUGOSC:" << dlugosc << " TEKST:" << wiadomosc;
		pack();
		sendto(client_socket, buffer, buffer_size / 8, 0, (sockaddr*)&dest_addr, sin_size);
	}
}
// ==============================================================
void client::confirm_sending()
{
	// POTWIERDZENIE OTRZYMANIA WIADOMOŚCI

	oper = 2;
	odp = 1;
	dlu = 0;
	message = 0;
	uzupelnienie = 0;
	id = identyfikator;
	pack();
}
// ==============================================================
void client::demand_disconnect()
{
	// ZADANIE ZAKOŃCZENIA SESJI

	oper = 3;
	odp = 0;
	dlu = 0;
	message = 0;
	uzupelnienie = 0;
	id = identyfikator;
	pack();
}
// =============================================================
