#include <iostream>
#include <conio.h>
#include <stdio.h>
#include "ManageUser.h"
#include "ManageTransaction.h"
#include "User.h"

using namespace std;

#define ARRAY_SIZE 5

User* curUser = new User();

void mainMenuCustomer();
void mainMenuStaff();
void menuCustomer1();
void menuCustomer2();
void menuStaff1();
void menuStaff2(); 
void listTransaction();
void summary();

//-------------------- Hide Password ----------------------------------
std::string takePass(char sp = '*')
{
	const int IN_BACK = 8;
	const int IN_RET = 13;
	// Stores the password
	string passwd = "";
	char ch_ipt;
	// Until condition is true
	while (true) {
		ch_ipt = _getch();
		// if the ch_ipt
		if (ch_ipt == IN_RET) {
			std::cout << endl;
			return passwd;
		}
		else if (ch_ipt == IN_BACK
			&& passwd.length() != 0) {
			passwd.pop_back();
			// std::cout statement is very important as it will erase previously printed character
			std::cout << "\b \b";
			continue;
		}
		// Without using this, program will crash as \b can't be print in beginning of line 
		else if (ch_ipt == IN_BACK && passwd.length() == 0) {
			continue;
		}
		passwd.push_back(ch_ipt);
		std::cout << sp;
	}
}
bool isNumber(const std::string& str)
{
	for (char const& c : str) {
		if (std::isdigit(c) == 0) return false;
	}
	return true;
}

//-------------------  Get current customer details --------------------
Customer* currentCus() {
	ManageTransaction mt;
	Customer* cus = mt.searchCustomer(curUser->currentAcc);
	return cus;
}

//-------------------- Design Table ------------------------------------
void displayRow(char character, int length) {
	std::cout << setfill(character) << setw(length);
}

//---------------------- Exception function ----------------------------
void exceptionNotNumber() {
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cout << "\n\t\t\t\t\t<<Invalid input. Please enter number.>>\n\n";
	cout << "\t\t\t\t\t"; system("pause");
}

bool validIc(std::string ic) 
{
	bool status = true;
	if (ic.size() < 12 || ic.size() > 12)
	{
		status = false;
	}
	for (int i = 0; i < ic.size(); i++) {
		if (!isdigit(ic[i])) {
			status = false;
		}
	}
	return status;
}
//bool validTel(std::string tel)
//{
//	bool status = true;
//	if (tel.size() < 12 || tel.size() > 12)
//	{
//		status = false;
//	}
//	for (int i = 0; i < tel.size(); i++) {
//		if (tel[0] != 0 || tel[1] != 1) {
//			status = false;
//		}
//		if (!isdigit(tel[i])) {
//			status = false;
//		}
//	}
//	return status;
//}
//-------------------- MAIN FUNCTION -----------------------------------
int main()
{
	std::string account, passStr = "";
	int pass = NULL;
	curUser->userType = "";
	LoginRegister lr; ManageUser mu;
	int count = 1;
	do {
		std::system("cls");
		displayRow('*', 120); std::cout << "\n";
		displayRow(' ', 65); std::cout << " SB BANK "; std::cout << "\n";
		displayRow('*', 122); std::cout << "\n\n\n";
		displayRow(' ', 70); std::cout << "Please enter account id: "; 
		std::cin >> account;
		displayRow(' ', 54); std::cout << "Password:";
		passStr = takePass();
		if (!isNumber(passStr) || passStr == "") {
			exceptionNotNumber();
			main();
		}
		pass = stoi(passStr);
		curUser->userType = lr.login(account, pass);
		curUser->currentAcc = account;
		curUser->PIN = passStr;
		
		if (curUser->userType == "admin") {
			mainMenuStaff();
		}
		else if (curUser->userType == "customer") {
			mainMenuCustomer();
		}
		else if (curUser->userType == "adminfalse") {
			std::cout << "\n\t\t\t\t\t<<Sorry, you have tried more than 3 times.>>\n";
			break;
		}else {
			if (count == 3) {
				Customer* cus = new Customer();
				std::cout << "\n\t\t\t\t\t<<Sorry, you have tried more than 3 times.>>\n";
				cus = mu.searchCustomers(2,account);
				if (cus != NULL) {
					mu.updateAcc(cus->Ic, 2);
				}
				break;
			}
			count++;
		}
		cout << "\t\t\t\t\t"; std::system("pause");
	} while (curUser->userType == "" && count <= 3);
	

	return 0;
}

//-------------------- Customer1 menu functions ------------------------
void depositTrans()
{
	std::system("cls");
	double amount; string desc = ""; int choice = NULL, success;
	displayRow('*', 120); std::cout << "\n";
	displayRow(' ', 65); std::cout << " SB BANK "; displayRow(' ', 55); std::cout << "\n";
	displayRow('*', 121); std::cout << "\n\n";
	displayRow(' ', 74); std::cout << "**Deposit Transaction**\n\n";
	displayRow('-', 120); std::cout << "\n\n";
	displayRow(' ', 58); std::cout << "Amount (RM): "; 
	if (!(std::cin >> amount)){
		exceptionNotNumber();
		depositTrans();
	}
		displayRow(' ', 68); std::cout << "Transaction Reference: "; getline(cin >> ws, desc);
		displayRow('-', 120); std::cout << "\n\n";
		do {
			displayRow(' ', 69); std::cout << "[1]Continue Transaction\n";
			displayRow(' ', 63); std::cout << "[2]Change Amount\n\n";
			displayRow(' ', 56); std::cout << "[0]Cancel\n\n";
			displayRow('-', 120); std::cout << "\n";
			while (std::cout << "\t\t\t\t\t     Enter option:" && !(std::cin >> choice))	exceptionNotNumber();
			if (choice != 0 && choice != 1 && choice != 2) std::cout << "\n\t\t\t\t\t<<The number does not in options.>>\n\n";
		} while (choice != 0 && choice != 1 && choice != 2);
		if (choice == 1) {
			ManageTransaction mt;
			success = mt.transactionAcc(curUser->currentAcc, amount, desc);
			cout << "\n\n";
			if (success == 0) {
				displayRow(' ', 75); std::cout << "<<Transaction failed to process.>>\n";
			}
			else {
				displayRow(' ', 75); std::cout << "<<Transaction succeed to process.>>\n";
			}
			cout << "\t\t\t\t\t"; std::system("pause");
		}
		else if (choice == 2)
			depositTrans();
		menuCustomer1();	
}

void withdrawTrans()
{
	std::system("cls");
	double amount; int choice = NULL, success;
	displayRow('*', 120); std::cout << "\n";
	displayRow(' ', 65); std::cout << " SB BANK "; displayRow(' ', 55); std::cout << "\n";
	displayRow('*', 121); std::cout << "\n\n";
	displayRow(' ', 77); std::cout << "**Withdrawal Transaction**\n\n";
	displayRow('-', 120); std::cout << "\n\n";
	displayRow(' ', 61); std::cout << "Amount (RM):";
	if (!(cin >> amount)) {
		exceptionNotNumber();
		withdrawTrans();
	}
	displayRow('-', 120); std::cout << "\n\n";
	do {
		displayRow(' ', 73); std::cout << "[1]Continue Transaction\n";
		displayRow(' ', 67); std::cout << "[2]Change Amount\n\n";
		displayRow(' ', 60); std::cout << "[0]Cancel\n\n";
		displayRow('-', 120); std::cout << "\n";
		while (std::cout << "\t\t\t\t\t     Enter option:" && !(cin >> choice))	exceptionNotNumber();
		if (choice != 0 && choice != 1 && choice != 2)	std::cout << "\n\t\t\t\t\t<<The number does not in options.>>\n\n";
	} while (choice != 0 && choice != 1 && choice != 2);
	if (choice == 1) {
		ManageTransaction mt;
		success = mt.transactionAcc(curUser->currentAcc, amount);
		cout << "\n\n";
		if (success == 0) {
			displayRow(' ', 75); std::cout << "<<Transaction failed to process.>>\n";
		}
		else {
			displayRow(' ', 75); std::cout << "<<Transaction succeed to process.>>\n";
		}
		cout << "\t\t\t\t\t"; std::system("pause");
	}
	else if (choice == 2)
		withdrawTrans();
	menuCustomer1();
}

