#include <iostream>
#include <string>
#include <regex>
#include <iterator>
#include <sstream>
#include <stdlib.h>

#define PORT 443
#define LINES 50
#define TIMEOUT 20

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
	ostringstream oss;
	string bashCommand;
	const char *command;
	switch(type)
	{
		case 0:
			cout << "Canceling the generation" << endl;
			return 0;
		break;
		case 1:	
			cout << "IFTOP Generation selected." << endl;
			
			oss << "iftop -f \"dst port "<<PORT<<"\" -PNtnb -s "<<TIMEOUT<<" -L "<<LINES<<" > log.txt";
			bashCommand = oss.str();
			command = bashCommand.c_str(); 	
			system(command);
			return 0;
		break;
		case 2:
			cout << "TCPDUMP Generation selected." << endl;
			oss << "tcpdump -f \"dst port "<<PORT<<"\" -vttttq > log.txt";
			bashCommand = oss.str();
			command = bashCommand.c_str(); 	
			system(command);
			return 0;
		break;
	}
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