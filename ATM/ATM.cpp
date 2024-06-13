#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

const string CLIENTS_FILE = "clients.txt";
void startSystem();

enum enATM {
    en_Quick_withdraw = 1,
    en_Normal_withdraw,
    en_Deposit,
    en_Check_Balance,
    en_Change_PINCode,
    en_Logout
};

enum enQuickWithdraw {
    en_amount_20 = 1,
    en_amount_50,
    en_amount_100,
    en_amount_200,
    en_amount_400,
    en_amount_600,
    en_amount_800,
    en_amount_1000,
    en_Cancel
};

void perform_ATM_Service(enATM);

struct stClient {
    string accountNumber;
    string PINCode;
    string name;
    string phone;
    double balance;
};

stClient CURRENT_CLIENT;

vector <string> splitString(string st, string delimiter = " ") {

    vector <string> vWords;
    string word = "";
    short pos{ 0 };

    while ((pos = st.find(delimiter)) != std::string::npos) {
        word = st.substr(0, pos);
        if (word != "") {
            vWords.push_back(word);
        }
        st.erase(0, pos + delimiter.length());
    }

    if (st != "")
        vWords.push_back(st);

    return vWords;
}

stClient convertLineToRecord(string line, string delimiter = "#//#") {

    vector <string> vClientData = splitString(line, delimiter);

    stClient client;
    client.accountNumber = vClientData.at(0);
    client.PINCode = vClientData.at(1);
    client.name = vClientData.at(2);
    client.phone = vClientData.at(3);
    client.balance = stod(vClientData.at(4));

    return client;
}

string convertRecordToLine(stClient clientInfo, string delimiter = "#//#") {
    string st = "";

    st = clientInfo.accountNumber + delimiter
        + clientInfo.PINCode + delimiter
        + clientInfo.name + delimiter
        + clientInfo.phone + delimiter
        + to_string(clientInfo.balance);

    return st;
}

vector <stClient> loadDataFromFile(string fileName) {

    vector <stClient> vClients;
    stClient client;

    fstream manageClients;
    manageClients.open(fileName, ios::in); // Read Mode

    if (manageClients.is_open()) {
        string line;

        while (getline(manageClients, line)) {
            vClients.push_back(convertLineToRecord(line));
        }

        manageClients.close();
    }
    
    return vClients;
}

void saveVectorToFile(string fileName, vector <stClient>& vClients) {

    fstream manageClients;
    manageClients.open(fileName, ios::out);

    if (manageClients.is_open()) {

        for (stClient& client : vClients) {
            manageClients << convertRecordToLine(client) << endl;
        }

        manageClients.close();
    }

}

enATM readService() {
    short num{ 0 };

    do {
        cout << "Choose what do you want to do? [1 to 6]? ";
        cin >> num;
    } while (num < 1 || num > 6);

    return (enATM)num;
}

enQuickWithdraw readQuickWithdraw() {
    short num{ 0 };

    do {
        cout << "Choose what amount do you want to withdraw? [1 to 9]? ";
        cin >> num;
    } while (num < 1 || num > 9);

    return (enQuickWithdraw)num;
}

string tabs(short num) {
    string tab = "";
    for (short i = 1; i <= num; i++) {
        tab += "\t";
    }

    return tab;
}

void serviceHeader(string serviceName) {
    system("cls");
    cout << "---------------------------------------\n";
    cout << tabs(1) << serviceName << endl;
    cout << "---------------------------------------\n";
}

void show_ATM_Menu_Options() {
    system("cls");
    cout << "===============================================\n";
    cout << tabs(2) << "Main ATM Menu Screen\n";
    cout << "===============================================\n";
    cout << tabs(1) << "[1] Quick Withdraw\n";
    cout << tabs(1) << "[2] Normal Withdraw\n";
    cout << tabs(1) << "[3] Deposit\n";
    cout << tabs(1) << "[4] Check Balance\n";
    cout << tabs(1) << "[5] Change Password\n";
    cout << tabs(1) << "[6] Logout\n";
    cout << "===============================================\n";
}

