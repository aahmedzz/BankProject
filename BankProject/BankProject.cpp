#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

const string ClientsFile = "clients.txt";
const string UsersFile = "users.txt";
struct sUser {
    string Username;
    string Password;
    short Permissions;
};
sUser CurrentUser;
struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance = 0;
};

void ShowMainMenu();
void ShowLoginScreen();
void ShowTransacionPage();
void ManageUsersMenuPage();
enum enTransactionsMenueOptions {
    eDeposit = 1, eWithdraw = 2,
    eShowTotalBalance = 3, eShowMainMenue = 4
};
enum enMainMenuOptions {
    eListClients = 1, eAddNewClient = 2,
    eDeleteClient = 3, eUpdateClient = 4, eFindClient = 5,
    eShowTransactionsMenue = 6,enManageUsers = 7 ,
    enLogout = 8 , eExit = 9
};
enum enUsersMenuOptions {
    enListUsers = 1, enAddUser =2, enDeleteuser = 3,
    enUpdateUser = 4 ,enFindUser =5, enMainMenu = 6,
};
enum enUserPermissions {
    eShowClients = 1, eAddClients = 2, eDeleteClients = 4,
    eUpdateClients = 8, eFindClients = 16 ,eTransactions = 32,
    eManageUsers = 64, eFullAccess = 127,
};
bool CheckAccessPermission(enUserPermissions Permission)
{
    if (CurrentUser.Permissions == enUserPermissions::eFullAccess)
        return true;
    if ((Permission & CurrentUser.Permissions) == Permission)
        return true;
    else
        return false;
}
void ShowAccessDeniedMessage()
{
    system("cls");
    cout << "\n------------------------------------\n";
    cout << "Access Denied, \nYou dont Have Permission To Do this, \nPlease Conact Your Admin.";
    cout << "\n------------------------------------\n";
    cout << "\nPress Enter to go back to Transactions Menu... ";
    cin.ignore();
    cin.get();
    ShowMainMenu();
}

//Deal With Clients file Functions
vector<string> SplitString(string line, string separator) {
    string part;
    int pos;
    vector<string> Parts;
    while ((pos = line.find(separator)) != string::npos)
    {
        part = line.substr(0, pos);
        if (part != "") {
            Parts.push_back(part);
        }
        line.erase(0,pos + separator.length());
    }
    if (line != "") {
        Parts.push_back(line);
    }
    return Parts;
}
sClient ConvertLineToClient(string line) {
    sClient client;
    vector<string> clData = SplitString(line,"#//#");
    client.AccountNumber = clData[0];
    client.PinCode = clData[1];
    client.Name = clData[2];
    client.Phone = clData[3];
    client.AccountBalance = stod(clData[4]);
    return client;
}
vector<sClient> LoadClientsFromFile(string fileName) {
    fstream MyFile;
    vector<sClient> clients;
    string line;
    MyFile.open(fileName, ios::in);
    if (MyFile.is_open()) {
        while (getline(MyFile,line)) {
            clients.push_back(ConvertLineToClient(line));
        }
        MyFile.close();
    }
    return clients;
}
string ConvertClientToLine(sClient client, string separator) {
    string clientStr = "";
    clientStr += client.AccountNumber + separator;
    clientStr += client.PinCode + separator;
    clientStr += client.Name + separator;
    clientStr += client.Phone + separator;
    clientStr += to_string(client.AccountBalance);

    return clientStr;
}
void AppendClientToFile(string FileName,sClient client) {
    fstream MyFile;
    MyFile.open(FileName ,ios::out | ios::app);
    if (MyFile.is_open()) {
        MyFile << ConvertClientToLine(client,"#//#") << "\n";
        cout << "\nClient saved successfully.\n";
        MyFile.close();
    }
}
bool IsClientExist(string accountNumber) {
    vector<sClient> clients = LoadClientsFromFile(ClientsFile);
    for (sClient &client : clients)
    {
        if (client.AccountNumber == accountNumber)
        {
            return true;
        }
    }
    return false;
}
bool IsClientExist(string accountNumber,sClient &sclient) {
    vector<sClient> clients = LoadClientsFromFile(ClientsFile);
    for (sClient& client : clients)
    {
        if (client.AccountNumber == accountNumber)
        {
            sclient = client;
            return true;
        }
    }
    return false;
}
string ReadClientAccountNumber()
{
    string AccountNumber = "";
    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;

    return AccountNumber;
}
void SaveClientsToFile(string FileName, vector<sClient> clients) {
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    if (MyFile.is_open()) {
        for (sClient& client : clients) {
            MyFile << ConvertClientToLine(client, "#//#") << "\n";
        }
        MyFile.close();
    }
}
void PrintClientRecord(sClient Client)
{
    cout << "\n\nThe following is the extracted client record:\n";
    cout << "\nAccout Number: " << Client.AccountNumber;
    cout << "\nPin Code : " << Client.PinCode;
    cout << "\nName : " << Client.Name;
    cout << "\nPhone : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
}

