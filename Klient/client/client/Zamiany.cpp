//______________________ZAMIANY - CPP_______________________//
#include "pch.h"
#include "Zamiany.h"

using namespace std;

int from2to10(string from2to10) {

	int wynik = 0;

	for (int i = from2to10.size() - 1; i >= 0; i--) {
		wynik = wynik + (from2to10[i] - '0') * pow(2, from2to10.size() - 1 - i);
	}
	return wynik;
}

string from10to2(int from10to2)
{
	if (from10to2 == 0)
		return "00000000";

	string result = "";
	while (from10to2 > 0)
	{
		result = result.insert(0, to_string(from10to2 % 2));
		from10to2 = from10to2 / 2;
	}
	if (result.size() < 8)
	{
		for (int i = result.size(); i < 8; i++) {
			result.insert(0, "0");
		}
	}
	return result;
}

string charto2(char charto2_input)
{
	int pom = charto2_input;
	if (pom == '0') {
		return "00000000";
	}
	string result = "";

	if (pom >= 0) {
		while (pom > 0) {
			result = result.insert(0, to_string(pom % 2));
			pom = pom / 2;


		}
		if (result.size() < 8) {
			for (int i = result.size(); i < 8; i++) {
				result.insert(0, "0");
			}
		}
	}
	else {
		pom = 256 + pom;
		while (pom > 0) {
			result = result.insert(0, to_string(pom % 2));
			pom = pom / 2;
		}
		if (result.size() < 8) {
			for (int i = result.size(); i < 8; i++) {
				result.insert(0, "0");
			}
		}
	}

	return result;

}

int bitTOint(const  string &a)
{
	int l = 0;
	for (int i = a.size() - 1, p = 1; i >= 0; i--, p *= 2)
	{
		if (a[i] == '1')
			l += p;
	}
	return l;
}
char bitTOchar(string t)
{
	char pom;
	pom = bitTOint(t);
	return pom;
}

