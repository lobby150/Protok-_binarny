//_________________________SERVER - CPP_______________________________________//
#include"pch.h"
#include "server.h"

int server::UDP() {

	//INICJALIZACJA WINSOCK'A
	WSAStartup(MAKEWORD(2, 2), &wsaData); // Inicjalizacja WinSock

	my_address.sin_family = AF_INET; //wybor rodziny adresow [IPv4]
	my_address.sin_port = htons(28027); // przypisanie numeru portu [28027]
	inet_pton(AF_INET, "127.0.0.1", &my_address.sin_addr); //przypisanie adresu IP [127.0.0.1]

	server_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); //tworzenie socketa
	bind(server_socket, (sockaddr*)&my_address, sizeof(my_address)); //przypisanie socketa na podany port

	sockaddr_in current_address;
	memset(&current_address, 0, sizeof(current_address)); // Wyzeruj reszte struktury	their_addr.sin_family = AF_INET; // host byte order
	inet_pton(AF_INET, "127.0.0.1", &current_address.sin_addr);
	current_address.sin_port = htons(28027); // short, network byte order
	//std::string info_to_client1;
	//int users_counter = 0;
	//std::string wiadomosc;
	//std::string pomocniczy;

	while (1) {

		recvfrom(server_socket, buffer, 128, 0, (struct sockaddr *) &current_address, &sin_size);
		unpack();

		if (oper == 0 && odp == 0) {
			std::cout << "Proba polaczenia przez klienta " << std::endl;
			if (users_counter == 0) {
				client1_address = current_address;
				users_counter++;
				clean();
				acceptConnection();
				pack();
				sendto(server_socket, buffer, 6, 0, (sockaddr*)&current_address, sin_size);
				std::cout << "Witamy 1 klienta " << std::endl;
			}
			else {
				if (users_counter == 1) {
					if (current_address.sin_port != client1_address.sin_port) {
						client2_address = current_address;
						users_counter++;
						clean();
						acceptConnection();
						pack();
						sendto(server_socket, buffer, 6, 0, (sockaddr*)&client2_address, sin_size);
						std::cout << "Witamy 2 klienta " << std::endl;
					}
				}
				else {
					users_counter++;
					clean();
					denyConnection();
					sendto(server_socket, buffer, 6, 0, (sockaddr*)&current_address, sin_size);
					std::cout << "3 klienta zegnamy" << std::endl;
				}
			}
		}

		if (oper == 0 && odp == 1) {
		}

		if (oper == 0 && odp == 2) {
		}

		if (oper == 0 && odp == 3) {
		}

		if (oper == 0 && odp == 4) {
			std::cout << "Klient chce ID " << std::endl;
			clean();
			giveID();
			pack();
			sendto(server_socket, buffer, 7, 0, (sockaddr*)&current_address, sin_size);
			std::cout << "Dano ID " << std::endl;
		}

		if (oper == 0 && odp == 5) {
		}

		if (oper == 1 && odp == 0) {
			std::cout << "Klient zaprasza klienta do rozmowy" << std::endl;
			if (users_counter > 1) {
				std::cout << "2 klient zaproszony" << std::endl;
				if (current_address.sin_port == client1_address.sin_port) {
					unpack();
					std::cout << "Klient 1 -> serwer " << std::endl;
					read();
					int rozmiar = 0;
					std::string roz = length.to_string();
					rozmiar = from2to10(roz.substr(0, 8)) * 1000 + from2to10(roz.substr(8, 8)) * 100 + from2to10(roz.substr(16, 8)) * 10 + from2to10(roz.substr(24, 8)) * 1;
					buffer_size = 3 + 3 + 32 + 8 + 2 + rozmiar;
					sendto(server_socket, buffer, buffer_size / 8, 0, (sockaddr*)&client2_address, sin_size);
					std::cout << "Serwer -> klient2 " << std::endl;
					read();
				}
				else {
					unpack();
					std::cout << "Klient 2 -> serwer  " << std::endl;
					read();
					int rozmiar = 0;
					std::string roz = length.to_string();
					rozmiar = from2to10(roz.substr(0, 8)) * 1000 + from2to10(roz.substr(8, 8)) * 100 + from2to10(roz.substr(16, 8)) * 10 + from2to10(roz.substr(24, 8)) * 1;
					buffer_size = 3 + 3 + 32 + 8 + 2 + rozmiar;
					sendto(server_socket, buffer, buffer_size / 8, 0, (sockaddr*)&client1_address, sin_size);
					std::cout << "Serwer -> klient1 " << std::endl;
					read();
				}
			}
			else {
				std::cout << "Za malo klientow" << std::endl;
				clean();
				noClient();
				pack();
				sendto(server_socket, buffer, 6, 0, (sockaddr*)&current_address, sin_size);
			}
		}

		if (oper == 1 && odp == 1) {
			if (current_address.sin_port == client1_address.sin_port) {
				unpack();
				std::cout << "Klient1 -> serwer  ";
				read();
				int rozmiar = rozmiar_buffera();
				sendto(server_socket, buffer, buffer_size/8, 0, (sockaddr*)&client2_address, sin_size);
				std::cout << "serwer -> klient2  ";
				read();
			}
			else {
				unpack();
				std::cout << "Klient 2 -> serwer ";
				read();
				int rozmiar = rozmiar_buffera();
				sendto(server_socket, buffer, buffer_size/8, 0, (sockaddr*)&client1_address, sin_size);
				std::cout << "serwer -> Klient 1  ";
				read();
			}
		}

		if (oper == 1 && odp == 2) {
			if (current_address.sin_port == client1_address.sin_port) {
				unpack();
				std::cout << "Klient1 -> serwer ";
				read();
				int rozmiar = rozmiar_buffera();
				sendto(server_socket, buffer, buffer_size/8, 0, (sockaddr*)&client2_address, sin_size);
				std::cout << "Serwer -> klient 2";
				read();
			}
			else {
				unpack();
				std::cout << "Klient 2 -> serwer ";
				read();
				int rozmiar = rozmiar_buffera();
				sendto(server_socket, buffer, buffer_size/8, 0, (sockaddr*)&client1_address, sin_size);
				std::cout << "Serwer -> klient1 ";
				read();
			}
		}

		if (oper == 1 && odp == 3) {
			std::cout << "13\n";
		}

		if (oper == 2 && odp == 0) {


			if (current_address.sin_port == client1_address.sin_port) {
				unpack();
				std::cout << "Klient 1 -> serwer ";
				read();
				int rozmiar = rozmiar_buffera();
				sendto(server_socket, buffer, buffer_size/8, 0, (sockaddr*)&client2_address, sin_size);
				std::cout << "Serwer -> klient2 ";
				read();
			}
			else {
				unpack();
				std::cout << "Klient 2 -> serwer ";
				read();
				int rozmiar = rozmiar_buffera();
				sendto(server_socket, buffer, buffer_size/8, 0, (sockaddr*)&client1_address, sin_size);
				std::cout << "Serwer -> klient 1 ";
				read();
			}
		}

		if (oper == 2 && odp == 1) {
			if (current_address.sin_port == client1_address.sin_port) {
				//unpack();
				std::cout << "Klient 1 -> serwer ";
				read();
				pack();
				int rozmiar = rozmiar_buffera();
				sendto(server_socket, buffer, buffer_size/8, 0, (sockaddr*)&client2_address, sin_size);
				std::cout << "Serwer -> klient2 ";
				read();
			}
			else {
				//unpack();
				std::cout << "Klient2 -> serwer ";
				read();
				pack();
				int rozmiar = rozmiar_buffera();
				sendto(server_socket, buffer, buffer_size/8, 0, (sockaddr*)&client1_address, sin_size);
				std::cout << "Serwer -> klient1 ";
				read();
			}
		}

		if (oper == 2 && odp == 2) {
			std::cout << "22\n";
		}

		if (oper == 3 && odp == 0) {
			std::cout << "Zakonczenie sesji, prosze" << std::endl;
			clean();
			confirmEndingConnection();
			pack();
			sendto(server_socket, buffer, 6, 0, (sockaddr*)&current_address, sin_size);
			std::cout << "Pozwalam na zakonczenie sesji" << std::endl;
		}

		if (oper == 3 && odp == 1) {
		}

	}



	closesocket(server_socket);
	WSACleanup();

	return 1;
}