//Show Clients
void PrintClientRow(sClient Client) {
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(15) << left << Client.PinCode;
    cout << "| " << setw(30) << left << Client.Name;
    cout << "| " << setw(20) << left << Client.Phone;
    cout << "| " << setw(20) << left << Client.AccountBalance;
    cout << "|" << endl;
}
void PrintAllClients(vector<sClient> clients) {
    cout << "\n\t\t\t\t\t\tClient List (" << clients.size() << ") Client(s).";
    cout << "\n---------------------------------------------------------------------------------------------------------------" << endl;
    cout << "| " << setw(15) << left << "AccountNumber";
    cout << "| " << setw(15) << left << "PinCode";
    cout << "| " << setw(30) << left << "Name";
    cout << "| " << setw(20) << left << "Phone";
    cout << "| " << setw(20) << left << "AccountBalance";
    cout << "|" << endl;
    cout << "---------------------------------------------------------------------------------------------------------------\n";
    for (sClient& client : clients) {
        PrintClientRow(client);
    }
    cout << "---------------------------------------------------------------------------------------------------------------\n";
}
void ShowClientsList() {
    if(!CheckAccessPermission(enUserPermissions::eShowClients))
    {
        ShowAccessDeniedMessage();
    }
    system("cls");
    vector<sClient> clients = LoadClientsFromFile(ClientsFile);
    PrintAllClients(clients);
    cout << "\nPress Enter to return to main menu... ";
    cin.ignore();
    cin.get();
    ShowMainMenu();
}