void transferTrans()
{
	std::system("cls");
	ManageTransaction mt;
	double amount = 0; string desc = "", receiverAcc = ""; int choice = NULL, success;
	displayRow('*', 120); std::cout << "\n";
	displayRow(' ', 65); std::cout << " SB BANK "; displayRow(' ', 55); std::cout << "\n";
	displayRow('*', 121); std::cout << "\n\n";
	displayRow(' ', 75); std::cout << "**Transfer Transaction**\n\n";
	displayRow('-', 120); std::cout << "\n\n";
	displayRow(' ', 70); std::cout << "Receiver Account Number: "; getline(cin >> ws, receiverAcc);
	displayRow(' ', 58); std::cout << "Amount (RM): "; 
	if (!(cin >> amount)) {
		exceptionNotNumber();
		transferTrans();
	};
	displayRow(' ', 68); std::cout << "Transaction Reference: "; getline(cin >> ws, desc);
	if (desc.size() > 50) {
		cout << "\n\t\t\t\t\tDescription is too long. Please insert again.\n";
		cout << "\t\t\t\t\t"; std::system("pause");
		transferTrans();
	}
	Customer* receiver = mt.searchCustomer(receiverAcc);
	if (receiver == NULL) {
		std::cout << "\t\t\t\t\t\t<<Account number does not exist.>>\n";
		cout << "\t\t\t\t\t"; system("pause");
		transferTrans();
	}
	if (receiverAcc == curUser->currentAcc) {
		std::cout << "\t\t\t\t\t\t<<Cannot transfer to your own account.>>\n";
		cout << "\t\t\t\t\t"; system("pause");
		transferTrans();
	}
	std::system("cls");
	displayRow('*', 120); std::cout << "\n";
	displayRow(' ', 65); std::cout << " SB BANK "; displayRow(' ', 55); std::cout << "\n";
	displayRow('*', 121); std::cout << "\n\n";
	displayRow(' ', 72); std::cout << "**Receiver Details**\n\n";
	displayRow('-', 120); std::cout << "\n\n";
	displayRow(' ', 59); std::cout << "Receiver Name: " << receiver->Name << "\n";
	displayRow(' ', 69); std::cout << "Receiver Account Number: " << receiver->AccountNo << "\n";
	displayRow(' ', 63); std::cout << "Transfer Amount: RM" << amount << "\n";
	displayRow(' ', 54); std::cout << "Reference:" << desc << "\n\n";
	displayRow('-', 120); std::cout << "\n\n";
	do {
		displayRow(' ', 68); std::cout << "[1]Continue Transaction\n";
		displayRow(' ', 62); std::cout << "[2]Change Amount\n\n";
		displayRow(' ', 55); std::cout << "[0]Cancel\n\n";
		displayRow('-', 120); std::cout << "\n";
		while (std::cout << "\t\t\t\t\t     Enter option:" && !(cin >> choice))	exceptionNotNumber();
		if (choice != 0 && choice != 1 && choice != 2) {
			std::cout << "\n"; displayRow(' ', 75); std::cout << "<<The number not in option.>>\n\n";
		}
	} while (choice != 0 && choice != 1 && choice != 2);
	if (choice == 1) {
		ManageTransaction mt;
		success = mt.transactionAcc(curUser->currentAcc, amount, desc, receiverAcc);
		cout << "\n\n";
		if (success == 0) {
			displayRow(' ', 75); std::cout << "<<Transaction failed to process.>>\n";
		}
		else {
			displayRow(' ', 75); std::cout << "<<Transaction succeed to process.>>\n";
		}
		cout << "\t\t\t\t\t"; std::system("pause");
	}
	else if (choice == 2)
		transferTrans();
	menuCustomer1();
}

void menuCustomer1()
{
	std::system("cls");
	int choice;
	try {
		Customer* cus = currentCus();
		displayRow('*', 120); std::cout << "\n";
		displayRow(' ', 65); std::cout << " SB BANK "; displayRow(' ', 55); std::cout << "\n";
		displayRow('*', 122); std::cout << "\n\n\n";
		displayRow(' ', 72); std::cout << "**Money Transaction**\n\n";
		displayRow(' ', 65); std::cout << "Current Amount: RM " << fixed << setprecision(2) << cus->CurAmount << "\n\n";
		displayRow(' ', 63); std::cout << "1. Deposit Money\n";
		displayRow(' ', 64); std::cout << "2. Withdraw Money\n";
		displayRow(' ', 65); std::cout << "3. Transfer Money\n\n";
		displayRow(' ', 60); std::cout << "0. Back Menu\n\n";
		displayRow('-', 120); std::cout << "\n";
		displayRow(' ', 60); std::cout << "Enter option: "; 
		if (!(cin >> choice)) throw 1;

		switch (choice)
		{
		case 1:
			depositTrans();
			break;
		case 2:
			withdrawTrans();
			break;
		case 3:
			transferTrans();
			break;
		case 0:
			mainMenuCustomer();
			break;
		default:
			throw 2;
			break;
		}
	}
	catch (int err) {
		switch (err) {
		case 1:
			exceptionNotNumber();
			menuCustomer1();
			break;
		case 2:
			std::cout << "\n\t\t\t\t\t<<The number does not in options.>>" << endl;
			cout << "\t\t\t\t\t"; system("pause");
			menuCustomer1();
			break;
		}
	}
}

//-------------------- Customer2 menu functions ------------------------

void checkBalance()
{
	std::system("cls");
	Customer* cus = currentCus();
	DbConnect conn;
	char curDate[40];
	displayRow('*', 120); std::cout << "\n";
	displayRow(' ', 65); std::cout << " SB BANK "; displayRow(' ', 55); std::cout << "\n";
	displayRow('*', 120); std::cout << "\n\n\n";
	displayRow(' ', 70); std::cout << "**Check Balance**\n\n";
	displayRow('-', 120); std::cout << "\n\n";
	displayRow(' ', 65); std::cout << "Current Amount: RM " << fixed << setprecision(2) << cus->CurAmount << "\n";
	conn.getCurrentTime(curDate);
	displayRow(' ', 62); std::cout << "Last updated on " << curDate << "\n\n";
	displayRow('-', 120); std::cout << "\n";
	system("pause>nul");
	menuCustomer2();
}

