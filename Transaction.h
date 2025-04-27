#include <string>

#ifndef TRANSACTION_H
#define TRANSACTION_H

class Transaction
{
public:
	std::string Id, Desc, CusIC, Date, TransAcc, ReceiveAcc, TransType, DelDate;
	int TypeID;
	double Amount, totalAmount;
	Transaction* next;
};

#endif