void show_Quick_Withdraw_Options() {
    system("cls");
    cout << "===============================================\n";
    cout << tabs(2) << "Quick Withdraw Screen\n";
    cout << "===============================================\n";
    cout << tabs(1) << "[1] 20" << tabs(2) << " [2] 50\n";
    cout << tabs(1) << "[3] 100" << tabs(2) << " [4] 200\n";
    cout << tabs(1) << "[5] 400" << tabs(2) << " [6] 600\n";
    cout << tabs(1) << "[7] 800" << tabs(2) << " [8] 1000\n";
    cout << tabs(1) << "[9] Cancel\n";
    cout << "===============================================\n";
}

void goBackToMainMenu() {
    cout << "\n\nPress any key to go back to main menu...";
    system("pause>0");
    startSystem();
}

int getBalanceAmount(string accountNumber, string PINCode, string fileName) {

    vector <stClient> vClients = loadDataFromFile(fileName);

    for (stClient& client : vClients) {
        if (client.accountNumber == accountNumber && client.PINCode == PINCode)
            return client.balance;
    }
}

bool findClientByAccountNumber(string accountNumber, vector<stClient> &vClients, stClient& client) {

    for (stClient& C : vClients) {
        if (C.accountNumber == accountNumber) {
            client = C;
            return true;
        }
    }

    return false;
}

bool isValidClient(string accountNumber, string PINCode, vector<stClient>& vClients) {

    for (stClient& client : vClients) {
        if (client.accountNumber == accountNumber && client.PINCode == PINCode)
            return true;
    }
    
    return false;
}

void showBalance() {

    serviceHeader("Show Balance Screen");

    cout << "\nYour Balance is " << getBalanceAmount(CURRENT_CLIENT.accountNumber, CURRENT_CLIENT.PINCode, CLIENTS_FILE) << " $\n";
}

bool withdraw(int withdrawAmount, string fileName, stClient &clint) {

    vector<stClient> vClients = loadDataFromFile(fileName);

    for (stClient& C : vClients) {
        if (C.accountNumber == clint.accountNumber && C.PINCode == clint.PINCode) {
            cout << "\nYour balance is " << C.balance << endl;

            if (withdrawAmount % 5 == 0) {
                if (C.balance >= withdrawAmount) {

                    char confirm = 'n';

                    cout << "\nAre you sure you want to perform this transaction (y/n)? ";
                    cin >> confirm;

                    if (confirm == 'y' || confirm == 'Y') {
                        C.balance -= withdrawAmount;
                        saveVectorToFile(CLIENTS_FILE, vClients);
                        cout << "\n\nDone successfully! your new balance is " << C.balance << endl;
                        return true;
                    }
                }
                else {
                    cout << "\nThe amount exceeds your balance, you can withdraw up to " << C.balance << endl;
                }
            }
        }     
    }

    return false;
}

void deposit() {

    serviceHeader("Deposit Screen");

    int amount = 0;
    do {
        cout << "\n\nEnter a positive deposit amount: ";
        cin >> amount;
    } while (amount <= 0);

    stClient client;
    vector <stClient> vClients = loadDataFromFile(CLIENTS_FILE);

    if (findClientByAccountNumber(CURRENT_CLIENT.accountNumber, vClients, client)) {

        client.balance += amount;

        for (stClient& C : vClients) {
            if (C.accountNumber == client.accountNumber) {
                C = client;
                saveVectorToFile(CLIENTS_FILE,vClients);
                cout << "\n\nDone successfully! your new balance is " << C.balance << endl;
                break;
            }
        }

    }
}

