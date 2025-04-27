#include "ManageUser.h"

//check account and password for login validation
std::string LoginRegister::login(std::string account, int pass)
{
	std::string user = "";

	//to declare object dbConnect
	DbConnect conn;

	//to create a sql statement to be run
	sql::PreparedStatement* ps = conn.prepareStatement("SELECT * FROM Admin WHERE Admin_ID = ? AND Admin_Pass = ?");

	//to convert string to upper case
	transform(account.begin(), account.end(), account.begin(), ::toupper);

	//to set the ? with the actual values
	ps->setString(1, account);
	ps->setInt(2, pass);

	//to run the sql statement, and store in return table of format (ResultSet)
	sql::ResultSet* rs = ps->executeQuery();

	//to check if the sql statement success or have result
	if (rs->next())
	{
		//to get the value from the result
		user = "admin";
	}
	else
	{
		user = "adminfalse";
		std::string pin = std::to_string(pass);
		sql::PreparedStatement* ps = conn.prepareStatement("SELECT * FROM Customer WHERE CusAccountNo = ? AND CusPIN = ?");
		transform(account.begin(), account.end(), account.begin(), ::toupper);
		ps->setString(1, account);
		ps->setString(2, pin);
		sql::ResultSet* rs = ps->executeQuery();
		if (rs->next()) {
			if (rs->getString("CusStatus") == "Active") {
				user = "customer";
			}
			else {
				std::cout << "\n\n";
				std::cout << std::setfill(' ') << std::setw(92);
				std::cout << "Your account is inactive. Please contact to bank officer.\n";
				user = "inactive";
			}
		}
		else {
			std::cout << "\n\n";
			std::cout << "\t\t\t\t\tSorry, wrong account or PIN number. Try again." << std::endl;
			user = "";
		}
	}

	//to close the result, statement and connection to avoid limitation issues
	//close following the order of the object created
	rs->close();
	ps->close();

	//to delete object from the memory
	delete rs;
	delete ps;

	return user;
}

//check if there existing account no
bool LoginRegister::isExist(long long int acc)
{
	bool exist = false;
	DbConnect conn;
	sql::PreparedStatement* ps = conn.prepareStatement("SELECT * from Customer WHERE CusAccountNo = ?");
	ps->setString(1, std::to_string(acc));
	sql::ResultSet* rs = ps->executeQuery();
	if (rs->next()) {
		exist = true;
	}
	return exist;
}

//check if there existing cusIC
bool LoginRegister::isExistCus(std::string ic)
{
	bool exist = false;
	DbConnect conn;
	sql::PreparedStatement* ps = conn.prepareStatement("SELECT * from Customer WHERE CusIC = ?");
	ps->setString(1, ic);
	sql::ResultSet* rs = ps->executeQuery();
	if (rs->next()) {
		exist = true;
	}
	return exist;
}

//generate account no
long long int LoginRegister::generateAccNo()
{
	int num[12] = {};
	srand(time(0));
	for (int i = 0; i < 12; i++) {
		num[i] = rand() % 11;
	}
	long long int accNo = 0;
	for (int i = 0; i < 12; i++) {
		accNo *= 10;
		accNo += num[i];
	}	
	return accNo;
}

//insert new customer
int LoginRegister::registerAcc(std::string name, std::string address, std::string ic, int pin, double curAmount)
{
	DbConnect conn;
	long long int accno;
	do {
		accno = generateAccNo();
	} while (isExist(accno));

	if (isExistCus(ic)) {
		return 0;
	}
	else {
		std::string pinno = std::to_string(pin);
		sql::PreparedStatement* ps = conn.prepareStatement("INSERT INTO Customer(CusIC,CusName,CusAccountNo,CusAddress,CusPIN,CusCurrentAmount) VALUES (?,?,?,?,?,?)");

		ps->setString(1, ic);
		ps->setString(2, name);
		ps->setString(3, std::to_string(accno));
		ps->setString(4, address);
		ps->setString(5, pinno);
		ps->setDouble(6, curAmount);

		int newRow = ps->executeUpdate();
		ps->close();
		delete ps;

		return newRow;
	}
}




//transform from result set to string (CUSTOMER)
void ManageUser::transform(sql::ResultSet* rs, Customer* cus)
{
	cus->Ic = rs->getString("CusIC");
	cus->Name = rs->getString("CusName");
	cus->AccountNo = rs->getString("CusAccountNo");
	cus->Address = rs->getString("CusAddress");
	cus->PIN = rs->getString("CusPIN");
	cus->CurAmount = rs->getDouble("CusCurrentAmount");
	cus->Status = rs->getString("CusStatus");
}

//update customer account detail (INT value)
int ManageUser::updateAcc(int option, std::string ic, int value)
{
	DbConnect conn;
	std::string pin = std::to_string(value);
	sql::PreparedStatement * ps = conn.prepareStatement("UPDATE Customer SET CusPIN = ? WHERE CusIC = ?");
	ps->setString(1, pin);
	ps->setString(2, ic);

	int numRowsAffected = ps->executeUpdate();

	ps->close();

	delete ps;

	return numRowsAffected;
}

