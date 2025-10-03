#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<iomanip>

using namespace std;
const string UsersFileName = "Users.txt";
const string ClientsFileName = "Clients.txt";
enum enMainMenueOptions
{
	eListClients = 1, eAddNewClient = 2, eDeleteClient = 3,
	eUpdateClient = 4, eFindClient = 5, eShowTransactionsMenue = 6,
	eManageUsers = 7, eExit = 8
};
enum enTransactionsMenueOptions { eDeposit = 1, eWithdraw = 2, eShowTotalBalance = 3, eShowMainMenue = 4 };
enum enMainMenuePermissions
{
	eAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4,
	pUpdateClients = 8, pFindClient = 16, pTranactions = 32, pManageUsers = 64
};
enum enManageUsersMenueOptions {
	eListUsers = 1, eAddNewUser = 2, eDeleteUser = 3,
	eUpdateUser = 4, eFindUser = 5, eMainMenue = 6
};

void ShowMainMenue();
void ShowTransactionsMenue();
void ShowManageUsersMenue();
bool CheckAccessPermission(enMainMenuePermissions Permission);
void Login();

struct  stUser
{
	string UserName;
	string Password;
	int Permissions;
	bool MarkForDelete = false;
};
struct sCleint
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance;
	bool MarkForDelete = false;
};

