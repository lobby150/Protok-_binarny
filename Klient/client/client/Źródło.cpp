//______________________MAIN - SOURCE______________________________//
#include"pch.h"
#include "client.h"

int main()
{
	std::cout << "Choose option:[1]\n Work as client\n";

	int INPUT;

	do {
		INPUT = _getch();

		if (INPUT == '1') {
			std::cout << "Working as client\n";
			client c;
			c.UDP();
		}
	} while (INPUT != 1);

	
}