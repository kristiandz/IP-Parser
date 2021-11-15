#include <iostream>
#include <string>
#include <iterator>
#include <sstream>
#include <fstream>
#include <regex>

#include <mariadb/conncpp.hpp>
#include <stdlib.h>
#include <unistd.h>

#define PORT 443
#define LINES 50
#define TIMEOUT 20

using namespace std;

int generateList(int);
int parseList(int);
int previewList();
int checkList();
int updateFirewall();

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
				system("clear");
				cout << "Select which log format to parse the IP's from \n\n[1 Parse IFTOP log] [2 Parse TCPDUMP log] [0 Cancel parsing]\n\nEnter selection: ";
				while(selection)
				{
					cin >> i;
					if(i >= 0 && i <= 2)
					{
						i = parseList(i);
						if(i > 0)
						{
							cout << "Successfully parsed IP list, found " << i << ((i>1) ? " matches.":" match." )<< "\n" << endl;
							i = 1;
							sleep(3);
						}
						else if(i == 0)
						{
							cout << "No matches in the log file\n" << endl;
							sleep(2);
							i = 1;
							selection = false;
						}
						selection = false;
					}
					else cout << "Enter selection: ";
				}
				selection = true;
				break;
			case 3:
				previewList();
				cout<<"\n\033[0;33mPress ENTER to continue...\033[0;37m";
				cin.ignore().get();
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
	ifstream file("log.txt"); //Hardcoding file name since the program outputs the same
	ofstream parsed_file("parsed_list.txt");
	string str;
	int counter = 0;
	if(file) 
	{
    	ostringstream ss;
    	ss << file.rdbuf();
    	str = ss.str();
	}
	switch(type)
	{
		case 0:
			cout << "Canceled IP parsing, returning to main menu" << endl;
			sleep(1);
			break;
		case 1:
		{
			regex iftop_regex("(?:[0-9]{1,3}.){3}[0-9]{1,3}(?=.[0-9]{1,5}[ ]{1,40}<=)"); // Regex for iftop with no hostname resolution!
			sregex_token_iterator end;
			sregex_token_iterator iter(str.begin(), str.end(), iftop_regex, 0);
			for( ; iter != end; ++iter )
			{
				cout << "Match: " << *iter << endl;
				parsed_file << *iter << endl;
				counter++;
			}
			file.close(); 
			parsed_file.close();
			return counter;
			break;
		}
		case 2:
		{
			regex tcpdump_regex("(?:[0-9]{1,3}.){3}[0-9]{1,3}(?=.[0-9]{1,5} >)"); // Regex for tcpdump with no hostname resolution!
			sregex_token_iterator end;
			sregex_token_iterator iter(str.begin(), str.end(), tcpdump_regex, 0);
			for( ; iter != end; ++iter )
			{
				cout << "Match: " << *iter << endl;
				parsed_file << *iter << endl;
				counter++;
			}
			file.close(); 
			parsed_file.close();
			return counter;
			break;
		}
	}
	file.close(); 
	parsed_file.close();
	return -1;
}

int previewList()
{
	ifstream parsed_file("parsed_list.txt");
	string str; 
	if(parsed_file) 
	{
    	ostringstream ss;
    	ss << parsed_file.rdbuf();
    	str = ss.str();
	}
	cout << "\nStored values: \n" << str << endl;
	parsed_file.close();
	return 0;
}

int checkList() // Check ipaliases as well
{
	ifstream parsed_list("parsed_list.txt");
	ofstream checked_list("checked_list.txt");
	vector<string> ip; string line;
	if(parsed_list)
		while (getline(parsed_list, line))
			ip.push_back(line);	
	try 
	{
        sql::Driver* driver = sql::mariadb::get_driver_instance();
        sql::SQLString url("jdbc:mariadb://localhost:3306/database-name-here"); // Fill in the data
        sql::Properties properties({{"user", "enter-your-username-here"}, {"password", "enter-your-password-here"}}); // Fill in the data
        unique_ptr<sql::Connection> conn(driver->connect(url, properties));
		try 
		{
			unique_ptr<sql::Statement> stmnt(conn->createStatement());
			for(auto i = ip.begin(); i < ip.end(); i++)
			{
				sql::ResultSet *res = stmnt->executeQuery("SELECT id FROM clients WHERE clients.ip = \"" + *i + "\"" ); // Specific query for my use, the program will be updated for a more generic use overall
				if(!res->next())
					if(checked_list)
						checked_list << *i << endl;
			}
			cout << "";
		}
		catch(sql::SQLException& e)
		{
			cerr << "Error while running the query: " << e.what() << endl;
		}
		conn->close();
	}
    catch(sql::SQLException& error)
	{
        cerr << "\nError Connecting to MariaDB Platform: " << error.what() << endl;
        return 1;
    }
	parsed_list.close();
	checked_list.close();
	sleep(2);
	return 0;
}

int updateFirewall()
{
	ostringstream oss;
	string bashCommand;
	const char *command;
	string line;
	ifstream checked_list("checked_list.txt");
	ofstream blacklist("/etc/csf/blacklists/parser_blacklist.txt", ofstream::app);
	if(checked_list && blacklist)
	{
		while (getline(checked_list, line))
			blacklist << line << " # add geoip info here..." << endl;
	}
	cout << "Updated the csf deny list" << endl;
	oss << "csf -r";
	bashCommand = oss.str();
	command = bashCommand.c_str();
	system(command);
	sleep(1);
	cout << "\nRestarted CSF firewall to apply the blacklist" << endl;
	sleep(3);
	return 0;
}