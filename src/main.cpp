#include <iostream>
#include <string>
#include <iterator>
#include <sstream>
#include <fstream>
#include <regex>

#include <mariadb/conncpp.hpp>
#include <stdlib.h>
#include <unistd.h>

#define PORT 28950
#define LINES 20000
#define TIMEOUT 45

using namespace std;

int generateList(int);
int parseList(int);
int previewList();
int checkList();
int updateFirewall();

int main()
{	
	bool selection = true;
	int i = 1;
	while(i)
	{
		system("clear");
		cout << "\033[4;32mIP PARSER | FIREWALL UPDATER FOR ANTIDDOS\n\n\033[0;37mType the number in front of the text to select an option.\n\n\033[0;32m[1 Generate IP List] [2 Parse IP List] [3 Preview IP List] [4 Database and Geolocation check] [5 Update Firewall] [0 Exit]\n\n\033[0;37mEnter selection: ";
		cin >> i; // Restrict chars,return to loop
		switch(i)
		{
			case 0:
				cout << "\n\033[1;32mExiting the program\n\033[0;37m" << endl;
				return 0;
				break;
			case 1:
				system("clear");
				cout << "Select the method of extracting IP's from the network \n\n\033[0;32m[1 Generate IP's using IFTOP] [2 Generate IP's using TCPDUMP] [0 Cancel generation]\n\n\033[0;37mEnter selection: ";
				while(selection)
				{
					cin >> i;
					if(i >= 0 && i <= 2)
					{
						i = generateList(i);
						if(i == 0)
						{
							cout << "\033[1;32mSuccessfully generated IP list\n\033[0;37m" << endl;
							sleep(3);
							i = 1;
						}
						else if(i == -1)
						{
							sleep(1);
							cout << "Returning to main menu..." << endl;
						}
						else
							cout << "\033[0;33mFailed to obtain the IP list\n\033[0;37m" << endl;
						selection = false;
					}
					else cout << "Enter selection: ";
				}
				selection = true;	
				break;
			case 2:
				system("clear");
				cout << "Select which log format to parse the IP's from \n\n\033[0;32m[1 Parse IFTOP log] [2 Parse TCPDUMP log] [0 Cancel parsing]\n\n\033[0;37mEnter selection: ";
				while(selection)
				{
					cin >> i;
					if(i >= 0 && i <= 2)
					{
						i = parseList(i);
						if(i > 0)
						{
							cout << "\033[1;32mSuccessfully parsed IP list, found " << i << ((i>1) ? " matches.":" match." )<< "\n\033[0;37m" << endl;
							i = 1;
							sleep(3);
						}
						else if(i == 0)
						{
							cout << "\033[0;33mNo matches in the log file\n\033[0;37m" << endl;
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
				checkList();
				break;
			case 5:
				updateFirewall();
				break;
			default:
				cout << "\n\033[0;33m> Valid selection are numbers from 0-5, please try again\033[0;37m" <<endl;
				sleep(1);
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
			cout << "\033[0;33mCanceling the generation\n\033[0;37m";
			return -1;
		break;
		case 1:	
			cout << "\n\033[1;32mIFTOP Generation selected.\033[0;37m" << endl;
			oss << "iftop -f \"dst port "<<PORT<<"\" -PNtnb -s "<<TIMEOUT<<" -L "<<LINES<<" > ./log.txt";
			bashCommand = oss.str();
			command = bashCommand.c_str(); 	
			system(command);
			return 0;
		break;
		case 2:
			cout << "\n\033[1;32mTCPDUMP Generation selected.\033[0;37m" << endl;
			oss << "tcpdump -f \"dst port "<<PORT<<"\" -vnttttq > ./log.txt & \nid=$!; sleep 10; kill $id ";
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
	string str; int counter = 0;
	if(file && parsed_file) 
	{
    	ostringstream ss;
    	ss << file.rdbuf();
    	str = ss.str();
	}
	switch(type)
	{
		case 0:
			cout << "\033[0;33mCanceled IP parsing, returning to main menu\033[0;37m" << endl;
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
	cout << "\n\033[1;32mStored values: \033[0;37m\n" << str << endl;
	parsed_file.close();
	return 0;
}

int checkList()
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
			cout << "\033[0;32mFinished the IP check, you can find the malicious IP's in the checked_list file.\033[0;37m" << endl;
		}
		catch(sql::SQLException& e)
		{
			cerr << "Error while running the query: " << e.what() << endl;
		}
		conn->close();
	}
    catch(sql::SQLException& error)
	{
        cerr << "\nError Connecting to MariaDB Platform: \033[0;33m" << error.what() << "\033[0;37m" << endl;
        return 1;
    }
	parsed_list.close();
	checked_list.close();
	sleep(3);
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
	cout << "\033[0;32mUpdated the csf deny list\033[0;37m" << endl;
	oss << "csf -r";
	bashCommand = oss.str();
	command = bashCommand.c_str();
	system(command);
	sleep(1);
	cout << "\n\033[0;32mRestarted CSF firewall to apply the blacklist\033[0;37m" << endl;
	sleep(3);
	return 0;
}

/* 
WARNING: While testing this please don't lock yourself out, be careful not to add your IP to the blacklist before running this. 
Preview the block list if you are not sure what are you doing. Depending on the IP generation, you may pull your IP as well, 
and if the new ip is not in the mysql database, it will be included, or for testing purposes if you are trying different ports
or manually adding ip combinations.
*/

// TODO: Make a separate function for bash commands, mysql as well