//Add new client
sClient ReadNewClient()
{
    sClient Client;
    string accountNumber = ReadClientAccountNumber();
    while (IsClientExist(accountNumber)) {
        cout << "Client with Account Number ( " << accountNumber << " ) already exists.\n";
        accountNumber = ReadClientAccountNumber();
    }
    Client.AccountNumber = accountNumber;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
void AddNewClientPage() {
    if (!CheckAccessPermission(enUserPermissions::eAddClients))
    {
        ShowAccessDeniedMessage();
    }
    system("cls");
    cout << "==================================================\n";
    cout << "\t\tAdd New Client Screen\n";
    cout << "==================================================\n";
    char addMore;
    do {
        sClient client = ReadNewClient();
        AppendClientToFile(ClientsFile, client);
        cout << "Do you want to add more clients? ";
        cin >> addMore;}
    while (tolower(addMore) == 'y');

    cout << "\nPress Enter to return to main menu... ";
    cin.ignore();
    cin.get();
    ShowMainMenu();
}

//Delete client
bool ReadDeleteBool() {
    char delConfirm;
    cout << "Are you sure you want to delete this client? y/n?";
    cin >> delConfirm;
    if (tolower(delConfirm) == 'y')
        return true;
    else
        return false;
}
void DeleteClientFromFile(string accountNumber) {
    vector<sClient> AllClients = LoadClientsFromFile(ClientsFile);
    vector<sClient> ClientsToSave;
    for (sClient& client : AllClients) {
        if (client.AccountNumber != accountNumber) {
            ClientsToSave.push_back(client);
        }
    }
    SaveClientsToFile(ClientsFile,ClientsToSave);
    cout << "\nDeleted successfully...\n";
}
void DeleteClientPage() {
    if (!CheckAccessPermission(enUserPermissions::eDeleteClients))
    {
        ShowAccessDeniedMessage();
    }
    system("cls");
    cout << "==================================================\n";
    cout << "\t\tDelete Client Screen\n";
    cout << "==================================================\n";

    sClient Client;
    string AccountNumber = ReadClientAccountNumber();
    if (IsClientExist(AccountNumber,Client))
    {
    	PrintClientRecord(Client);
    	cout << endl;
    	if (ReadDeleteBool()) {
    		DeleteClientFromFile(AccountNumber);
    	}
    	else
    	{
    		cout << "Deletion cancelled...\n";
    	}
    }
    else
    {
    	cout << "\nClient with Account Number (" << AccountNumber <<") is Not Found!\n";
    }
    cout << "\nPress Enter to return to main menu... ";
    cin.ignore();
    cin.get();
    ShowMainMenu();
}

//Update client
bool ReadUpdateBool() {
    char updConfirm;
    cout << "Are you sure you want to Update this client? y/n?";
    cin >> updConfirm;
    if (tolower(updConfirm) == 'y')
        return true;
    else
        return false;
}
sClient ChangeClientRecord(string accountNumber) {
    sClient Client;
    Client.AccountNumber = accountNumber;
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
void UpdateClientInFile(string accountNumber) {
    vector<sClient> AllClients = LoadClientsFromFile(ClientsFile);
    for (sClient& client : AllClients) {
        if (client.AccountNumber == accountNumber) {
            client = ChangeClientRecord(accountNumber);
        }
    }
    SaveClientsToFile(ClientsFile,AllClients);
    cout << "\nClient updated successfully...\n";
}
void UdateClientPage() {
    if (!CheckAccessPermission(enUserPermissions::eUpdateClients))
    {
        ShowAccessDeniedMessage();
    }
    system("cls");
    cout << "==================================================\n";
    cout << "\t\tUpdate Client Screen\n";
    cout << "==================================================\n";

    sClient Client;
    string AccountNumber = ReadClientAccountNumber();
    if (IsClientExist(AccountNumber, Client))
    {
        PrintClientRecord(Client);
        cout << endl;
        if (ReadUpdateBool()) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            UpdateClientInFile(AccountNumber);
        }
        else
        {
            cout << "Update cancelled...\n";
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!\n";
    }

    cout << "\nPress Enter to return to main menu... ";
    cin.ignore();
    cin.get();
    ShowMainMenu();
}

//Find Client
void FindClientPage() {
    if (!CheckAccessPermission(enUserPermissions::eFindClients))
    {
        ShowAccessDeniedMessage();
    }
    system("cls");
    cout << "==================================================\n";
    cout << "\t\tFind Client Screen\n";
    cout << "==================================================\n";
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();
    if (IsClientExist(AccountNumber, Client))
    {
        PrintClientRecord(Client);
        cout << endl;
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!\n";
    }

    cout << "\nPress Enter to return to main menu... ";
    cin.ignore();
    cin.get();
    ShowMainMenu();
}

//Deposit transaction
bool ReadTransactionBool() {
    char tranConfirm;
    cout << "Are you sure you want to preform this transaction? y/n?";
    cin >> tranConfirm;
    if (tolower(tranConfirm) == 'y')
        return true;
    else
        return false;
}
void UpdateClientBalanceInFile(string accountNumber,double accBalance) {
    vector<sClient> AllClients = LoadClientsFromFile(ClientsFile);
    for (sClient& client : AllClients) {
        if (client.AccountNumber == accountNumber) {
            client.AccountBalance = accBalance;
        }
    }
    SaveClientsToFile(ClientsFile, AllClients);
    cout << "\nBalance updated successfully...\n";
}
void DepositPage() {
    system("cls");
    cout << "==================================================\n";
    cout << "\t\tDeposit Screen\n";
    cout << "==================================================\n";
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();
    double depositAmount;
    if (IsClientExist(AccountNumber, Client))
    {
        PrintClientRecord(Client);
        cout << endl;
        cout << "Please enter deposit amount? ";
        cin >> depositAmount;
        if (ReadTransactionBool()) {
            Client.AccountBalance = Client.AccountBalance + depositAmount;
            UpdateClientBalanceInFile(Client.AccountNumber, Client.AccountBalance);
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!\n";
    }

    cout << "\nPress Enter to return to Transaction menu... ";
    cin.ignore();
    cin.get();
    ShowTransacionPage();
}

//withdraw transaction
void WithdrawPage() {
    system("cls");
    cout << "==================================================\n";
    cout << "\t\tWithdraw Screen\n";
    cout << "==================================================\n";
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();
    double withdrawAmount;
    if (IsClientExist(AccountNumber, Client))
    {
        PrintClientRecord(Client);
        cout << endl;
        cout << "Please enter withdraw amount? ";
        cin >> withdrawAmount;
        while (withdrawAmount > Client.AccountBalance) 
        {
            cout << "\nWithdraw amount exeeds account balance...\n";
            cout << "Please enter valid withdraw amount? ";
            cin >> withdrawAmount;
        }
        if (ReadTransactionBool()) {
            Client.AccountBalance = Client.AccountBalance - withdrawAmount;
            UpdateClientBalanceInFile(Client.AccountNumber, Client.AccountBalance);
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!\n";
    }

    cout << "\nPress Enter to return to Transaction menu... ";
    cin.ignore();
    cin.get();
    ShowTransacionPage();
}

//Total Balances
void PrintBalanceRow(sClient Client) {
    cout << "| " << setw(25) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(39) << left << Client.AccountBalance;
    cout << "|" << endl;
}
void TotalBalancespage() {
    system("cls");
    vector<sClient> clients = LoadClientsFromFile(ClientsFile);
    double TotalBalances = 0;
    cout << "\n\t\t\t\t\t\tClient List (" << clients.size() << ") Client(s).";
    cout << "\n---------------------------------------------------------------------------------------------------------------" << endl;
    cout << "| " << setw(25) << left << "AccountNumber";
    cout << "| " << setw(40) << left << "Client Name";
    cout << "| " << setw(39) << left << "AccountBalance";
    cout << "|" << endl;
    cout << "---------------------------------------------------------------------------------------------------------------\n";
    for (sClient& client : clients) {
        TotalBalances = TotalBalances + client.AccountBalance;
        PrintBalanceRow(client);
    }
    cout << "---------------------------------------------------------------------------------------------------------------\n";
    cout << "\n\t\t\t\t\t\tTotal Balances =" << TotalBalances << endl;

    cout << "\nPress Enter to return to Transaction menu... ";
    cin.ignore();
    cin.get();
    ShowTransacionPage();
}

//transaction page
void ShowTransacionPage() {
    if (!CheckAccessPermission(enUserPermissions::eTransactions))
    {
        ShowAccessDeniedMessage();
    }
    system("cls");
    short option;
    enTransactionsMenueOptions Tranoption;
    cout << "==================================================\n";
    cout << "\t\tTransaction Menu Screen\n";
    cout << "==================================================\n";
    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total balances.\n";
    cout << "\t[4] Main menu.\n";;
    cout << "==================================================" << endl;
    cout << "Choose what do you want to do? [1 to 4]? ";
    cin >> option;
    Tranoption = (enTransactionsMenueOptions)option;
    while (!(option >= 1 && option <= 4)) {
        cout << "Not Available Action...\n";
        cout << "Choose what do you want to do? [1 to 4]?";
        cin >> option;
    }
    switch (Tranoption)
    {
        case (enTransactionsMenueOptions::eDeposit):
            DepositPage();
            break;
        case (enTransactionsMenueOptions::eWithdraw):
            WithdrawPage();
            break;
        case (enTransactionsMenueOptions::eShowTotalBalance):
            TotalBalancespage();
            break;
        case (enTransactionsMenueOptions::eShowMainMenue):
            ShowMainMenu();
            break;
    }
}

//main menu
void ShowMainMenu() {
    system("cls");
    short option;
    enMainMenuOptions Menuoption;
    cout << "==================================================\n";
    cout << "\t\tMain Menu Screen\n";
    cout << "==================================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.\n";
    cout << "\t[9] Exit.\n";
    cout << "=================================================="<<endl;
    cout << "Choose what do you want to do? ["<<enMainMenuOptions::eListClients<<" to "<<enMainMenuOptions::eExit<<"]?";
    cin >> option;
    Menuoption = (enMainMenuOptions)option;
    while (!(option >= enMainMenuOptions::eListClients && option <= enMainMenuOptions::eExit)) {
        cout << "Not Available Action...\n";
        cout << "Choose what do you want to do? [" << enMainMenuOptions::eListClients << " to " << enMainMenuOptions::eExit << "]?";
        cin >> option;
        Menuoption = (enMainMenuOptions)option;
    }

    switch (Menuoption)
    {
        case (enMainMenuOptions::eListClients):
            ShowClientsList();
            break;
        case (enMainMenuOptions::eAddNewClient):
            AddNewClientPage();
            break;
        case (enMainMenuOptions::eDeleteClient):
            DeleteClientPage();
            break;
        case (enMainMenuOptions::eUpdateClient):
            UdateClientPage();
            break;
        case (enMainMenuOptions::eFindClient):
            FindClientPage();
            break;
        case (enMainMenuOptions::eShowTransactionsMenue):
            ShowTransacionPage();
            break;
        case (enMainMenuOptions::enManageUsers):
            ManageUsersMenuPage();
            break;
        case (enMainMenuOptions::enLogout):
            ShowLoginScreen();
            break;
        case (enMainMenuOptions::eExit):
            exit(0);
            break;
    }
}

//Deal With Users file Functions
sUser ConvertLineToUser(string line) {
    sUser user;
    vector<string> userData = SplitString(line, "#//#");
    user.Username = userData[0];
    user.Password = userData[1];
    user.Permissions = stoi(userData[2]);
    return user;
}
vector<sUser> LoadUsersFromFile(string fileName) {
    fstream MyFile;
    string line;
    vector<sUser> users;
    MyFile.open(fileName, ios::in);
    if (MyFile.is_open()) {
        while (getline(MyFile, line)) {
            if(line == "")
                continue;
            users.push_back(ConvertLineToUser(line));
        }
        MyFile.close();
    }
    return users;
}
bool IsUserExist(string username,sUser &founduser) {
    vector<sUser> users = LoadUsersFromFile(UsersFile);
    for (sUser& user : users) {
        if (user.Username == username) {
            founduser = user;
            return true;
        }
    }
    return false;
}
bool FindUserWithUsernameAndPassword(string username, string password, sUser& foundUser) {
    vector<sUser> users = LoadUsersFromFile(UsersFile);
    for (sUser& user : users) {
        if (user.Username == username && user.Password == password) {
            foundUser = user;
            return true;
        }
    }
    return false;
}
string ConvertUserToLine(sUser user, string separator) {
    string userStr = "";
    userStr += user.Username + separator;
    userStr += user.Password + separator;
    userStr += to_string(user.Permissions);

    return userStr;
}
void AppendUserToFile(string FileName, sUser user) {
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);
    if (MyFile.is_open()) {
        MyFile << ConvertUserToLine(user, "#//#") << "\n";
        cout << "\nUser saved successfully.\n";
        MyFile.close();
    }
}
string ReadUsername() {
    string username;
    cout << "Enter Username? ";
    cin >> username;
    return username;
}
string ReadPassword() {
    string password;
    cout << "Enter Password? ";
    cin >> password;
    return password;
}
short ReadPermissions() {
    int permissions=0;
    char permissionOpt;
    cout << "Do you want to give full access? y/n?";
    cin >> permissionOpt;
    if (tolower(permissionOpt) == 'y')
        permissions = enUserPermissions::eFullAccess;
    else
    {
        cout << "Do you want to give ful access to: \n";
        cout << "Show Clients List? y/n?";
        cin >> permissionOpt;
        if(tolower(permissionOpt) == 'y') permissions = (permissions | enUserPermissions::eShowClients);
        cout << "Add Clients ? y/n?";
        cin >> permissionOpt;
        if (tolower(permissionOpt) == 'y') permissions = (permissions | enUserPermissions::eAddClients);
        cout << "Delete Clients ? y/n?";
        cin >> permissionOpt;
        if (tolower(permissionOpt) == 'y') permissions = (permissions | enUserPermissions::eDeleteClients);
        cout << "Update Clients ? y/n?";
        cin >> permissionOpt;
        if (tolower(permissionOpt) == 'y') permissions = (permissions | enUserPermissions::eUpdateClients);
        cout << "Find Clients ? y/n?";
        cin >> permissionOpt;
        if (tolower(permissionOpt) == 'y') permissions = (permissions | enUserPermissions::eFindClients);
        cout << "Transactions ? y/n?";
        cin >> permissionOpt;
        if (tolower(permissionOpt) == 'y') permissions = (permissions | enUserPermissions::eTransactions);
        cout << "Manage Users ? y/n?";
        cin >> permissionOpt;
        if (tolower(permissionOpt) == 'y') permissions = (permissions | enUserPermissions::eManageUsers);
    }
    return permissions;
}
void PrintUserRecord(sUser user)
{
    cout << "\n\nThe following is the extracted user record:\n";
    cout << "\nUsername : " << user.Username;
    cout << "\nPassword : " << user.Password;
    cout << "\nPermissions: " << user.Permissions;
}
sUser ChangeUserRecord(string username) {
    sUser user;
    user.Username = username;
    user.Password = ReadPassword();
    user.Permissions = ReadPermissions();

    return user;
}
void SaveUsersToFile(string FileName, vector<sUser> users) {
    fstream MyFile;
    MyFile.open(FileName, ios::out);
    if (MyFile.is_open()) {
        for (sUser& user : users) {
            MyFile << ConvertUserToLine(user, "#//#") << "\n";
        }
        MyFile.close();
    }
}

//show users list
void PrintUserRow(sUser user) {
    cout << "| " << setw(15) << left << user.Username;
    cout << "| " << setw(15) << left << user.Password;
    cout << "| " << setw(30) << left << user.Permissions;
    cout << endl;
}
void PrintAllUsers(vector<sUser> users) {
    cout << "\n\t\t\t\t\t\tClient List (" << users.size() << ") Client(s).";
    cout << "\n---------------------------------------------------------------------------------------------------------------" << endl;
    cout << "| " << setw(15) << left << "User Name";
    cout << "| " << setw(15) << left << "Password";
    cout << "| " << setw(30) << left << "Permissions";
    cout << endl;
    cout << "---------------------------------------------------------------------------------------------------------------\n";
    for (sUser& user : users) {
        PrintUserRow(user);
    }
    cout << "---------------------------------------------------------------------------------------------------------------\n";
}
void ListUsersPage() {
    system("cls");
    vector<sUser> users = LoadUsersFromFile(UsersFile);
    PrintAllUsers(users);
    cout << "\nPress Enter to go back to Transactions Menu... ";
    cin.ignore();
    cin.get();
    ManageUsersMenuPage();
}

//Add users
sUser ReadNewUser()
{
    sUser user;
    user.Username = ReadUsername();
    while (IsUserExist(user.Username,user)) {
        cout << "User with Username ( " << user.Username << " ) already exists.\n";
        user.Username = ReadUsername();
    }
    user.Password = ReadPassword();
    user.Permissions = ReadPermissions();

    return user;
}
void AddNewUserPage() {
    system("cls");
    cout << "==================================================\n";
    cout << "\t\tAdd New User Screen\n";
    cout << "==================================================\n";
    char addMore;
    do {
        sUser user = ReadNewUser();
        AppendUserToFile(UsersFile, user);
        cout << "Do you want to add more Users? ";
        cin >> addMore;
    } while (tolower(addMore) == 'y');

    cout << "\nPress Enter to go back to Transactions Menu... ";
    cin.ignore();
    cin.get();
    ManageUsersMenuPage();
}

//Delete user
bool ReadUserDeleteBool() {
    char delConfirm;
    cout << "Are you sure you want to delete this user? y/n?";
    cin >> delConfirm;
    if (tolower(delConfirm) == 'y')
        return true;
    else
        return false;
}
void DeleteUserFromFile(string username) {
    vector<sUser> AllUsers = LoadUsersFromFile(UsersFile);
    vector<sUser> UsersToSave;
    for (sUser& user : AllUsers) {
        if (user.Username != username) {
            UsersToSave.push_back(user);
        }
    }
    SaveUsersToFile(UsersFile, UsersToSave);
    cout << "\nDeleted successfully...\n";
}
void DeleteUserPage() {
    if (!CheckAccessPermission(enUserPermissions::eDeleteClients))
    {
        ShowAccessDeniedMessage();
    }
    system("cls");
    cout << "==================================================\n";
    cout << "\t\tDelete User Screen\n";
    cout << "==================================================\n";

    sUser user;
    user.Username = ReadUsername();
    if (IsUserExist(user.Username,user))
    {
        PrintUserRecord(user);
        cout << endl;
        if (ReadUserDeleteBool()) {
            DeleteUserFromFile(user.Username);
        }
        else
        {
            cout << "Deletion cancelled...\n";
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << user.Username << ") is Not Found!\n";
    }

    cout << "\nPress Enter to go back to Transactions Menu... ";
    cin.ignore();
    cin.get();
    ManageUsersMenuPage();
}

//Update users
bool ReadUserUpdateBool() {
    char updConfirm;
    cout << "Are you sure you want to Update this User? y/n?";
    cin >> updConfirm;
    if (tolower(updConfirm) == 'y')
        return true;
    else
        return false;
}
void UpdateUserInFile(string username) {
    vector<sUser> AllUsers = LoadUsersFromFile(UsersFile);
    for (sUser &user: AllUsers) {
        if (user.Username == username) {
            user = ChangeUserRecord(username);
        }
    }
    SaveUsersToFile(UsersFile, AllUsers);
    cout << "\nClient updated successfully...\n";
}
void UpdateUserPage() {
    system("cls");
    cout << "==================================================\n";
    cout << "\t\tUpdate User Screen\n";
    cout << "==================================================\n";

    sUser user;
    user.Username = ReadUsername();
    if (IsUserExist(user.Username,user))
    {
        PrintUserRecord(user);
        cout << endl;
        if (ReadUpdateBool()) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            UpdateUserInFile(user.Username);
        }
        else
        {
            cout << "Update cancelled...\n";
        }
    }
    else
    {
        cout << "\nUser with Account Number (" << user.Username << ") is Not Found!\n";
    }

    cout << "\nPress Enter to go back to Transactions Menu... ";
    cin.ignore();
    cin.get();
    ManageUsersMenuPage();
}

//Find user
void FindUserPage() {
    system("cls");
    cout << "==================================================\n";
    cout << "\t\tFind User Screen\n";
    cout << "==================================================\n";
    sUser user;
    user.Username = ReadUsername();
    if (IsUserExist(user.Username, user))
    {
        PrintUserRecord(user);
        cout << endl;
    }
    else
    {
        cout << "\nClient with Account Number (" << user.Username << ") is Not Found!\n";
    }

    cout << "\nPress Enter to go back to Transactions Menu... ";
    cin.ignore();
    cin.get();
    ManageUsersMenuPage();
}

//Mange users screen
void ManageUsersMenuPage() {
    if (!CheckAccessPermission(enUserPermissions::eManageUsers))
    {
        ShowAccessDeniedMessage();
    }
    system("cls");
    short option;
    enUsersMenuOptions Menuoption;
    cout << "==================================================\n";
    cout << "\t\tManage Users Menu Screen\n";
    cout << "==================================================\n";
    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update User.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menu.\n";
    cout << "==================================================" << endl;
    cout << "Choose what do you want to do? [" << enUsersMenuOptions::enListUsers << " to " << enUsersMenuOptions::enMainMenu << "]?";
    cin >> option;
    Menuoption = (enUsersMenuOptions)option;
    while (!(option >= enUsersMenuOptions::enListUsers && option <= enUsersMenuOptions::enMainMenu)) {
        cout << "Not Available Action...\n";
        cout << "Choose what do you want to do? [" << enUsersMenuOptions::enListUsers << " to " << enUsersMenuOptions::enMainMenu << "]?";
        cin >> option;
        Menuoption = (enUsersMenuOptions)option;
    }

    switch (Menuoption)
    {
        case (enUsersMenuOptions::enListUsers):
            ListUsersPage();
            break;
        case(enUsersMenuOptions::enAddUser):
            AddNewUserPage();
            break;
        case(enUsersMenuOptions::enDeleteuser):
            DeleteUserPage();
            break;
        case(enUsersMenuOptions::enUpdateUser):
            UpdateUserPage();
            break;
        case(enUsersMenuOptions::enFindUser):
            FindUserPage();
            break;
        case (enUsersMenuOptions::enMainMenu):
            ShowMainMenu();
            break;
    }
}

//login screen
void ShowLoginScreen() {
    string username, password;
    bool firstAttempt = true;
    do {
        system("cls");
        cout << "==================================================\n";
        cout << "\t\tLogin Screen\n";
        cout << "==================================================\n";
        if (!firstAttempt)
            cout << "Invalid UserName/Password!";
        cout << "\nEnter Username? ";
        cin >> username;
        cout << "Enter Password? ";
        cin >> password;

        firstAttempt = false;
    } while (!FindUserWithUsernameAndPassword(username, password, CurrentUser));
    ShowMainMenu();
}

int main()
{
    ShowLoginScreen();
    system("pause");
}