//update customer account detail (STRING value)
int ManageUser::updateAcc(int option, std::string ic, std::string value)
{
	DbConnect conn;
	int numRowsAffected;
	sql::PreparedStatement* ps;
	switch (option)
	{
	case 1:
		ps = conn.prepareStatement("UPDATE Customer SET CusName = ? WHERE CusIC = ?");
		break;
	case 2:
		ps = conn.prepareStatement("UPDATE Customer SET CusAddress = ? WHERE CusIC = ?");
		break;
	default:
		return 0;
		break;
	}
	ps->setString(1, value);
	ps->setString(2, ic);
	numRowsAffected = ps->executeUpdate();
	ps->close();
	delete ps;
	return numRowsAffected;
}

//update customer account STATUS
int ManageUser::updateAcc(std::string ic, int status)
{
	DbConnect conn;
	sql::PreparedStatement* ps;
	ps = conn.prepareStatement("UPDATE Customer SET CusStatus = ? WHERE CusIC = ?");
	if(status==1)
		ps->setString(1, "Active");
	else
		ps->setString(1, "Deactive");
	ps->setString(2, ic);
	int numRowsAffected = ps->executeUpdate();
	ps->close();
	delete ps;
	return numRowsAffected;
}

Customer* ManageUser::searchCustomers()
{
	DbConnect conn;
	sql::PreparedStatement* ps = conn.prepareStatement("SELECT * FROM Customer");
	sql::ResultSet* rs = ps->executeQuery();
	Customer* cus = NULL;
	Customer* head = NULL;

	while (rs->next())
	{
		if (cus == NULL)
		{
			cus = new Customer();
			head = cus;
		}
		else
		{
			cus->next = new Customer();
			cus = cus->next;
		}

		transform(rs, cus);
	}

	rs->close();
	ps->close();

	delete rs;
	delete ps;

	return head;
}

//search for customer account with IC
Customer* ManageUser::searchCustomer(std::string ic)
{
	DbConnect conn;
	sql::PreparedStatement* ps = conn.prepareStatement("SELECT * FROM Customer WHERE CusIC = ?");

	ps->setString(1, ic);

	sql::ResultSet* rs = ps->executeQuery();
	Customer* cus = NULL;

	if (rs->next())
	{
		cus = new Customer();
		transform(rs, cus);
	}

	rs->close();
	ps->close();

	delete rs;
	delete ps;

	return cus;
}

//search for customers account with name/accno/ic
Customer* ManageUser::searchCustomers(int choice, std::string val)
{
	DbConnect conn;
	sql::PreparedStatement* ps = NULL;
	switch (choice)
	{
	case 1:
		ps = conn.prepareStatement("SELECT * FROM Customer WHERE UPPER(CusName) LIKE UPPER(?)");
		break;
	case 2:
		ps = conn.prepareStatement("SELECT * FROM Customer WHERE CusAccountNo LIKE ?");
		break;
	case 3:
		ps = conn.prepareStatement("SELECT * FROM Customer WHERE CusIC LIKE ?");
		break;
	default:
		break;
	}
	ps->setString(1, "%"+val+"%");
	sql::ResultSet* rs = ps->executeQuery();
	Customer* cus = NULL;
	Customer* head = NULL;

	while (rs->next())
	{
		if (cus == NULL)
		{
			cus = new Customer();
			head = cus;
		}
		else
		{
			cus->next = new Customer();
			cus = cus->next;
		}

		transform(rs, cus);
	}

	rs->close();
	ps->close();

	delete rs;
	delete ps;

	return head;
}

//sort customer list
Customer* ManageUser::sortCustomers(int choice, int order)
{
	DbConnect conn;
	sql::PreparedStatement* ps = NULL;
	switch (order) {
	case 1:
		switch (choice)
		{
		case 1:
			ps = conn.prepareStatement("SELECT * FROM Customer ORDER BY CusName");
			break;
		case 2:
			ps = conn.prepareStatement("SELECT * FROM Customer ORDER BY CusAccountNo");
			break;
		case 3:
			ps = conn.prepareStatement("SELECT * FROM Customer ORDER BY CusIC");
			break;
		case 4:
			ps = conn.prepareStatement("SELECT * FROM Customer ORDER BY CusStatus");
			break;
		}
		break;
	case 2:
		switch (choice)
		{
		case 1:
			ps = conn.prepareStatement("SELECT * FROM Customer ORDER BY CusName DESC");
			break;
		case 2:
			ps = conn.prepareStatement("SELECT * FROM Customer ORDER BY CusAccountNo DESC");
			break;
		case 3:
			ps = conn.prepareStatement("SELECT * FROM Customer ORDER BY CusIC DESC");
			break;
		case 4:
			ps = conn.prepareStatement("SELECT * FROM Customer ORDER BY CusStatus DESC");
			break;
		}
		break;
	}
	sql::ResultSet* rs = ps->executeQuery();
	Customer* cus = NULL;
	Customer* head = NULL;

	while (rs->next())
	{
		if (cus == NULL)
		{
			cus = new Customer();
			head = cus;
		}
		else
		{
			cus->next = new Customer();
			cus = cus->next;
		}

		transform(rs, cus);
	}

	rs->close();
	ps->close();

	delete rs;
	delete ps;

	return head;
}