stUser CurrentCleint;
string ReadClientAccountNumber()
{
	string AccountNumber = "";
	cout << "\nPlease enter AccountNumber? ";
	cin >> AccountNumber;
	return AccountNumber;
}
short ReadMainMenueOption()
{
	cout << "Choose what do you want to do? [1 to 8]? ";
	short Choice = 0;
	cin >> Choice;


	return Choice;
}
short ReadTransactionsMenueOption()
{
	cout << "Choose what do you want to do? [1 to 4]? ";
	short Choice = 0;
	cin >> Choice;

	return Choice;
}
short ReadManageUsersMenueOption()
{
	cout << "Choose what do you want to do? [1 to 4]? ";
	short Choice = 0;
	cin >> Choice;

	return Choice;
}
int  ReadPermissionsToSet()
{
	int Permissions = 0;
	char Answer = 'n';
	cout << "\nDo you want to give full access? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		return -1;
	}

	cout << "\nDo you want to give access to : \n ";
	
	cout << "\nShow Client List? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		Permissions += enMainMenuePermissions::pListClients;
	}

	cout << "\nAdd New Client? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		Permissions += enMainMenuePermissions::pAddNewClient;
	}

	cout << "\nDelete Client? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		Permissions += enMainMenuePermissions::pDeleteClient;
	}

	cout << "\nFind Client? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		Permissions += enMainMenuePermissions::pFindClient;
	}

	cout << "\nTransactions Client? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		Permissions += enMainMenuePermissions::pTranactions;
	}

	cout << "\nManage Client? y/n? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		Permissions += enMainMenuePermissions::pManageUsers;
	}
	return Permissions;
}
string ReadUserName()
{
	string Username = "";

	cout << "\nPlease enter Username? ";
	cin >> Username;
	return Username;
}
vector < string> SplitString(string S1, string Delim)
{
	vector < string>vString;
	string sWord;
	short Pos = 0;
	while ((Pos = S1.find(Delim)) != std::string::npos)
	{
		sWord = S1.substr(0, Pos);
		if (sWord != "")
		{
			vString.push_back(sWord);
		}
		S1.erase(0, Pos + Delim.length());
	}

	if (S1 != "")
	{
		vString.push_back(S1);
	}
	return vString;
}
stUser  ConvertUserLineToRecord(string Line, string Seperator = "#//#")
{
	vector < string > vString = SplitString(Line, Seperator);

	stUser  User;
	User.UserName = vString[0];
	User.Password = vString[1];
	User.Permissions = stoi(vString[2]);

	return User;
}
vector < stUser> LoadUserDataFromFile(string FileName)
{
	vector < stUser> vUserData;
	fstream MyFile;

	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string Line;
		stUser  User;
		while (getline(MyFile, Line))
		{
			User = ConvertUserLineToRecord(Line);
			vUserData.push_back(User);
		}
		MyFile.close();
	}
	return vUserData;
}
sCleint  ConvertClintLineToRecord(string Line, string Seperator = "#//#")
{
	vector <string> vString = SplitString(Line, Seperator);
	sCleint  Client;
	if (vString.size() < 5)
	{
		cout << "Error" << endl;
		return Client;
	}

	Client.AccountNumber = vString[0];
	Client.PinCode = vString[1];
	Client.Name = vString[2];
	Client.Phone = vString[3];
	Client.AccountBalance = stod(vString[4]);

	return Client;
}
string ConvertRecordToLine(sCleint  Client, string Seperator = "#//#")
{
	string stClientRecord = "";
	stClientRecord += Client.AccountNumber + Seperator;
	stClientRecord += Client.PinCode + Seperator;
	stClientRecord += Client.Name + Seperator;
	stClientRecord += Client.Phone + Seperator;
	stClientRecord += to_string(Client.AccountBalance);

	return stClientRecord;

}
string  ConvertUserRecordToLine(stUser User, string Seperator = "#//#")
{
	string stClientRecord = "";
	stClientRecord += User.UserName + Seperator;
	stClientRecord += User.Password + Seperator;
	stClientRecord += to_string(User.Permissions);
	return stClientRecord;
}
void  ShowAccessDeniedMessage()
{
	cout << "\n------------------------------------\n";
	cout << "Access Denied, \nYou dont Have Permission To Do this,\nPlease Conact Your Admin.";
	cout << "\n------------------------------------\n";

}
void GoBackToManageUsersMenue()
{
	cout << "\n\nPress any key to go back to Transactions Menue...";
	system("pause>0");
	ShowManageUsersMenue();
}
vector <sCleint>  LoadCleintsDataFromFile(string FileName)
{
	vector <sCleint> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string Line;
		sCleint  Client;
		while (getline(MyFile, Line))
		{
			if (Line == "")
			{
				continue;
			}
			Client = ConvertClintLineToRecord(Line);
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return vClients;
}
vector <stUser> LoadUsersDataFromFile(string FileName)
{
	vector <stUser> vUser;
	fstream   MyFile;

	MyFile.open(FileName, ios::in);
	if(MyFile.is_open())
	{
		stUser  User;
		string Line;

		while (getline(MyFile, Line))
		{
			if (Line == "")
			{
				continue;
			}
			User = ConvertUserLineToRecord(Line);
			vUser.push_back(User);
		}
		MyFile.close();
	}
	return vUser;
}

void PrintClientRecordLine(sCleint  Client)
{
	cout << "| " << left << setw(15) << Client.AccountNumber ;
	cout << "| " << left << setw(10) << Client.PinCode;
	cout << "| " << left << setw(40) << Client.Name;
	cout << "| " << left << setw(12) << Client.Phone;
	cout << "| " << left << setw(12) << Client.AccountBalance;

}
void PrintClientCard(sCleint  Client)
{
	cout << "\nThe following are the client details:\n";
	cout << "-----------------------------------";
	cout << "\nAccout Number: " << Client.AccountNumber;
	cout << "\nPin Code     : " << Client.PinCode;
	cout << "\nName         : " << Client.Name;
	cout << "\nPhone        : " << Client.Phone;
	cout << "\nAccount Balance: " << Client.AccountBalance;
	cout << "\n-----------------------------------\n";
}
void PrintUserCard(stUser  User)
{
	cout << "\nThe following are the user details:\n";
	cout << "-----------------------------------";
	cout << "\nUsername    : " << User.UserName;
	cout << "\nPassword    : " << User.Password;
	cout << "\nPermissions : " << User.Permissions;
	cout << "\n-----------------------------------\n";
}
void PrintUserRecordLine(stUser  User)
{
	cout << "| " << left << setw(15) << User.UserName;
	cout << "| " << left << setw(10) << User.Password;
	cout << "| " << left << setw(40) << User.Permissions;
}
sCleint  ChangeClientRecord(string AccountNumber)
{
	sCleint Client;
	Client.AccountNumber = AccountNumber;

	cout << "\n\nEnter PinCode? ";
	getline(cin >> ws, Client.PinCode);

	cout << "Enter Name? ";
	getline(cin, Client.Name);

	cout << "Enter Phone? ";
	getline(cin, Client.Phone);

	cout << "Enter AccountBalance? ";
	cin >> Client.AccountBalance;

	return Client;

}
stUser ChangeUserRecord(string UserName)
{
	stUser  User;
	User.UserName = UserName;
	cout << "\n\nEnter Password? ";
	getline(cin >> ws, User.Password);

	User.Permissions = ReadPermissionsToSet();

	return User;
}
bool CheckAccessPermission(enMainMenuePermissions  Permission)
{
	if (CurrentCleint.Permissions == enMainMenuePermissions::eAll)
		return true;

	if ((Permission & CurrentCleint.Permissions) == Permission)
		return true;
	else
		return false;
}
void  GoBackToMainMenue()
{
	cout << "\n\nPress any key to go back to Main Menue...";
	system("pause>0");
	ShowMainMenue();
}
void  GoBackToTransactionsMenue()
{
	cout << "\n\nPress any key to go back to Main Menue...";
	system("pause>0");
	ShowTransactionsMenue();

}
void  AddDataLineToFile(string FileName, string stDataLine)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);
	if (MyFile.is_open())
	{
		MyFile << stDataLine << endl;
		MyFile.close();
	}
}
bool FindClientByAccountNumber(string AccountNumber, vector <sCleint> vClients, sCleint& Client)
{
	for (sCleint C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}
bool FindUserByUsername(string UserName, vector <stUser> vUsers, stUser& User)
{
	for (stUser U : vUsers)
	{
		if (U.UserName == UserName)
		{
			User = U;
			return true;
		}
	}
	return false;
}
bool  ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
	vector <sCleint> vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);

	if (MyFile.is_open())
	{

		string Line;
		sCleint Client;

		while (getline(MyFile, Line))
		{
			Client = ConvertClintLineToRecord(Line);
			if (Client.AccountNumber == AccountNumber)
			{
				MyFile.close();
				return true;
			}
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return false;
}
bool UserExistsByUsername(string UserName, string FileName)
{
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open())
	{
		string Line;
		stUser User;

		while (getline(MyFile, Line))
		{
			User = ConvertUserLineToRecord(Line);
			if (User.UserName == UserName)
			{
				MyFile.close();
				return true;
			}
		}
		MyFile.close();
	}
	return false;
}
vector <sCleint> SaveCleintsDataToFile(string FileName, vector <sCleint> vClients)
{
	string DataLine;
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	if (MyFile.is_open())
	{
		for (sCleint C : vClients)
		{
			if (C.MarkForDelete == false)
			{
				DataLine = ConvertRecordToLine(C);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vClients;
}
vector <stUser> SaveUsersDataToFile(string FileName, vector <stUser> vUsers)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	string DataLine;

	if (MyFile.is_open())
	{
		for (stUser U : vUsers)
		{
			if (U.MarkForDelete == false)
			{
				DataLine = ConvertUserRecordToLine(U);
				MyFile << DataLine << endl;
			}
		}
		MyFile.close();
	}
	return vUsers;
}
sCleint ReadNewClient()
{
	sCleint  Client;

	cout << "enter Account number?";
	getline(cin >> ws, Client.AccountNumber);
	while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
	{
		cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
		getline(cin >> ws, Client.AccountNumber);
	}
	cout << "Enter PinCode? ";
	getline(cin, Client.PinCode);

	cout << "Enter Name? ";
	getline(cin, Client.Name);

	cout << "Enter Phone? ";
	getline(cin, Client.Phone);

	cout << "Enter AccountBalance? ";
	cin >> Client.AccountBalance;

	return Client;
}
stUser  ReadNewUser()
{
	stUser  User;
	cout << "Enter Username? ";
	getline(cin >> ws, User.UserName);
	while (UserExistsByUsername (User.UserName, UsersFileName))
	{
		cout << "\nUser with [" << User.UserName << "] already exists, Enter another Username? ";
		getline(cin >> ws, User.UserName);
	}
	cout << "Enter Password? ";
	getline(cin, User.Password);

	User.Permissions = ReadPermissionsToSet();
	return User;
}
void AddNewUser()
{
	stUser User;
	User = ReadNewUser();
	AddDataLineToFile(UsersFileName, ConvertUserRecordToLine(User));

}
void AddNewUsers()
{
	char AddMore = 'Y';
	do
	{
		cout << "Adding New User:\n\n";
		AddNewUser();
		cout << "\nUser Added Successfully, do you want to add more Users? Y/N? ";
		cin >> AddMore;
	} while (toupper(AddMore) == 'Y');
}
void AddNewClient()
{
	sCleint  Client;
	Client = ReadNewClient();
	AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}
void AddNewClients()
{
	char AddMore = 'Y';
	do
	{
		cout << "Adding New Client:\n\n";
		AddNewClient();
		cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
		cin >> AddMore;


	} while (toupper(AddMore) == 'Y');
}
void ShowAddNewClientsScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pAddNewClient))
	{
		ShowAccessDeniedMessage();
		return;
	}
	cout << "\n-----------------------------------\n";
	cout << "\tAdd New Clients Screen";
	cout << "\n-----------------------------------\n";

	AddNewClients();

}
void ShowAddNewUserScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pAddNewClient))
	{
		ShowAccessDeniedMessage();
		return;
	}
	cout << "\n-----------------------------------\n";
	cout << "\tAdd New User Screen";
	cout << "\n-----------------------------------\n";

	AddNewUsers();
}
void ShowAllClientsScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pListClients))
	{
		ShowAccessDeniedMessage();
		return;
	}
	vector <sCleint> vClient = LoadCleintsDataFromFile(ClientsFileName);
	cout << "\n\t\t\t\t\tClient List (" << vClient.size() << ") Client(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	if (vClient.size() == 0)
	{
		cout << "\t\t\t\tNo Clients Available In the System!";
	}
	else
		for (sCleint Client : vClient)
		{
			PrintClientRecordLine(Client);
			cout << endl;
		}
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
}