void accountStatement()
{
	system("cls");
	Customer* cus = currentCus(); Transaction* trans = NULL;
	DbConnect conn; ManageTransaction mt;
	char curDate[40]; int choice = NULL;
	displayRow('*', 120); std::cout << "\n";
	displayRow(' ', 65); std::cout << " SB BANK "; displayRow(' ', 55); std::cout << "\n";
	displayRow('*', 122); std::cout << "\n\n\n";
	displayRow(' ', 73); std::cout << "**Account Statement**\n\n";
	displayRow(' ', 67); std::cout << "Current Amount: RM " << fixed << setprecision(2) << cus->CurAmount << "\n\n\n";
	displayRow(' ', 67); std::cout << "[1] Last 30 days\n";
	displayRow(' ', 67); std::cout << "[2] Last 60 days\n";
	displayRow(' ', 67); std::cout << "[3] Last 90 days\n";
	displayRow(' ', 62); std::cout << "[0] Cancel\n\n";
	displayRow('-', 120); std::cout << "\n";
	std::cout << "\t\t\t\t\t\tEnter option:";
	if (!(cin >> choice)) { 
		exceptionNotNumber(); 
		accountStatement();
	}
	switch (choice)
	{
	case 1:
		trans = mt.getTransaction(cus->AccountNo,30);
		break;
	case 2:
		trans = mt.getTransaction(cus->AccountNo,60);
		break;
	case 3:
		trans = mt.getTransaction(cus->AccountNo,90);
		break;
	case 0:
		menuCustomer2();
		break;
	default:
		std::cout << "\n\t\t\t\t\t<<The number not in options.>>" << endl;
		cout << "\t\t\t\t\t"; system("pause");
		accountStatement();
		break;
	}
	if (trans != NULL)
	{
		system("cls");
		displayRow('*', 120); std::cout << "\n";
		displayRow(' ', 65); std::cout << " SB BANK "; displayRow(' ', 55); std::cout << "\n";
		displayRow('*', 122); std::cout << "\n\n\n";
		
		switch (choice)
		{
		case 1:
			displayRow(' ', 78); std::cout << "**Last 30 days of Transaction**\n\n";
			break;
		case 2:
			displayRow(' ', 78); std::cout << "**Last 60 days of Transaction**\n\n";
			break;
		case 3:
			displayRow(' ', 78); std::cout << "**Last 90 days of Transactiont**\n\n";
			break;
		}
		Transaction* t = trans;
		displayRow('-', 120); std::cout << "\n";
		do
		{
			std::cout << "\t\t\t| Date Time: " << t->Date << "\n";
			std::cout << "\t\t\t| "; displayRow(' ', 15); std::cout <<left<< t->Id;
			if (t->TransType == "Deposit") {
				displayRow(' ', 50); std::cout << right << "+ RM " << fixed << setprecision(2) << t->Amount << "\n";
			}
			else if (t->TransType == "Transfer") {
				if (curUser->currentAcc == t->ReceiveAcc) {
					//cout << "Current Acc:" << curUser->currentAcc << "\t" << t->ReceiveAcc;
					displayRow(' ', 50); std::cout << right << "+ RM " << fixed << setprecision(2) << t->Amount <<"\n";
				}
				if (curUser->currentAcc != t->ReceiveAcc) {
					//cout << "Current Acc:" << curUser->currentAcc << "\t" << t->ReceiveAcc;
					displayRow(' ', 50); std::cout << right << "- RM " << fixed << setprecision(2) << t->Amount <<"\n";
				}
			}
			else {
				displayRow(' ', 50); std::cout << right << "- RM " << fixed << setprecision(2) << t->Amount << "\n";
			}
			if (t->TransType == "Transfer") {				
				if (curUser->currentAcc == t->ReceiveAcc) {
					Customer* sender = mt.searchCustomer(t->TransAcc);
					std::cout << "\t\t\t| From Account: "; std::cout << sender->Name << "\n";
				}
				if (curUser->currentAcc != t->ReceiveAcc) {
					Customer* receiver = mt.searchCustomer(t->ReceiveAcc);
					std::cout << "\t\t\t| Receive Account: "; std::cout << receiver->Name  << "\n";
				}
			}
			if (t->TransType == "Deposit" || t->TransType == "Transfer") {
				std::cout << "\t\t\t| Description: " << t->Desc << "\n";
			}
			std::cout << "\n";
			displayRow('-', 120); std::cout << "\n";
			t = t->next;
		} while (t != NULL);
		std::cout << setfill('-') << setw(120) << endl;
	}
	else {
		displayRow(' ', 73); std::cout << "<<No transaction found.>>" << endl;
	}
	std::system("pause>nul");
	menuCustomer2();
}

void menuCustomer2()
{
	system("cls");
	int choice;
	try {
		displayRow('*', 120); std::cout << "\n";
		displayRow(' ', 65); std::cout << " SB BANK \n";
		displayRow('*', 122); std::cout << "\n\n\n";
		displayRow(' ', 71); std::cout << "**Funds Summary**\n\n";
		displayRow(' ', 66); std::cout << "1. Check Balance\n";
		displayRow(' ', 71); std::cout << "2. Account Statement\n\n";
		displayRow(' ', 63); std::cout << "0. Back Menu\n\n";
		displayRow('-', 120); std::cout << "\n";
		displayRow(' ', 63); std::cout << "Enter option: "; 
		if (!(cin >> choice))throw 1;

		switch (choice)
		{
		case 1:
			checkBalance();
			break;
		case 2:
			accountStatement();
			break;
		case 0:
			mainMenuCustomer();
			break;
		default:
			throw 2;
			break;
		}
	}
	catch (int err) {
		switch (err) {
		case 1:
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "\n\t\t\t\t\t<<Invalid input. Please enter number.>>\n\n";
			cout << "\t\t\t\t\t"; system("pause");
			menuCustomer2();
			break;
		case 2:
			std::cout << "\n\t\t\t\t\t<<The number does not in options.>>" << endl;
			cout << "\t\t\t\t\t"; system("pause");
			menuCustomer2();
			break;
		}
	}
}

//-------------------- Staff1 menu functions ------------------------
void registerAccount()
{
	std::system("cls");
	int choice, pin = NULL; double curAmount = NULL; string ic, name, address;
	displayRow('*', 120); std::cout << "\n";
	displayRow(' ', 65); std::cout << " SB BANK \n";
	displayRow('*', 122); std::cout << "\n\n\n";
	displayRow(' ', 71); std::cout << "**Register Account**\n\n";
	displayRow(' ', 66); std::cout << "1. Customer Account\n\n";
	displayRow(' ', 59); std::cout << "0. Back Menu\n\n";
	displayRow('-', 120); std::cout << "\n";
	displayRow(' ', 59); std::cout << "Enter option: "; 
	if (!(cin >> choice)) {
		exceptionNotNumber();
		registerAccount();
	};
	switch (choice) 
	{
	case 1:
	RegisterDetails:
		std::system("cls");
		displayRow('*', 120); std::cout << "\n";
		displayRow(' ', 65); std::cout << " SB BANK \n";
		displayRow('*', 122); std::cout << "\n\n\n";
		displayRow(' ', 80); std::cout << "Enter the following customer details:\n\n";
		displayRow(' ', 47); std::cout << "Name: "; getline(cin >> ws, name);
		if (name.size() > 100) {
			cout << "\n\t\t\t\t\tName is too long. Please insert again.\n";
			cout << "\t\t\t\t\t"; std::system("pause");
			goto RegisterDetails;
		}
		displayRow(' ', 69); std::cout << "Identification Card Number: "; getline(cin>>ws,ic);
		if (!validIc(ic)) {
			cout << "\n\t\t\t\t\tInvalid IC number. Please insert again.\n";
			cout << "\t\t\t\t\t"; std::system("pause");
			goto RegisterDetails;
		};
		displayRow(' ', 50); std::cout << "Address: "; getline(cin >> ws, address);
		if (address.size() > 250) {
			cout << "\n\t\t\t\t\Address is too long. Please insert again.\n";
			cout << "\t\t\t\t\t"; std::system("pause");
			goto RegisterDetails;
		}
		displayRow(' ', 52);
		while (std::cout << "PIN number:" && !(cin >> pin)) {
			exceptionNotNumber();
			displayRow(' ', 52);
		}
		if (to_string(pin).size() > 6) {
			cout << "\n\t\t\t\t\tPIN is longer than 6 numbers. Please insert again.\n";
			cout << "\t\t\t\t\t"; std::system("pause");
			goto RegisterDetails;
		}
		displayRow(' ', 71); 
		while (std::cout << "Open Account Amount (RM #.##):" && !(cin >> curAmount)) {
			exceptionNotNumber();
			displayRow(' ', 71);
		}
		std::cout << "\n";
		do {
			displayRow('-', 120); std::cout << "\n";
			displayRow(' ', 90); std::cout << "Are you sure you want to register this account? Yes[1]/ No[0] :";
			if (!(cin >> choice)) { exceptionNotNumber(); choice = NULL; }
			if (choice != 0 && choice != 1) std::cout << "\n\t\t\t\t\t<<The number does not in options.>>\n\n";
		} while (choice != 0 && choice != 1 && choice != NULL);

		if (choice == 1) {
			LoginRegister lr;
			int success = lr.registerAcc(name, address, ic, pin, curAmount);
			std::cout << "\n\n";
			if (success == 1) {
				std::cout << "\t\t\t\t\t<<New customer account created.>>\n";
			}
			else {
				std::cout << "\t\t\t\t\t<<Account registration has failed.>>\n";
			}
		}
		else {
			menuStaff1();
		}
		break;
	case 0:
		menuStaff1();
		break;
	default:
		std::cout << "\n\n\t\t\t\t\t<<The number does not in options.>>\n\n";
		cout << "\t\t\t\t\t"; std::system("pause");
		registerAccount();
		break;
	}
	cout << "\t\t\t\t\t"; std::system("pause");
	menuStaff1();
}