void server::pack() {

	std::string pomocnicza;

	pomocnicza = oper.to_string();
	pomocnicza += odp.to_string();
	pomocnicza += length.to_string();
	pomocnicza += id.to_string();
	pomocnicza += message.to_string();
	pomocnicza += uzupelnienie.to_string();
	//pomocnicza += id.to_string();
	for (int i = 0; i < 70; i++) {
		std::string bajt_1 = pomocnicza.substr(i * 8, 8);
		buffer[i] = bit_to_int(bajt_1);
		//std::cout << "PAKUJE: "<<buffer[i];
	}
}

void server::unpack() {

	std::string pomocnicza;
	std::bitset<8> chwilowy;
	for (int i = 0; i < 70; i++) {
		pomocnicza += fromCharto2(buffer[i]);
	}

	oper = bit_to_int(pomocnicza.substr(0, 3));
	odp = bit_to_int(pomocnicza.substr(3, 3));
	length = bit_to_int(pomocnicza.substr(6, 32));
	id = bit_to_int(pomocnicza.substr(38, 8));
	message = bit_to_int(pomocnicza.substr(46, 512));
	uzupelnienie = bit_to_int(pomocnicza.substr(558, 2));
	//id = bit_to_int(pomocnicza.substr(552, 8));
}

void server::read() {

	std::string pomocnicza = "";
	for (int i = 0; i < 70; i++) {
		pomocnicza += fromCharto2(buffer[i]);
	}
	std::string pom = "";
	int rozmiar = bit_to_int(pomocnicza.substr(6, 32));

	for (int i = 38; i < 38 + rozmiar; i++) {
		pom = pom + pomocnicza[i];
	}

	std::cout << oper << " " << odp << " " << length << " " << pom << " " << uzupelnienie << " " << id << std::endl;
}