bool  MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sCleint>& vClients)
{
	for (sCleint& C : vClients)
	{
		if (C.AccountNumber == AccountNumber)
		{
			C.MarkForDelete = true;
			return true;
		}
	}
	return false;
}
bool  MarkUserForDeleteByUsername(string UserName, vector <stUser>& vUsers)
{
	for (stUser& U : vUsers)
	{
		if (U.UserName == UserName)
		{
			U.MarkForDelete = true;
			return true;
		}
	}
	return false;
}
bool DeleteClientByAccountNumber(string AccountNumber, vector <sCleint>& vClient)
{
	sCleint Client;
	char Answer = 'n';

	if (FindClientByAccountNumber(AccountNumber, vClient, Client))
	{
		PrintClientCard(Client);
		cout << "\n\nAre you sure you want delete this client? y/n ? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			MarkClientForDeleteByAccountNumber(AccountNumber, vClient);
			SaveCleintsDataToFile(ClientsFileName, vClient);
			vClient = LoadCleintsDataFromFile(ClientsFileName);

			cout << "\n\nClient Deleted Successfully.";
			return true;
		}
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}
}
bool DeletUserByUserName(string UserName, vector <stUser> &vUsers)
{
	if (UserName == "Admin")
	{
		cout << "\n\nYou cannot Delete This User.";
		return false;
	}
	stUser User;
	char Answer = 'y';
	if (FindUserByUsername(UserName, vUsers, User))
	{
		PrintUserCard(User);

		cout << "\n\nAre you sure you want delete this User? y/n ? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			MarkUserForDeleteByUsername(UserName, vUsers);
			SaveUsersDataToFile(UsersFileName, vUsers);

			vUsers = LoadUsersDataFromFile(UsersFileName);
			cout << "\n\nUser Deleted Successfully.";
			return true;
		}
	}
	else
	{
		cout << "\nUser with Username (" << UserName << ") is Not Found!";
		return false;
	}
}
void ShowDeleteClientsScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pDeleteClient))
	{
		ShowAccessDeniedMessage();
		return;
	}
	
	cout << "\n-----------------------------------\n";
	cout << "\tDelete Client Screen";
	cout << "\n-----------------------------------\n";

	vector <sCleint> vClient;
	vClient = LoadCleintsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	DeleteClientByAccountNumber(AccountNumber, vClient);
	
}
void ShowDeleteUserScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tDelete Users Screen";
	cout << "\n-----------------------------------\n";
	vector<stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
	string  UserName = ReadUserName();
	DeletUserByUserName(UserName, vUsers);
}