void updateAccount()
{
	std::system("cls");
	int choice, valueInt = NULL, success = NULL; string ic, valueString;
	ManageUser mu;
	Customer* cus = new Customer();
	displayRow('*', 120); std::cout << "\n";
	displayRow(' ', 65); std::cout << " SB BANK \n";
	displayRow('*', 122); std::cout << "\n\n\n";
	displayRow(' ', 71); std::cout << "**Update Account**\n\n";
	displayRow('-', 120); std::cout << "\n";
	displayRow(' ', 61); std::cout << "Enter IC Number:"; getline(cin >> ws, ic);
	cus = mu.searchCustomer(ic);
	if (cus != NULL) {
	CustomerDetails:
		std::system("cls");
		displayRow('*', 120); std::cout << "\n";
		displayRow(' ', 65); std::cout << " SB BANK \n";
		displayRow('*', 122); std::cout << "\n\n\n";
		displayRow(' ', 72); std::cout << "**Customer Details**\n\n";
		displayRow(' ', 63); std::cout << "Identification Card: " << cus->Ic << endl;
		displayRow(' ', 48); std::cout << "Name: " << cus->Name << endl;
		displayRow(' ', 58); std::cout << "Account Number: " << cus->AccountNo << endl;
		displayRow(' ', 51); std::cout << "Address: " << cus->Address << endl;
		displayRow(' ', 52); std::cout << "PIN Code: ";
		for (int i = 0; i < cus->PIN.size(); i++) cout << "*"; cout << endl;
		displayRow(' ', 50); std::cout << "Status: " << cus->Status << endl;
		displayRow(' ', 58); std::cout << "Current Amount : RM " << cus->CurAmount << endl << endl;
		displayRow('-', 120); std::cout << "\n";
	DetailOption:
		std::cout << "\n";
		displayRow(' ', 52); std::cout << "Details :\n";
		displayRow(' ', 51); std::cout << "[1] Name\n";
		displayRow(' ', 54); std::cout << "[2] Address\n";
		displayRow(' ', 56); std::cout << "[3] PIN Code\n\n";
		displayRow(' ', 54); std::cout << "[0] Cancel\n\n";
		displayRow('-', 120); std::cout << "\n";
		displayRow(' ', 55); std::cout << "Enter option:";
		if (!(cin >> choice)) {
			exceptionNotNumber();
			goto CustomerDetails;
		}
		if (choice == 1 || choice == 2) {
			displayRow(' ', 55); std::cout << "Enter value :"; getline(cin >> ws, valueString);
			success = mu.updateAcc(choice, cus->Ic, valueString);
		}
		else if (choice == 3) {
		EnterPIN:
			displayRow(' ', 55); std::cout << "Enter value :";
			valueString = takePass();
			if (!isNumber(valueString) || valueString == "") {
				exceptionNotNumber();
				goto EnterPIN;
			}
			if (valueString.size() > 6) {
				cout << "\n\t\t\t\t\tPIN is longer than 6 numbers.\n";
				cout << "\t\t\t\t\t"; std::system("pause>nul");
				goto EnterPIN;
			}
			valueInt = stoi(valueString);
			success = mu.updateAcc(choice, cus->Ic, valueInt);
		}
		else if (choice == 0) {
			menuStaff1();
		}
		else {
			cout << "\n\t\t\t\t\t<<The number not in the options.>>\n";
			cout << "\t\t\t\t\t"; system("pause");
			goto CustomerDetails;
		}
		std::cout << "\n\n";
		if (success == 1) {
			displayRow(' ', 78); std::cout << "<<Customer account has been updated.>>\n";
		}
		else {
			displayRow(' ', 62); std::cout << "<<Account updation has failed.>>\n";
		}
	}
	else {
		displayRow(' ', 82); std::cout << "<<No customer with the identification number.>>\n";
	}
	cout << "\t\t\t\t\t"; std::system("pause");
	menuStaff1();
}