void performQuickWithdrawOption(enQuickWithdraw option) {
    switch (option)
    {
    case en_amount_20:
        withdraw(20, CLIENTS_FILE, CURRENT_CLIENT);
        goBackToMainMenu();
        break;
    case en_amount_50:
        withdraw(50, CLIENTS_FILE, CURRENT_CLIENT);
        goBackToMainMenu();
        break;
    case en_amount_100:
        withdraw(100, CLIENTS_FILE, CURRENT_CLIENT);
        goBackToMainMenu();
        break;
    case en_amount_200:
        withdraw(200, CLIENTS_FILE, CURRENT_CLIENT);
        goBackToMainMenu();
        break;
    case en_amount_400:
        withdraw(400, CLIENTS_FILE, CURRENT_CLIENT);
        goBackToMainMenu();
        break;
    case en_amount_600:
        withdraw(600, CLIENTS_FILE, CURRENT_CLIENT);
        goBackToMainMenu();
        break;
    case en_amount_800:
        withdraw(800, CLIENTS_FILE, CURRENT_CLIENT);
        goBackToMainMenu();
        break;
    case en_amount_1000:
        withdraw(1000, CLIENTS_FILE, CURRENT_CLIENT);
        goBackToMainMenu();
        break;
    case en_Cancel:
        startSystem();
        break;
    default:
        startSystem();
        break;
    }
}

void quickWithdraw() {
    show_Quick_Withdraw_Options();
    performQuickWithdrawOption(readQuickWithdraw());
}

void normalWithDraw() {
    serviceHeader("Normal Withdraw Screen");

    int amount = 0;
    cout << "\nEnter an amount multiple by 5's: ";
    cin >> amount;

    while (amount % 5 != 0) {
        cout << "\n\nEnter an amount multiple by 5's: ";
        cin >> amount;
    }

    withdraw(amount, CLIENTS_FILE, CURRENT_CLIENT);
}

void startSystem() {
    show_ATM_Menu_Options();
    perform_ATM_Service(readService());
}

void login() {

    serviceHeader("Login Screen");

    vector<stClient> vClients = loadDataFromFile(CLIENTS_FILE);

    string accountNumber, PINCode;
    cout << "\nEnter Account Number: ";
    getline(cin >> ws, accountNumber);
    cout << "Enter PIN code: ";
    getline(cin, PINCode);

    if (isValidClient(accountNumber, PINCode, vClients)) {
        stClient client;
        if (findClientByAccountNumber(accountNumber, vClients, client)) {
            CURRENT_CLIENT = client;
        }
    }
    else {
        while (!isValidClient(accountNumber, PINCode, vClients)) {
            serviceHeader("Login Screen");

            cout << "Invalid Account Number/PINCode!\n";

            cout << "\nEnter Account Number: ";
            getline(cin >> ws, accountNumber);
            cout << "Enter PIN code: ";
            getline(cin, PINCode);
        }
    }

    startSystem();

}

void logout() {
    login();
}

string readNewPINCode() {
    string newPassword = "";
    string confirmPassword = "";

    cout << "\nEnter new PIN code: ";
    cin >> newPassword;
    cout << "Confirm PIN code: ";
    cin >> confirmPassword;

    while (confirmPassword != newPassword) {
        cout << "\nPlease enter the same PIN code in both new and confirm!\n";

        cout << "\nEnter new PIN code: ";
        cin >> newPassword;
        cout << "Confirm PIN code: ";
        cin >> confirmPassword;
    }

    return newPassword;
}

void changePassowrd(string accountNumber, string fileName) {

    vector<stClient> vClients = loadDataFromFile(fileName);

    for (stClient& client : vClients) {
        if (client.accountNumber == accountNumber) {
            client.PINCode = readNewPINCode();
            saveVectorToFile(fileName, vClients);
            cout << "\n\nDone successfully!\n";
            break;
        }
    }

}

void showChangePasswordScreen() {
    serviceHeader("Change Password Screen");
    changePassowrd(CURRENT_CLIENT.accountNumber, CLIENTS_FILE);
}

void perform_ATM_Service(enATM service) {
    switch (service)
    {
    case en_Quick_withdraw:
        quickWithdraw();
        goBackToMainMenu();
        break;
    case en_Normal_withdraw:
        normalWithDraw();
        goBackToMainMenu();
        break;
    case en_Deposit:
        deposit();
        goBackToMainMenu();
        break;
    case en_Check_Balance:
        showBalance();
        goBackToMainMenu();
        break;
    case en_Change_PINCode:
        showChangePasswordScreen();
        goBackToMainMenu();
    case en_Logout:
        logout();
        break;
    default:
        logout();
        break;
    }
}

int main()
{
    login();

    return 0;
}