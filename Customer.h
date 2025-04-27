#include <string>

#ifndef CUSTOMER_H
#define CUSTOMER_H

class Customer
{
public:
	std::string Ic, Name, AccountNo, Address, Status, PIN;
	double CurAmount = NULL;
	Customer* next = NULL;
};

#endif