void deactiveAccount()
{
	std::system("cls");
	int choice = NULL, success, status; string ic;
	ManageUser mu;
	Customer* cus = new Customer();
	displayRow('*', 120); std::cout << "\n";
	displayRow(' ', 65); std::cout << " SB BANK \n";
	displayRow('*', 122); std::cout << "\n\n\n";
	displayRow(' ', 75); std::cout << "**Active/Deactive Account**\n\n";
DetailOption:
	displayRow('-', 120); std::cout << "\n";
	displayRow(' ', 60); std::cout << "Enter IC Number:"; getline(cin >> ws, ic);
	cus = mu.searchCustomer(ic);
	if (cus != NULL) {
	CustomerDetails:
		std::system("cls");
		displayRow('*', 120); std::cout << "\n";
		displayRow(' ', 65); std::cout << " SB BANK "; displayRow(' ', 55); std::cout << "\n";
		displayRow('*', 122); std::cout << "\n\n\n";
		displayRow(' ', 75); std::cout << "**Customer Details:-\n\n\n";
		displayRow(' ', 64); std::cout << "Identification Card: " << cus->Ic << endl;
		displayRow(' ', 49); std::cout << "Name: " << cus->Name << endl;
		displayRow(' ', 59); std::cout << "Account Number: " << cus->AccountNo << endl;
		displayRow(' ', 52); std::cout << "Address: " << cus->Address << endl;
		displayRow(' ', 53); std::cout << "PIN Code: ";
		for (int i = 0; i < cus->PIN.size(); i++) cout << "*"; cout << endl;
		displayRow(' ', 51); std::cout << "Status: " << cus->Status << endl;
		displayRow(' ', 59); std::cout << "Current Amount: " << cus->CurAmount << endl << endl;
		displayRow('-', 120); std::cout << "\n";
		std::cout << "\n";
		displayRow(' ', 52); std::cout << "Action :\n";
		displayRow(' ', 56); std::cout << "[1] Activate\n";
		displayRow(' ', 58); std::cout << "[2] Deactivate\n";
		displayRow(' ', 55); std::cout << "[0] Cancel\n\n";
		displayRow('-', 120); std::cout << "\n";
		displayRow(' ', 56); std::cout << "Enter option:";
		if (!(cin >> status)) {
			exceptionNotNumber();
			goto CustomerDetails;
		}
		if (status != 0 && status != 1 && status != 2) {
			cout << "\n\t\t\t\t\t<<The number not in the options.>>\n";
			cout << "\t\t\t\t\t"; system("pause");
			goto CustomerDetails;
		}
		std::cout << "\n"; displayRow('-', 120); std::cout << "\n";
		if (status == 0) 
			menuStaff1();
		else {
			do {
				if (status == 1) {
					displayRow(' ', 95);
					while (std::cout << "Are you sure you want to activate this account? Yes[1]/ No[0] :" && !(cin >> choice)) {
						exceptionNotNumber();
						displayRow(' ', 95);
					}
				}
				else {
					displayRow(' ', 95);
					while (std::cout << "Are you sure you want to deactive this account? Yes[1]/ No[0] :" && !(cin >> choice)) {
						exceptionNotNumber();
						displayRow(' ', 95);
					}
				}
				if (choice != 0 && choice != 1) {
					cout << "\n\t\t\t\t\t<<The number not in the options.>>\n\n";
				}
			} while (choice != 0 && choice != 1);
			if (choice == 1) {
				success = mu.updateAcc(cus->Ic, status);
				if (success == 1) {
					std::cout << "\n\t\t\t\t\t<<Status account has been updated.>>\n\n";
				}
				else {
					std::cout << "\n\t\t\t\t\t<<Status updation has failed.>>\n\n";
				}
				std::cout << "\t\t\t\t"; system("pause");
			}
		}
	}
	else {
		displayRow(' ', 76); std::cout << "<<Customer does not exist.>>\n\n";
		cout << "\t\t\t\t\t"; std::system("pause");
	}
	menuStaff1();
}

void searchAccount()
{
	int choice = NULL; string ic;
	std::system("cls");
	ManageUser mu;
	int success; string value;
	Customer* cus = new Customer();
	displayRow('*', 120); std::cout << "\n";
	displayRow(' ', 65); std::cout << " SB BANK \n";
	displayRow('*', 122); std::cout << "\n\n\n";
	displayRow(' ', 71); std::cout << "**Search Account**\n\n";
	displayRow(' ', 60); std::cout << "Search By :\n";
	displayRow(' ', 56); std::cout << "[1]Name\n";
	displayRow(' ', 66); std::cout << "[2]Account Number\n";
	displayRow(' ', 62); std::cout << "[3]IC Number\n\n";
	displayRow(' ', 59); std::cout << "[0]Cancel\n\n";
	displayRow('-', 120); std::cout << "\n";
	displayRow(' ', 62); std::cout << "Enter option: ";
	if (!(std::cin >> choice)) { exceptionNotNumber(); searchAccount(); }
	if (choice == 1 || choice == 2 || choice == 3) {
		displayRow(' ', 61); std::cout << "Enter value :"; getline(cin >> ws, value);
	}
	else if (choice == 0) {
		menuStaff1();
	}
	else {
		cout << "\n\t\t\t\t\t<<The number not in the options.>>\n";
		cout << "\t\t\t\t\t"; system("pause");
		searchAccount();
	}
CustomerDetails:
	cus = mu.searchCustomers(choice, value);
	if (cus != NULL)
	{		
		system("cls");
		displayRow('*', 120); std::cout << "\n";
		displayRow(' ', 65); std::cout << " SB BANK \n";
		displayRow('*', 122); std::cout << "\n\n\n";
		displayRow(' ', 71); std::cout << "**Customer Details**\n\n";
		do{
			displayRow(' ', 65); std::cout << "Identification Card: " << cus->Ic << endl;
			displayRow(' ', 50); std::cout << "Name: " << cus->Name << endl;
			displayRow(' ', 60); std::cout << "Account Number: " << cus->AccountNo << endl;
			displayRow(' ', 53); std::cout << "Address: " << cus->Address << endl;
			displayRow(' ', 54); std::cout << "PIN Code: ";
			for (int i = 0; i < cus->PIN.size(); i++) cout << "*"; cout << endl;
			displayRow(' ', 52); std::cout << "Status: " << cus->Status << endl;
			displayRow(' ', 65); std::cout << "Current Amount (RM): " << fixed << setprecision(2) << cus->CurAmount << endl << endl;
			cus = cus->next;
		} while (cus != NULL);

	}
	else {
		std::cout << "\n\t\t\t\t\t\t<<No customer found>>\n";
	}
	int action = NULL;
	displayRow('-', 120); std::cout << "\n";
	displayRow(' ', 75); std::cout << "[1]Search Again, [0]Back Menu :";
	if (!(std::cin >> action)) {
		exceptionNotNumber();
		goto CustomerDetails;
	};
	if (action != 0 && action != 1) {
		cout << "\n";
		displayRow(' ', 76); std::cout << "<<The number does not in options.>>" << endl;
		cout << "\t\t\t\t\t"; std::system("pause");
		goto CustomerDetails;
	}
	if (action == 1)
		searchAccount();
	else
		menuStaff1();
}

void menuStaff1()
{
	std::system("cls");
	int choice;
	try {
		displayRow('*', 120); std::cout << "\n";
		displayRow(' ', 65); std::cout << " SB BANK \n";
		displayRow('*', 122); std::cout << "\n\n\n";
		displayRow(' ', 71); std::cout << "**Manage Account**\n\n";
		displayRow(' ', 68); std::cout << "1. Register Account\n";
		displayRow(' ', 66); std::cout << "2. Update Account\n";
		displayRow(' ', 79); std::cout << "3. Activate/Deactivate Account\n";
		displayRow(' ', 67); std::cout << "4. Search Account\n\n";
		displayRow(' ', 62); std::cout << "0. Back Menu\n\n";
		displayRow('-', 120); std::cout << "\n";
		displayRow(' ', 62); std::cout << "Enter option: "; 
		if (!(std::cin >> choice))throw 1;

		switch (choice)
		{
		case 1:
			registerAccount();
			break;
		case 2:
			updateAccount();
			break;
		case 3:
			deactiveAccount();
			break;
		case 4:
			searchAccount();
			break;
		case 0:
			mainMenuStaff();
			break;
		default:
			throw 2;
			break;
		}
	}
	catch (int err) {
		switch (err) {
		case 1:
			exceptionNotNumber();
			menuStaff1();
			break;
		case 2:
			std::cout << "\n\t\t\t\t\t<<The number does not in options.>>" << endl;
			cout << "\t\t\t\t\t"; system("pause");
			menuStaff1();
			break;
		}
	}
}

//-------------------- Date Validation Function ---------------------

bool isValidDate(int d, int m, int y)
{
	bool status = true;
	if ((m == 1 || m == 3 || m == 5 || m == 7 || m == 8 || m == 10 || m == 12) && (d > 31 || d < 1))
		status = false;
	else if ((m == 4 || m == 6 || m == 9 || m == 11) && (d > 30 || d < 1))
		status = false;
	else if ((m == 2) && (y % 4 == 0) && (d > 29 || d < 1))
		status = false;
	else if ((m == 2) && (y % 4 != 0) && (d > 28 || d < 1))
		status = false;
	else if ((y < 999) || (y > 10000))
		status = false;
	if ((m < 1) || (m > 12))
		status = false;
	return status;
}

