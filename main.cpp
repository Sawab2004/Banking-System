#include<iostream>
#include <iomanip>
#include <string>
#include <map>
#include <list>

using namespace std;

class Transaction
{
public:
	string type, date;
	double amount;

    Transaction(const string& t, double a, const string& d):type(t), amount(a), date(d) {}
};

class Account {
private:
    string accountNumber;
    double balance;
    list<Transaction> transactions;

public:
    Account() : accountNumber(""), balance(0.0) {}
    Account(const string& accNum, double initialBalance)
        : accountNumber(accNum), balance(initialBalance) {}

    void deposit(double amount, const string& date) {
        if (amount > 0) {
            balance += amount;
            transactions.push_back(Transaction("Deposit", amount, date));
            cout << "Deposited: $" << amount << ". New balance: $" << balance << endl;
        }
        else {
            cout << "Deposit amount must be positive!" << endl;
        }
    }

    bool withdraw(double amount, const string& date) {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            transactions.push_back(Transaction("Withdrawal", amount, date));
            cout << "Withdrew: $" << amount << ". New balance: $" << balance << endl;
            return true;
        }
        else {
            cout << "Insufficient funds or invalid amount!" << endl;
            return false;
        }
    }

    void transfer(Account& toAccount, double amount, const string& date) {
        if (withdraw(amount, date)) 
        {
            toAccount.deposit(amount, date);
            cout << "Transferred: $" << amount << " to account " << toAccount.accountNumber << endl;
        }
    }

    void displayInfo() const {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Balance: $" << balance << endl;
    }

    void viewTransactions() const {
        cout << "Transactions for account " << accountNumber << ":" << endl;
        for (const auto& transaction : transactions) 
        {
            cout << transaction.date << " - " << transaction.type << ": $" << transaction.amount << endl;
        }
    }
};

class Customer {
private:
    string name;
    string id;
    Account* account; 

public:
    Customer() : name(""), id(""), account(nullptr) {}

    Customer(const string& n, const string& i, Account* acc)
        : name(n), id(i), account(acc) {}

    void viewAccountInfo() const {
        cout << "Customer Name: " << name << endl;
        cout << "Customer ID: " << id << endl;
        account->displayInfo();
    }

    Account* getAccount() const {
        return account;
    }
};

class BankingService {
public:
    static void deposit(Account& acc, double amount, const string& date) {
        acc.deposit(amount, date);
    }

    static void withdraw(Account& acc, double amount, const string& date) {
        acc.withdraw(amount, date);
    }

    static void transfer(Account& fromAcc, Account& toAcc, double amount, const string& date) {
        fromAcc.transfer(toAcc, amount, date);
    }
};


int main()
{
    map<string, Customer> customers;
    map<string, Account> accounts;

    int choice;
    string name, id, accNum, date;
    double amount;

    while (true) {
        cout << "\nBanking System Menu:\n";
        cout << "1. Add Customer\n";
        cout << "2. Add Account\n";
        cout << "3. View Account Info\n";
        cout << "4. Perform Transaction\n";
        cout << "5. View Transactions\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();
        switch (choice) {
        case 1:
        {
            cout << "Enter customer name: ";
            getline(cin, name);
            cout << "Enter customer ID: ";
            getline(cin, id);
            cout << "Enter account number for the customer: ";
            getline(cin, accNum);

            if (accounts.find(accNum) != accounts.end())
            {
                customers[id] = Customer(name, id, &accounts[accNum]);
                cout << "Customer added successfully.\n";
            }
            else
            {
                cout << "Account number not found!\n";
            }
            break;
        }

        case 2:
        {
            cout << "Enter account number: ";
            getline(cin, accNum);
            cout << "Enter initial balance: ";
            cin >> amount;
            cin.ignore(); // Ignore newline character

            if (accounts.find(accNum) == accounts.end()) {
                accounts[accNum] = Account(accNum, amount);
                cout << "Account added successfully.\n";
            }
            else {
                cout << "Account number already exists!\n";
            }
            break;
        }
        case 3:
        {
            cout << "Enter customer ID to view account info: ";
            getline(cin, id);

            if (customers.find(id) != customers.end()) {
                customers[id].viewAccountInfo();
            }
            else {
                cout << "Customer ID not found!\n";
            }
            break;
        }
        case 4:
        {
            cout << "Enter customer ID to perform transaction: ";
            getline(cin, id);
            if (customers.find(id) != customers.end()) 
            {
                Account* fromAccount = customers[id].getAccount();
                cout << "Enter transaction type (Deposit/Withdrawal/Transfer): ";
                string type;
                getline(cin, type);

                if (type == "Deposit" || type == "Withdrawal") 
                {
                    cout << "Enter amount: ";
                    cin >> amount;
                    cin.ignore(); 
                    cout << "Enter date (YYYY-MM-DD): ";
                    getline(cin, date);
                    if (type == "Deposit") {
                        BankingService::deposit(*fromAccount, amount, date);
                    }
                    else if (type == "Withdrawal") {
                        BankingService::withdraw(*fromAccount, amount, date);
                    }
                }
                else if(type == "Transfer")
                {
                    cout << "Enter target account number: ";
                    getline(cin, accNum);
                    if (accounts.find(accNum) != accounts.end()) {
                        Account* toAccount = &accounts[accNum];
                        cout << "Enter amount: ";
                        cin >> amount;
                        cin.ignore();
                        cout << "Enter date (YYYY-MM-DD): ";
                        getline(cin, date);

                        BankingService::transfer(*fromAccount, *toAccount, amount, date);
                    }
                    else {
                        cout << "Target account number not found!\n";
                    }
                }
                else {
                    cout << "Invalid transaction type!\n";
                }
            }
            else {
                cout << "Customer ID not found!\n";
            }
            break;
        }
        case 5: {
            cout << "Enter customer ID to view transactions: ";
            getline(cin, id);

            if (customers.find(id) != customers.end()) {
                customers[id].getAccount()->viewTransactions();
            }
            else {
                cout << "Customer ID not found!\n";
            }
            break;
        }

        case 6: {
            cout << "Exiting the system..." << endl;
            return 0;
        }

        default: {
            cout << "Invalid choice! Please try again." << endl;
            break;
        }

        }

    }

    return 0;
}