void server::clean() {
	oper = 0;
	odp = 0;
	length = 0;
	message = 0;
	uzupelnienie = 0;
	id = identyfikator;
}

int server::bit_to_int(const std::string &s) {
	int liczba = 0;
	for (int i = s.size() - 1, p = 1; i >= 0; i--, p *= 2)
	{
		if (s[i] == '1')
			liczba += p;
	}
	return liczba;
}

void server::saveContent(std::string napis) {
	std::string binarnytekst = "";

	for (int i = 0; i < napis.size(); i++) {
		binarnytekst = binarnytekst + fromCharto2(napis[i]);
	}
	message.reset();
	for (int i = binarnytekst.size() - 1; i >= 0; i--) {
		if (binarnytekst[i] == '1') {
			message.set(511 - i, 1);
		}
	}
}

void server::acceptConnection() {
	//_____________WYSLANIE INFORMACJI O PRZYJECIU POLACZENIA______________//
	oper = 0;
	odp = 1;
	length = 0;
	message = 0;
	uzupelnienie = 0;
	id = identyfikator;
	pack();
	//sendto(server_socket, buffer, 6, 0, (sockaddr*)&current_address, sin_size);
}

void server::denyConnection() {
	//____________WYSLANIE INFORMACJI O ODMOWIE POLACZENIA________________//
	oper = 000;
	odp = 010;
	length = 0;
	message = 0;
	uzupelnienie = 0;
	id = identyfikator;
	pack();
	//sendto(server_socket, buffer, 6, 0, (sockaddr*)&current_address, sin_size);
}

void server::giveID() {
	//______________WYSLANIE POTWIERDZENIA O PRZYDZIELENIU ID___________//
	oper = 000;
	odp = 101;
	length = 0;
	message = 0;
	uzupelnienie = 0;
	id = identyfikator;
	pack();
	//sendto(server_socket, buffer, 7, 0, (sockaddr*)&current_address, sin_size);
}

void server::denyID() {
	//______________WYSLANIE ZADANIA POLACZENIA_______________________//
	oper = 000;
	odp = 110;
	length = 0;
	message = 0;
	uzupelnienie = 0;
	id = identyfikator;
	pack();
	sendto(server_socket, buffer, 6, 0, (sockaddr*)&current_address, sin_size);
}

void server::noClient() {
	//________________WYSLANIE ZADANIA POLACZENIA____________________//
	oper = 1;
	odp = 3;
	length = 0;
	message = 0;
	uzupelnienie = 0;
	id = identyfikator;
	pack();
	sendto(server_socket, buffer, 6, 0, (sockaddr*)&current_address, sin_size);
}

void server::denySending() {
	//_______________WYSLANIE ZADANIA POLACZENIA___________________//
	oper = 2;
	odp = 2;
	length = 0;
	message = 0;
	uzupelnienie = 0;
	id = identyfikator;
	pack();
	sendto(server_socket, buffer, 6, 0, (sockaddr*)&current_address, sin_size);
}

void server::confirmEndingConnection() {
	//________________WYSLANIE ZADANIA POLACZENIA_________________//
	oper = 3;
	odp = 1;
	length = 0;
	message = 0;
	uzupelnienie = 0;
	id = identyfikator;
	pack();
	//sendto(server_socket, buffer, 6, 0, (sockaddr*)&current_address, sin_size);
}
int server::rozmiar_buffera()
{
	for (int i = 0; i < 128; i++)
	{
		if (buffer[i] == 0) {
			return i + 1;
		}
	}
}