bool isValidRange(int sd, int sm, int sy, int ed, int em, int ey)
{
	bool status = true;
	if (ey < sy)
		status = false;
	if ((ey == sy) && (em < sm))
		status = false;
	if ((ey == sy) && (em == sm) && (ed < sd))
		status = false;
	return status;
}

std::string checkMonthDay(int d, int m) {
	string str = "-";
	if (to_string(m).length() != 2)
		str = str + "0" + to_string(m) + "-";
	else
		str = str + to_string(m) + "-";
	if (to_string(d).length() != 2)
		str = str + "0" + to_string(d);
	else
		str = str + to_string(d);
	return str;
}


//-------------------- Staff2 menu functions ------------------------
void restoreTrans()
{
	ManageTransaction mt; int choice = NULL;
	std::system("cls");
	displayRow('*', 120); std::cout << "\n";
	displayRow(' ', 65); std::cout << " SB BANK \n";
	displayRow('*', 122); std::cout << "\n\n\n";
	displayRow(' ', 80); std::cout << "**Restore Transaction List**\n\n";
	Transaction* trans = mt.getRestore();
	if (trans != NULL)
	{
		Transaction* t = trans; int i = 1;
		displayRow('*', 120); std::cout << "\n";
		std::cout << "| No"; displayRow(' ', 7);
		std::cout << "| Type"; displayRow(' ', 22);
		std::cout << "| Transaction Id"; displayRow(' ', 17);
		std::cout << "| Account Number"; displayRow(' ', 10);
		std::cout << "| Receiver"; displayRow(' ', 17);
		std::cout << "| Amount(RM)"; displayRow(' ', 10);
		std::cout << "| Description"; displayRow(' ', 10);
		std::cout << "| Date"; displayRow(' ', 17);
		std::cout << "| Delete On"; displayRow(' ', 3);
		std::cout << "|\n";
		displayRow('-', 120); std::cout << "\n";
		do
		{
			std::cout << "| "; displayRow(' ', 3); std::cout << i;
			std::cout << "| "; displayRow(' ', 10); std::cout << t->TransType;
			std::cout << "| "; displayRow(' ', 15); std::cout << t->Id;
			std::cout << "| "; displayRow(' ', 14); std::cout << t->TransAcc;
			std::cout << "| "; displayRow(' ', 13); std::cout << t->ReceiveAcc;
			std::cout << "| "; displayRow(' ', 10); std::cout << fixed << setprecision(2) << t->Amount;
			std::cout << "| "; displayRow(' ', 15); std::cout << t->Desc;
			std::cout << "| "; displayRow(' ', 10); std::cout << t->Date.substr(0,10);
			std::cout << "| "; displayRow(' ', 10); std::cout << t->DelDate.substr(0, 10);
			std::cout << "|\n";
			std::cout << "| "; displayRow(' ', 5);
			std::cout << "| "; displayRow(' ', 12);
			std::cout << "| "; displayRow(' ', 17); 
			std::cout << "| "; displayRow(' ', 16); 
			std::cout << "| "; displayRow(' ', 15);
			std::cout << "| "; displayRow(' ', 12);
			std::cout << "| "; displayRow(' ', 17);
			std::cout << "| "; displayRow(' ', 10); std::cout << t->Date.substr(11, 8);
			std::cout << "| "; displayRow(' ', 10); std::cout << t->DelDate.substr(11, 8);
			std::cout << "|\n";
			t = t->next; i++;
		} while (t != NULL);
		std::cout << setfill('*') << setw(122) << "\n\n\n";
		displayRow(' ', 62); std::cout << "[1] Restore\n";
		displayRow(' ', 62); std::cout << "[2] Delete\n\n";
		displayRow(' ', 65); std::cout << "[0] Back Menu\n\n";
		displayRow('-', 121); std::cout << "\n"; 
		displayRow(' ', 58); std::cout << "Option:";
		if (!(cin >> choice)) {
			exceptionNotNumber();
			restoreTrans();
		}
		string transId;
		switch (choice)
		{
		case 1:
			displayRow(' ', 66); std::cout << "Transaction Id: ";
			getline(cin >> std::ws, transId);
			mt.restoreTransaction(transId);
			break;
		case 2:
			displayRow(' ', 66); std::cout << "Transaction Id: ";
			getline(cin >> ws, transId);
			mt.deleteRestore(transId);
			break;
		case 0:
			break;
		default:
			displayRow(' ', 72); std::cout << "<<The number not in options.>>\n";
			cout << "\t\t\t\t\t"; system("pause");
			restoreTrans();
			break;
		}
	}
	else {
		displayRow(' ', 72); std::cout << "<<No transaction found.>>" << std::endl;
		cout << "\t\t\t\t\t"; system("pause");
	}
	listTransaction();
}

void deleteTransaction()
{
	std::string transID; int choice = NULL;
	ManageTransaction mt;
	displayRow(' ', 67); std::cout << "Enter Transaction ID: "; getline(cin >>ws, transID);
	displayRow(' ', 85); 
	while (std::cout << "Do you confirm to delete? Yes[1]/No[0]: " && !(cin >> choice)) {
		exceptionNotNumber();
		displayRow(' ', 85);
	}
	switch (choice)
	{
	case 1:
		mt.deleteTransaction(transID);
		break;
	case 0:
		listTransaction();
		break;
	default:
		displayRow(' ', 65); std::cout << "<<The number does not in options.>>";
		break;
	}
	
}

