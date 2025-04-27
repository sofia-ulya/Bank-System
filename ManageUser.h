#include "DbConnect.h"
#include "Customer.h"

#ifndef LOGINREGISTER_H
#define LOGINREGISTER_H

class LoginRegister
{
public:
	std::string login(std::string, int);
	int registerAcc(std::string, std::string, std::string, int, double);
	long long int generateAccNo();
	bool isExistCus(std::string);
	bool isExist(long long int);
};

class ManageUser
{
public:
	void transform(sql::ResultSet* rs, Customer* cus);
	int updateAcc(int, std::string, int);
	int updateAcc(int, std::string, std::string);
	int updateAcc(std::string, int);
	Customer* searchCustomers();
	Customer* searchCustomer(std::string);
	Customer* searchCustomers(int, std::string);
	Customer* sortCustomers(int, int);
};

#endif