bool UpdateClientByAccountNumber(string AccountNumber, vector <sCleint>& vClients)
{
	char Answer = 'N';
	sCleint  Client;
	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);
		cout << "\n\nAre you sure you want update this client? y/n ? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			for (sCleint& C : vClients)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}
			SaveCleintsDataToFile(ClientsFileName, vClients);
			cout << "\n\nClient Updated Successfully.";
			return true;
		}
	}
	else
	{
		cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
		return false;
	}
}
bool UpdateUserByUsername(string UserName, vector <stUser>& vUsers)
{
	stUser  User;
	char Answer = 'n';

	if (FindUserByUsername(UserName, vUsers, User))
	{
		PrintUserCard(User);
		cout << "\n\nAre you sure you want update this User? y/n ? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			for (stUser& U : vUsers)
			{
				if (U.UserName == UserName)
				{
					U = ChangeUserRecord(UserName);
					break;
				}
			}
			SaveUsersDataToFile(UsersFileName, vUsers);
			cout << "\n\nUser Updated Successfully.";
			return true;
		}
	}
	else
	{
		cout << "\nUser with Account Number (" << UserName << ") is Not Found!";
		return false;
	}
}
void  ShowUpDateClientsScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pUpdateClients))
	{
		ShowAccessDeniedMessage();
		return;
	}
	cout << "\n-----------------------------------\n";
	cout << "\tUpdate Client Info Screen";
	cout << "\n-----------------------------------\n";
	vector <sCleint> vClients = LoadCleintsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	UpdateClientByAccountNumber(AccountNumber, vClients);
}
void  ShowUpDateUserScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tUpdate Users Screen";
	cout << "\n-----------------------------------\n";

	vector <stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
	string Username = ReadUserName();
	UpdateUserByUsername(Username, vUsers);

}
void  ShowFindClientsScreen()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pFindClient))
	{
		ShowAccessDeniedMessage();
		return;
	}
	cout << "\n-----------------------------------\n";
	cout << "\tFind Client Screen";
	cout << "\n-----------------------------------\n";

	vector <sCleint>vClients = LoadCleintsDataFromFile(ClientsFileName);
	sCleint  Client;
	string AccountNumber = ReadClientAccountNumber();

	if (FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		PrintClientCard(Client);
	}
	else
		cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";
}
void  ShowFindUserScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tFind User Screen";
	cout << "\n-----------------------------------\n";

	vector <stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
	stUser User;
	string Username = ReadUserName();

	if (FindUserByUsername(Username, vUsers, User))
		PrintUserCard(User);
	else
		cout << "\nUser with Username [" << Username << "] is not found!";
}
bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <sCleint> &vClients)
{
	char Answer = 'n';
	cout << "\n\nAre you sure you want perfrom this transaction? y/n ? ";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		for (sCleint& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;
				SaveCleintsDataToFile(ClientsFileName, vClients);
				cout << "\n\nDone Successfully. New balance is: " << C.AccountBalance;

				return true;
			}
		}
		return false;
	}
}
void  ShowDepositScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tDeposit Screen";
	cout << "\n-----------------------------------\n";
	sCleint Client;
	vector <sCleint> vClients = LoadCleintsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();

	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
		AccountNumber = ReadClientAccountNumber();
	}
	PrintClientCard(Client);
	double Amount = 0;
	cout << "\nPlease enter deposit amount? ";
	cin >> Amount;
	DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);
}
void  ShowWithDrawScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tWithdraw Screen";
	cout << "\n-----------------------------------\n";

	sCleint Client;
	vector <sCleint> vClients = LoadCleintsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();

	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
		AccountNumber = ReadClientAccountNumber();
	}
	PrintClientCard(Client);

	double Amount = 0;
	cout << "\nPlease enter withdraw amount? ";
	cin >> Amount;
	while (Amount > Client.AccountBalance)
	{
		cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
		cout << "Please enter another amount? ";
		cin >> Amount;
	}
	DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);
}
void PrintClientRecordBalanceLine(sCleint  Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}