void listTransaction()
{
	ManageTransaction mt;
	int choice = NULL, order = NULL;
	std::system("cls");
	displayRow('*', 120); std::cout << "\n";
	displayRow(' ', 65); std::cout << " SB BANK \n";
	displayRow('*', 122); std::cout << "\n\n\n";
	displayRow(' ', 70); std::cout << "**Transaction List**\n\n";
	displayRow(' ', 66); std::cout << "List Transaction :\n\n";
	displayRow(' ', 54); std::cout << "[1] All\n";
	displayRow(' ', 59); std::cout << "[2] Withdraw\n";
	displayRow(' ', 58); std::cout << "[3] Deposit\n";
	displayRow(' ', 59); std::cout << "[4] Transfer\n";
	displayRow(' ', 59); std::cout << "[5] Restore\n\n";
	displayRow(' ', 58); std::cout << "[0] Cancel\n\n";
	displayRow('-', 120); std::cout << "\n";
	displayRow(' ', 60); std::cout << "Enter option: ";
	if (!(std::cin >> choice)) {
		exceptionNotNumber();
		listTransaction();
	}
	if (choice == 1 || choice == 2 || choice == 3 || choice == 4) {
		order = 1;
	RangeDate:
		int sday, smonth, syear, eday, emonth, eyear; char slash;
		std::system("cls");
		displayRow('*', 120); std::cout << "\n";
		displayRow(' ', 65); std::cout << " SB BANK \n";
		displayRow('*', 122); std::cout << "\n\n\n";
		displayRow(' ', 78); std::cout << "Enter range of date (dd/mm/yyyy): \n";
		displayRow(' ', 55); std::cout << "Start Date: "; std::cin >> sday >> slash >> smonth >> slash >> syear;
		bool valid = false;
		do
		{
			if (std::cin.good()) {
				valid = true;
			}
			else {
				std::cin.clear();
				std::cin.ignore(INT_MAX, '\n');
				displayRow(' ', 74); std::cout << "<<Start date is invalid.>>\n";
				valid = false;
				displayRow(' ', 55); std::cout << "Start Date: "; std::cin >> sday >> slash >> smonth >> slash >> syear;
			}
		} while (valid == false);
		if (!isValidDate(sday, smonth, syear)) {
			std::cout << "\n\n";
			displayRow(' ', 74); std::cout << "<<Start date is invalid.>>\n";
			cout << "\t\t\t\t\t"; system("pause");
			goto RangeDate;
		}
		displayRow(' ', 53); std::cout << "End Date: "; std::cin >> eday >> slash >> emonth >> slash >> eyear;
		valid = false;
		do
		{
			if (std::cin.good()) {
				valid = true;
			}
			else {
				std::cin.clear();
				std::cin.ignore(INT_MAX, '\n');
				displayRow(' ', 74); std::cout << "<<End date is invalid.>>\n";
				valid = false;
				displayRow(' ', 53); std::cout << "End Date: "; std::cin >> eday >> slash >> emonth >> slash >> eyear;
			}
		} while (valid == false);
		if (!isValidDate(eday, emonth, eyear)) {
			std::cout << "\n\n";
			displayRow(' ', 74); std::cout << "<<End date is invalid.>>\n";
			cout << "\t\t\t\t\t"; system("pause");
			goto RangeDate;
		}
		if (!isValidRange(sday, smonth, syear, eday, emonth, eyear)) {
			std::cout << "\n\n";
			displayRow(' ', 76); std::cout << "<<Range of date is invalid.>>\n";
			cout << "\t\t\t\t\t"; system("pause");
			goto RangeDate;
		}
		std::string start = std::to_string(syear) + checkMonthDay(sday, smonth);
		std::string end = std::to_string(eyear) + checkMonthDay(eday, emonth);
	DisplayTable:
		std::system("cls");
		displayRow('*', 120); std::cout << "\n";
		displayRow(' ', 65); std::cout << " SB BANK \n";
		displayRow('*', 122); std::cout << "\n\n\n";
		if (choice == 1) {
			displayRow(' ', 72); std::cout << "**Transaction List**\n\n";
		}
		else if (choice == 2) {
			displayRow(' ', 70); std::cout << "**Withdraw List**\n\n";
		}
		else if (choice == 3) {
			displayRow(' ', 70); std::cout << "**Deposit List**\n\n";
		}
		else {
			displayRow(' ', 70); std::cout << "**Transfer List**\n\n";
		}
		displayRow(' ', 45); std::cout << "FROM: " << start << "\t\tUNTIL: " << end << "\n\n\n";
		Transaction* trans = mt.sortTransaction(choice, order, start, end);
		if (trans != NULL)
		{
			Transaction* t = trans; int i = 1;
			trans->totalAmount = 0;
			displayRow('*', 120); std::cout << "\n";
			std::cout << "| No"; displayRow(' ', 7);
			std::cout << "| Type"; displayRow(' ', 22);
			std::cout << "| Transaction Id"; displayRow(' ', 18);
			std::cout << "| Account Number"; displayRow(' ', 10);
			std::cout << "| Receiver"; displayRow(' ', 18);
			std::cout << "| Amount(RM)"; displayRow(' ', 10);
			std::cout << "| Description"; displayRow(' ', 11);
			std::cout << "| Date"; displayRow(' ', 17);
			std::cout << "|\n";
			displayRow('*', 120); std::cout << "\n";
			do
			{
				std::cout << "| "; displayRow(' ', 3); std::cout << i;
				std::cout << "| "; displayRow(' ', 10); std::cout << t->TransType;
				std::cout << "| "; displayRow(' ', 16); std::cout << t->Id;
				std::cout << "| "; displayRow(' ', 14); std::cout << t->TransAcc;
				std::cout << "| "; displayRow(' ', 14); std::cout << t->ReceiveAcc;
				std::cout << "| "; displayRow(' ', 10); std::cout << fixed << setprecision(2) << t->Amount;
				std::cout << "| "; displayRow(' ', 16); std::cout << t->Desc;
				std::cout << "| "; displayRow(' ', 19); std::cout << t->Date;
				std::cout << "|\n";
				trans->totalAmount += t->Amount;
				t = t->next; i++;
			} while (t != NULL);
			std::cout << "\n";
			displayRow('*', 121); std::cout << "\n\n";
			displayRow(' ', 120); std::cout << right << "Total Amount of Transactions (RM)\n";
			displayRow(' ', 110); std::cout << right << "= " << trans->totalAmount << "\n\n\n";
		OrderOption:
			int asc = NULL;
			displayRow(' ', 61); std::cout << "Continue with: \n";
			if (order == 1) {
				displayRow(' ', 65); std::cout << "[1] Sort Descending\n";
			}
			else if (order == 2) {
				displayRow(' ', 64); std::cout << "[1] Sort Ascending\n";
			}
			displayRow(' ', 69); std::cout << "[2] Delete Transaction\n\n";
			displayRow(' ', 55); std::cout << "[0] Back\n\n";
			displayRow('-', 120); std::cout << "\n";
			displayRow(' ', 53); std::cout << "Option: ";;
			if (!(cin >> asc)) {
				exceptionNotNumber();
				goto DisplayTable;
			}
			switch (asc) {
			case 1:
				if (order == 1)
					order = 2;
				else
					order = 1;
				goto DisplayTable;
				break;
			case 2:
				deleteTransaction();
				goto DisplayTable;
				break;
			case 0:
				listTransaction();
				break;
			default:
				std::cout << "\n\t\t\t\t\t<<The number does not in options.>>\n";
				cout << "\t\t\t\t\t"; std::system("pause");
				goto DisplayTable;
				break;
			}
			
		}else {
			displayRow(' ', 75); std::cout << "<<No transaction found.>>" << std::endl;
		}
		cout << "\t\t\t\t\t"; std::system("pause");
		menuStaff2();
		}
	else if (choice == 0)
		menuStaff2();
	else if (choice == 5) {
		restoreTrans();
	}
	else {
		std::cout << "\n\t\t\t\t\t<<The number does not in options.>>\n";
		cout << "\t\t\t\t\t"; std::system("pause");
		listTransaction();
	}
}

