#include <iostream>
#include <string>
#include <regex>
#include <iterator>
#include <sstream>

#include <stdlib.h>
#include <unistd.h>

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
	bool selection = true;
	int i = 1;
	while(i)
	{
		system("clear");
		cout << "IP Parser tool started \nType the number in front of the text to select an option.\n\n[1 Generate IP List] [2 Parse IP List] [3 Preview IP List] [4 Generate output file] [5 Update Firewall] [0 Exit]\n\nEnter selection: ";
		cin >> i;
		switch(i)
		{
			case 0:
				cout << "\nExiting the program\n" << endl;
				return 0;
			break;
			case 1:
				system("clear");
				cout << "Select the method of extracting IP's from the network \n\n[1 Generate IP's using IFTOP] [2 Generate IP's using TCPDUMP] [0 Cancel Generation]\n\nEnter selection: ";
				while(selection)
				{
					cin >> i;
					if(i >= 0 && i <= 2)
					{
						i = generateList(i);
						if(i == 0)
						{
							cout << "Successfully generated IP list\n" << endl;
							sleep(2);
							i = 1;
						}
						else
							cout << "Failed to obtain the IP list\n" << endl;
						selection = false;
					}
					else cout << "Enter selection: ";
				}	
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
			cout << "\nCanceling the generation\n" << endl;
			return 0;
		break;
		case 1:	
			cout << "\nIFTOP Generation selected." << endl;
			oss << "iftop -f \"dst port "<<PORT<<"\" -PNtnb -s "<<TIMEOUT<<" -L "<<LINES<<" > log.txt";
			bashCommand = oss.str();
			command = bashCommand.c_str(); 	
			system(command);
			return 0;
		break;
		case 2:
			cout << "\nTCPDUMP Generation selected." << endl;
			oss << "tcpdump -f \"dst port "<<PORT<<"\" -vttttq > log.txt & \nid=$!; sleep 5; kill $id ";
			bashCommand = oss.str();
			command = bashCommand.c_str(); 	
			system(command);
			return 0;
		break;
	}
	return 1;
}

int parseList(int type)
{
	cout << "Test";
	return 0;
}

int generateOutput()
{
	cout << "Test";
	return 0;
}

int updateFirewall()
{
	cout << "Test";
	return 0;
}