void ShowTotalBalances()
{
	vector <sCleint> vClients = LoadCleintsDataFromFile(ClientsFileName);
	cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	double TotalBalances = 0;

	if (vClients.size() == 0)
	{
		cout << "\t\t\t\tNo Clients Available In the System!";
	}
	else
		for (sCleint Client : vClients)
		{
			PrintClientRecordBalanceLine(Client);
			TotalBalances += Client.AccountBalance;

			cout << endl;
		}
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "\t\t\t\t\t   Total Balances = " << TotalBalances;
}
void ShowTotalBalancesScreen()
{
	ShowTotalBalances();
}

void PerfromTranactionsMenueOption(enTransactionsMenueOptions    TransactionMenueOption)
{
	switch (TransactionMenueOption)
	{
	case eDeposit:
	{
		system("cls");
		ShowDepositScreen();
		GoBackToTransactionsMenue();
		break;
	}
	case eWithdraw:
	{
		system("cls");
		ShowWithDrawScreen();
		GoBackToTransactionsMenue();
		break;
	}
	case eShowTotalBalance:
	{
		system("cls");
		ShowTotalBalancesScreen();
		GoBackToTransactionsMenue();
		break;
	}
	case eShowMainMenue:
	{
		ShowMainMenue();
	}
	}
}
void ShowAllUsersScreen()
{
	vector <stUser> vUsers = LoadUsersDataFromFile(UsersFileName);
	cout << "\n\t\t\t\t\tUsers List (" << vUsers.size() << ") User(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	cout << "| " << left << setw(15) << "User Name";
	cout << "| " << left << setw(10) << "Password";
	cout << "| " << left << setw(40) << "Permissions";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	if (vUsers.size() == 0)
		cout << "\t\t\t\tNo Users Available In the System!";
	else
		for (stUser User : vUsers)
		{

			PrintUserRecordLine(User);
			cout << endl;
		}

	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;


}
void ShowListUsersScreen()
{
	ShowAllUsersScreen();
}
void PerfromManageUsersMenueOption(enManageUsersMenueOptions  ManageUsersMenueOption)
{
	switch (ManageUsersMenueOption)
	{
	case eListUsers:
	{
		system("cls");
		ShowListUsersScreen();
		GoBackToManageUsersMenue();
		break;
	}
	case eAddNewUser:
	{
		system("cls");
		ShowAddNewUserScreen();
		GoBackToManageUsersMenue();
		break;
	}	
	case eDeleteUser:
	{
		system("cls");
		ShowDeleteUserScreen();
		GoBackToManageUsersMenue();
		break;
	}	
	case eUpdateUser:
	{
		system("cls");
		ShowUpDateUserScreen();
		GoBackToManageUsersMenue();
		break;
	}	
	case eFindUser:
	{
		system("cls");
		ShowFindUserScreen();
		GoBackToManageUsersMenue();
		break;
	}	
	case eMainMenue:
	{
		ShowMainMenue();

	}
	}
}
void ShowManageUsersMenue()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pManageUsers))
	{
		ShowAccessDeniedMessage();
		GoBackToMainMenue();
		return;
	}
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tManage Users Menue Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] List Users.\n";
	cout << "\t[2] Add New User.\n";
	cout << "\t[3] Delete User.\n";
	cout << "\t[4] Update User.\n";
	cout << "\t[5] Find User.\n";
	cout << "\t[6] Main Menue.\n";
	cout << "===========================================\n";

	PerfromManageUsersMenueOption((enManageUsersMenueOptions)ReadManageUsersMenueOption());
}
void ShowTransactionsMenue()
{
	if (!CheckAccessPermission(enMainMenuePermissions::pTranactions))
	{
		ShowAccessDeniedMessage();
		GoBackToMainMenue();
		return;
	}
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tTransactions Menue Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] Withdraw.\n";
	cout << "\t[3] Total Balances.\n";
	cout << "\t[4] Main Menue.\n";
	cout << "===========================================\n";

	PerfromTranactionsMenueOption((enTransactionsMenueOptions)ReadTransactionsMenueOption());
}
void  PerfromMainMenueOption(enMainMenueOptions  MainMenueOption)
{
	switch (MainMenueOption)
	{
	case eListClients:
	{
		system("cls");
		ShowAllClientsScreen();
		GoBackToMainMenue();
		break;
	}
	case eAddNewClient:
	{
		system("cls");
		ShowAddNewClientsScreen();
		GoBackToMainMenue();
		break;
	}
	case eDeleteClient:
	{
		system("cls");
		ShowDeleteClientsScreen();
		GoBackToMainMenue();
		break;
	}
	case eUpdateClient:
	{
		system("cls");
		ShowUpDateClientsScreen();
		GoBackToMainMenue();
		break;
	}
	case eFindClient:
	{
		system("cls");
		ShowFindClientsScreen();
		GoBackToMainMenue();
		break;
	}
	case eShowTransactionsMenue:
	{
		system("cls");
		ShowTransactionsMenue();
		break;
	}
	case eManageUsers:
	{
		system("cls");
		ShowManageUsersMenue();
		break;
	}
	case eExit:
	{
		system("cls");
		Login();
		break;
	}

	}
}
void ShowMainMenue()
{
	system("cls");
	cout << "===========================================\n";
	cout << "\t\tMain Menue Screen\n";
	cout << "===========================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Transactions.\n";
	cout << "\t[7] Manage Users.\n";
	cout << "\t[8] Logout.\n";
	cout << "===========================================\n";

	PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}
bool FindUserByUserNameAndPassword(string UserName, string Password, stUser& User)
{
	vector < stUser> vUsers = LoadUserDataFromFile(UsersFileName);
	for (stUser U : vUsers)
	{
		if (U.UserName == UserName && U.Password == Password)
		{
			User = U;
			return true;
		}
	}
	return false;
}
bool LoadUserInfo(string UserName, string Password)
{
	if (FindUserByUserNameAndPassword(UserName, Password, CurrentCleint))
	{
		return true;
	}
	return false;
}
void Login()
{
	bool LoginFailed = false;
	string UserName, Password;

	do
	{
		system("cls");
		cout << "\n-------------------------------------------\n";
		cout << "\tLogin Screen";
		cout << "\n-------------------------------------------\n";
		if (LoginFailed)
		{
			cout << "Invalid Username/Password!\n";
		}
		cout << "\nEnter UserNam ?";
		cin >> UserName;

		cout << "\nEnter Password ?";
		cin >> Password;
		LoginFailed = !LoadUserInfo(UserName, Password);
	} while (LoginFailed);
	ShowMainMenue();
}
int main()
{
	Login();
	system("pause>0");
	return 0;
}

