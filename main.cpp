#include <iostream>
#include <string>
#include <regex>
#include <iterator>
#include <sstream>
#include <stdlib.h>

using namespace std;

int generateList(int);
int parseList(int);
int updateFirewall();
int generateOutput();


int main ()
{	
	int i = 1;
	while(i)
	{
		system("clear");
		cout << "IP Parser tool started \nType the number in front of the text to select an option.\n\n [1 Generate IP List] [2 Parse IP List] [3 Preview IP List] [4 Generate output file] [5 Update Firewall] [0 Exit]\nEnter selection: ";
		cin >> i;
		switch(i)
		{
			case 0:
				cout << "\nExiting the program" << endl;
				return 0;
			break;
			case 1:
			break;
			case 2:
			break;
			case 3:
			break;
			case 4:
			break;
			case 5:
			break;
			default:
				cout << "\n> Valid selection are numbers from 0-5, please try again" << endl;
			break;
		}
	}
	return 0;
}

int generateList(int type)
{
	cout << "Test";
}

int parseList(int type)
{
	cout << "Test";
}

int generateOutput()
{
	cout << "Test";
}

int updateFirewall()
{
	cout << "Test";
}