#include "Transaction.h"
#include "ManageUser.h"


#ifndef MANAGETRANSACTION_H
#define MANAGETRANSACTION_H
class ManageTransaction
{
public:
	void transform(sql::ResultSet*, Transaction*);
	void transformRestore(sql::ResultSet*, Transaction*);
	Customer* searchCustomer(std::string);
	std::string getIC(std::string);
	bool isExist(std::string);
	std::string generateCusTransID();
	bool updateAcc(std::string, double);
	int transactionAcc(std::string, double, std::string);
	int transactionAcc(std::string, double);
	int transactionAcc(std::string, double, std::string, std::string);
	Transaction* getTransaction(std::string);
	Transaction* getTransaction(std::string,int);
	Transaction* sortTransaction(int, int, std::string, std::string);
	void generateSummary(std::string[], int[]);
	int deleteRestore(std::string);
	int restoreTransaction(std::string);
	int deleteTransaction(std::string);
	Transaction* getRestore();
};
#endif MANAGETRANSACTION_H