void listCustomer()
{
	ManageUser mu;
	int choice = NULL, order = NULL;
	Customer* customers;
	std::system("cls");
	displayRow('*', 120); std::cout << "\n";
	displayRow(' ', 65); std::cout << " SB BANK \n";
	displayRow('*', 122); std::cout << "\n\n\n";
	displayRow(' ', 69); std::cout << "**Customer List**\n\n";
	displayRow(' ', 58); std::cout << "Sort By :\n\n";
	displayRow(' ', 56); std::cout << "[1] Name\n";
	displayRow(' ', 66); std::cout << "[2] Account Number\n";
	displayRow(' ', 61); std::cout << "[3] IC Number\n";
	displayRow(' ', 59); std::cout << "[4] Status\n\n";
	displayRow(' ', 59); std::cout << "[0] Cancel\n\n";
	displayRow('-', 120); std::cout << "\n";
SortOption:
	displayRow(' ', 61); std::cout << "Enter option: ";
	if (!(cin >> choice)) {
		exceptionNotNumber();
		listCustomer();
	}
	if (choice == 1 || choice == 2 || choice == 3 || choice == 4) {
		order = 1;
	DisplayTable:
		std::system("cls");
		displayRow('*', 120); std::cout << "\n";
		displayRow(' ', 65); std::cout << " SB BANK \n";
		displayRow('*', 122); std::cout << "\n\n\n";
		displayRow(' ', 69); std::cout << "**Customer List**\n\n";
		customers = mu.sortCustomers(choice, order);
		if (customers != NULL)
		{
			Customer* cus = customers;
			displayRow('*', 120); std::cout << "\n";
			std::cout << "| Name"; displayRow(' ', 50);
			std::cout << "| IC Number"; displayRow(' ', 30);
			std::cout << "| Account Number"; displayRow(' ', 17);
			std::cout << "| Status"; displayRow(' ', 16);
			std::cout << "|\n";
			displayRow('*', 120); std::cout << "\n";
			do
			{
				std::cout << "| "; displayRow(' ', 43); std::cout << cus->Name;
				std::cout << "| "; displayRow(' ', 23); std::cout << cus->Ic;
				std::cout << "| "; displayRow(' ', 23); std::cout << cus->AccountNo;
				std::cout << "| "; displayRow(' ', 20); std::cout << cus->Status;
				std::cout << "|\n";
				cus = cus->next;
			} while (cus != NULL);
			std::cout << setfill('*') << setw(119) << "*" << endl;
		}
		else {
			displayRow(' ', 75); std::cout << "<<No customer found.>>" << std::endl;
		}
	OrderOption:
		int asc = NULL;
		if (order == 1) {
			displayRow(' ', 75); std::cout << "Change order to [1]Descending, [0]Back :";
		}
		else if (order == 2) {
			displayRow(' ', 75); std::cout << "Change order to [1]Ascending, [0]Back :";
		}
		if (!(cin >> asc)) {
			exceptionNotNumber();
			goto DisplayTable;
		}
		if (asc == 1) {
			if (order == 1)
				order = 2;
			else
				order = 1;
			goto DisplayTable;
		}
		else if (asc == 0) {
			listCustomer();
		}
		else {
			std::cout << "\n\t\t\t\t\t<<The number does not in options.>>\n\n";
			std::cout << "\t\t\t\t"; system("pause");
			goto DisplayTable;
		}
	}
	else if (choice == 0) {
		menuStaff2();
	}
	else {
		std::cout << "\n\t\t\t\t\t<<The number does not in options.>>\n\n";
		std::cout << "\t\t\t\t"; system("pause");
		listCustomer();
	}
}

void summary(){
	std::system("cls");
	ManageTransaction mt;
	displayRow('*', 120); std::cout << "\n";
	displayRow(' ', 65); std::cout << " SB BANK \n";
	displayRow('*', 122); std::cout << "\n\n\n";
	displayRow(' ', 78); std::cout << "**Summary of Transaction Trend**\n\n";
	string month[12] = {}, highMonth = ""; int count[12] = {}, highest = 1;
	string mName [] = {"January","February","March","April", "May", "June", "July", "August", "September", "October", "November","December"};
	displayRow('-', 158);
	cout << "\n\t\t\t\t Month \t\t\t Number of Transaction\n\n";
	mt.generateSummary(month, count);
	int x = 0;
	for (int i = 0; i < 12; i++) {
		cout << "\t\t\t\t";  displayRow(' ', 12); cout<<left<<mName[i];
		if (mName[i] == month[x]) {
			cout << " (" << count[x] << ") : "; 
		}
		else {
			cout << " (" << 0 << ") : ";
		}
		if (count[x] > highest) {
			highest = count[x];
			highMonth = month[x];
		}
		if (mName[i]==month[x]) {
			for (int j = 0; j < count[x]; j++)
				 cout << "*";
			
			x++;
		}
		cout << endl;
	}
	displayRow('-', 153);
	cout <<internal<<"\n\n\t\t\t\t<< Most transaction made on " << highMonth << " with " << highest << " transactions.>>\n\n";
	cout << "\t\t\t\t"; system("pause");
	menuStaff2();
}

void menuStaff2()
{
	std::system("cls");
	int choice;
	try {
		displayRow('*', 120); std::cout << "\n";
		displayRow(' ', 65); std::cout << " SB BANK \n";
		displayRow('*', 121); std::cout << "\n\n\n";
		displayRow(' ', 71); std::cout << "**Generate Report**\n\n";
		displayRow(' ', 65); std::cout << "1. Customer List\n";
		displayRow(' ', 68); std::cout << "2. Transaction List\n";
		displayRow(' ', 60); std::cout << "3. Summary\n\n";
		std::cout << "\n";
		displayRow(' ', 62); std::cout << "0. Back Menu\n\n";
		displayRow('-', 120); std::cout << "\n";
		displayRow(' ', 62); std::cout << "Enter option: "; 
		if (!(std::cin >> choice))throw 1;

		switch (choice)
		{
		case 1:
			listCustomer();
			break;
		case 2:
			listTransaction();
			break;
		case 3:
			summary();
			break;
		case 0:
			mainMenuStaff();
			break;
		default:
			throw 2;
			break;
		}
	}
	catch (int err) {
		switch (err) {
		case 1:
			exceptionNotNumber();
			menuStaff2();
			break;
		case 2:
			std::cout << "\n\t\t\t\t\t<<The number does not in options.>>" << endl;
			cout << "\t\t\t\t\t"; system("pause");
			menuStaff2();
			break;
		}
	}
}


//-------------------- Main Menu User -------------------------------
void mainMenuCustomer()
{
	std::system("cls");
	int choice;
	try {
		displayRow('*', 120); std::cout << "\n";
		displayRow(' ', 65); std::cout << " SB BANK "; std::cout << "\n";
		displayRow('*', 122); std::cout << "\n\n\n";
		displayRow(' ', 68); std::cout << "**Customer Menu**"; std::cout << "\n\n";
		displayRow(' ', 69); std::cout << "1. Money Transaction"; std::cout << "\n";
		displayRow(' ', 65); std::cout << "2. Funds Summary"; std::cout << "\n\n";
		displayRow(' ', 58); std::cout << "0. Logout"; std::cout << "\n\n";
		displayRow('-', 120); std::cout << "\n";
		displayRow(' ', 63); std::cout << "Enter option: ";
		if (!(std::cin >> choice)) throw 1;

		switch (choice)
		{
		case 1:
			menuCustomer1();
			break;
		case 2:
			menuCustomer2();
			break;
		case 0:
			main();
			break;
		default:
			throw 2;
			break;
		}
	}catch (int err) {
		switch (err) {
		case 1:
			exceptionNotNumber();
			mainMenuCustomer();
			break;
		case 2:
			std::cout << "\n\t\t\t\t\t<<The number does not in options.>>" << endl;
			cout << "\t\t\t\t\t"; system("pause");
			mainMenuCustomer();
			break;
		}
	}
}

void mainMenuStaff()
{
	try {
		int choice;
		std::system("cls");
		displayRow('*', 120); std::cout << "\n";
		displayRow(' ', 65); std::cout << " SB BANK \n";
		displayRow('*', 122); std::cout << "\n\n\n";
		displayRow(' ', 68); std::cout << "**Staff Menu**\n\n";
		displayRow(' ', 67); std::cout << "1. Manage Account\n";
		displayRow(' ', 69); std::cout << "2. Overview Report\n\n";
		displayRow(' ', 60); std::cout << "0. Logout\n\n";
		displayRow('-', 120); std::cout << "\n";
		displayRow(' ', 63); std::cout << "Enter option: "; 
		if (!(std::cin >> choice)) throw 1;
		switch (choice)
		{
		case 1:
			menuStaff1();
			break;
		case 2:
			menuStaff2();
			break;
		case 0:
			main();
			break;
		default:
			throw 2;			
			break;
		}
	}
	catch (int err) {
		switch (err) {
		case 1:
			exceptionNotNumber();
			mainMenuStaff();
			break;
		case 2:
			std::cout << "\n\t\t\t\t\t<<The number does not in options.>>" << endl;
			cout << "\t\t\t\t\t"; system("pause");
			mainMenuStaff();
			break;
		}
	}
}

