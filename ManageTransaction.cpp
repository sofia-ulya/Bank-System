#include "ManageTransaction.h"

//transform Result set to string (transaction)
void ManageTransaction::transform(sql::ResultSet* rs, Transaction* trans)
{
	trans->Id = rs->getString("CusTransID");
	trans->CusIC= rs->getString("CusIC");
	trans->Date = rs->getString("TransDate");
	trans->TransAcc = rs->getString("CusAccountNo");
	trans->ReceiveAcc = rs->getString("TransAccount");
	trans->TransType = rs->getString("TransType");
	trans->Amount = rs->getDouble("TransAmount");
	trans->Desc = rs->getString("TransDesc");
	trans->TypeID = rs->getInt("TransID");
}

//transform Result set to string (restore)
void ManageTransaction::transformRestore(sql::ResultSet* rs, Transaction* trans)
{
	trans->Id = rs->getString("CusTransID");
	trans->CusIC = rs->getString("CusIC");
	trans->Date = rs->getString("TransDate");
	trans->TransAcc = rs->getString("CusAccountNo");
	trans->ReceiveAcc = rs->getString("TransAccount");
	trans->TransType = rs->getString("TransType");
	trans->Amount = rs->getDouble("TransAmount");
	trans->Desc = rs->getString("TransDesc");
	trans->TypeID = rs->getInt("TransID");
	trans->DelDate = rs->getString("DeleteDate");
}



//search customer by account no
Customer* ManageTransaction::searchCustomer(std::string accno) 
{
	DbConnect conn; ManageUser mu;
	sql::PreparedStatement* ps = conn.prepareStatement("SELECT * FROM Customer WHERE CusAccountNo = ?");

	ps->setString(1, accno);

	sql::ResultSet* rs = ps->executeQuery();
	Customer* cus = NULL;

	if (rs->next())
	{
		cus = new Customer();
		mu.transform(rs, cus);
	}

	rs->close();
	ps->close();

	delete rs;
	delete ps;

	return cus;
}

//search customer ic by account no
std::string ManageTransaction::getIC(std::string accno)
{
	DbConnect conn; ManageUser mu; std::string ic;
	sql::PreparedStatement* ps = conn.prepareStatement("SELECT CusIC FROM Customer WHERE CusAccountNo = ?");

	ps->setString(1, accno);

	sql::ResultSet* rs = ps->executeQuery();
	Customer* cus = NULL;

	if (rs->next())
	{
		ic = rs->getString("CusIC");
	}

	rs->close();
	ps->close();

	delete rs;
	delete ps;

	return ic;
}



//check if there existing custTransID
bool ManageTransaction::isExist(std::string id)
{
	bool exist = false;
	DbConnect conn;
	sql::PreparedStatement* ps = conn.prepareStatement("SELECT * from customer_transaction WHERE CusTransID = ?");
	ps->setString(1, id);
	sql::ResultSet* rs = ps->executeQuery();
	if (rs->next()) {
		exist = true;
	}
	return exist;
}

//generate custTransID
std::string ManageTransaction::generateCusTransID()
{
	int num;
	std::string id = "SB";
	srand(time(0));
	for (int i = 0; i < 10; i++) {
		num = rand() % 10;
		id = id + std::to_string(num);
	}
	return id;
}

//update customer current amount
bool ManageTransaction::updateAcc(std::string accno, double amount)
{
	DbConnect conn;
	sql::PreparedStatement* ps;
	ps = conn.prepareStatement("UPDATE Customer SET CusCurrentAmount = ? WHERE CusAccountNo = ?");
	ps->setDouble(1, amount);
	ps->setString(2, accno);
	int numRowsAffected = ps->executeUpdate();
	ps->close();
	delete ps;
	if (numRowsAffected == 0) {
		return false;
	}
	else {
		return true;
	}
}




//deposit transaction
int ManageTransaction::transactionAcc(std::string accno, double amount, std::string desc)
{
	DbConnect conn; Customer* cus;
	int transID = 1;
	std::string cusTransID = "";
	char curDate[40];
	do {
		cusTransID = generateCusTransID();
	}while(isExist(cusTransID));
	conn.getCurrentTime(curDate);
	/*string transDate = "";
	for (int i = 0; i < 29; i++) {
		transDate = transDate + curDate[i];
	}*/
	cus = searchCustomer(accno);
	//cout << transID << cusTransID << curDate << cus->Ic << cus->CurAmount;
	sql::PreparedStatement* ps = conn.prepareStatement("INSERT INTO Customer_Transaction(CusTransID, TransDate, TransAmount, TransDesc, CusIC, TransID) VALUES (?,?,?,?,?,?)");

	ps->setString(1, cusTransID);
	ps->setString(2, curDate);
	ps->setDouble(3, amount);
	ps->setString(4, desc);
	ps->setString(5, cus->Ic);
	ps->setInt(6, transID);

	int newRow = ps->executeUpdate();

	if (newRow == 0) {
		//cout << "insert transaction failed.";
		return 0;
	}
	else {
		cus->CurAmount = cus->CurAmount + amount;		
		if (updateAcc(accno, cus->CurAmount)) {
			//cout << "insert transaction and update amount success.";
			return 1;
		}
		else {
			//cout << "update amount failed.";
			return 0;
		}
	}
	ps->close();
	delete ps;
}

//withdraw transaction
int ManageTransaction::transactionAcc(std::string accno, double amount)
{
	DbConnect conn; Customer* cus;
	int transID = 3;
	std::string cusTransID = "";
	char curDate[40];
	do {
		cusTransID = generateCusTransID();
	} while (isExist(cusTransID));
	conn.getCurrentTime(curDate);
	cus = searchCustomer(accno);
	if (cus->CurAmount > 20 && cus->CurAmount >= amount) {
		sql::PreparedStatement* ps = conn.prepareStatement("INSERT INTO Customer_Transaction(CusTransID, TransDate, TransAmount, CusIC, TransID) VALUES (?,?,?,?,?)");
		ps->setString(1, cusTransID);
		ps->setString(2, curDate);
		ps->setDouble(3, amount);
		ps->setString(4, cus->Ic);
		ps->setInt(5, transID);
		int newRow = ps->executeUpdate();

		if (newRow == 0) {
			//cout << "insert transaction failed.";
			return 0;
		}
		else {
			cus->CurAmount = cus->CurAmount - amount;
			if (updateAcc(accno, cus->CurAmount)) {
				//cout << "insert transaction and update amount success.";
				return 1;
			}
			else {
				//cout << "update amount failed.";
				return 0;
			}
		}
		ps->close();
		delete ps;
	}
	else {
		std::cout << "<<Insufficient funds.>>\n";
		return 0;
	}
	
}

//transfer transaction
int ManageTransaction::transactionAcc(std::string accno, double amount, std::string desc, std::string receiveAcc)
{
	DbConnect conn; Customer* cus;
	int transID = 2;
	std::string cusTransID = "";
	char curDate[40];
	do {
		cusTransID = generateCusTransID();
	} while (isExist(cusTransID));
	conn.getCurrentTime(curDate);
	cus = searchCustomer(accno);
	if (cus->CurAmount > 20 && cus->CurAmount > amount) {
		sql::PreparedStatement* ps = conn.prepareStatement("INSERT INTO Customer_Transaction(CusTransID, TransDate, TransAmount, CusIC, TransID, TransAccount, TransDesc) VALUES (?,?,?,?,?,?,?)");

		ps->setString(1, cusTransID);
		ps->setString(2, curDate);
		ps->setDouble(3, amount);
		ps->setString(4, cus->Ic);
		ps->setInt(5, transID);
		ps->setString(6, receiveAcc);
		ps->setString(7, desc);

		int newRow = ps->executeUpdate();

		if (newRow == 0) {
			//cout << "insert transaction failed.";
			return 0;
		}
		else {
			cus->CurAmount = cus->CurAmount - amount;
			if (updateAcc(accno, cus->CurAmount)) {
				//cout << "insert transaction and update amount success.";
				cus = searchCustomer(receiveAcc);
				cus->CurAmount = cus->CurAmount + amount;
				if (updateAcc(receiveAcc, cus->CurAmount)) {
					//cout << "insert transaction and update amount receiver success.";
					return 1;
				}
			}
			else {
				std::cout << "update amount failed.";
				return 0;
			}
		}
		ps->close();
		delete ps;
	}
	else {
		std::cout << "<<Insufficient funds.>>\n";
		return 0;
	}
}



//get all transaction by CUSTOMER
Transaction* ManageTransaction::getTransaction(std::string accno)
{
	DbConnect conn;
	sql::PreparedStatement* ps = conn.prepareStatement("SELECT t.TransID, TransType, CusTransID, TransAmount, TransDesc, TransAccount, TransDate, ct.CusIC, CusAccountNo FROM Customer_Transaction ct JOIN Transaction t ON t.TransId = ct.TransId JOIN customer c ON ct.CusIC = c.CusIC WHERE CusAccountNo = ? OR TransAccount = ?");
	ps->setString(1, accno);
	ps->setString(2, accno);
	sql::ResultSet* rs = ps->executeQuery();
	Transaction* trans = NULL;
	Transaction* head = NULL;

	while (rs->next())
	{
		if (trans == NULL)
		{
			trans = new Transaction();
			head = trans;
		}
		else
		{
			trans->next = new Transaction();
			trans = trans->next;
		}
		transform(rs, trans);
	}

	rs->close();
	ps->close();

	delete rs;
	delete ps;

	return head;
}

//get all transaction by INTERVAL DATE
Transaction* ManageTransaction::getTransaction(std::string accno, int interval)
{
	DbConnect conn; sql::PreparedStatement* ps = NULL;
	switch (interval)
	{
	case 30:
		ps = conn.prepareStatement("SELECT t.TransID, TransType, CusTransID, TransAmount, TransDesc, TransAccount, TransDate, ct.CusIC, CusAccountNo FROM Customer_Transaction ct JOIN Transaction t ON t.TransId = ct.TransId JOIN customer c ON ct.CusIC = c.CusIC WHERE TransDate > CURDATE() - INTERVAL 30 DAY AND CusAccountNo = ? OR TransAccount = ? ORDER BY TransDate Desc");
		break;
	case 60:
		ps = conn.prepareStatement("SELECT t.TransID, TransType, CusTransID, TransAmount, TransDesc, TransAccount, TransDate, ct.CusIC, CusAccountNo FROM Customer_Transaction ct JOIN Transaction t ON t.TransId = ct.TransId JOIN customer c ON ct.CusIC = c.CusIC WHERE TransDate > CURDATE() - INTERVAL 60 DAY AND CusAccountNo = ? OR TransAccount = ? ORDER BY TransDate Desc");
		break;
	case 90:
		ps = conn.prepareStatement("SELECT t.TransID, TransType, CusTransID, TransAmount, TransDesc, TransAccount, TransDate, ct.CusIC, CusAccountNo FROM Customer_Transaction ct JOIN Transaction t ON t.TransId = ct.TransId JOIN customer c ON ct.CusIC = c.CusIC WHERE TransDate > CURDATE() - INTERVAL 90 DAY AND CusAccountNo = ? OR TransAccount = ? ORDER BY TransDate Desc");
		break;
	}
	
	ps->setString(1, accno);
	ps->setString(2, accno);
	sql::ResultSet* rs = ps->executeQuery();
	Transaction* trans = NULL;
	Transaction* head = NULL;

	while (rs->next())
	{
		if (trans == NULL)
		{
			trans = new Transaction();
			head = trans;
		}
		else
		{
			trans->next = new Transaction();
			trans = trans->next;
		}
		transform(rs, trans);
	}

	rs->close();
	ps->close();

	delete rs;
	delete ps;

	return head;
}




//sort transaction based on trans type
Transaction* ManageTransaction::sortTransaction(int choice, int order, std::string start, std::string end)
{
	DbConnect conn;
	sql::PreparedStatement* ps = NULL;
	switch (order) {
	case 1:
		switch (choice)
		{
		case 1:
			ps = conn.prepareStatement("SELECT t.TransID, TransType, CusTransID, TransAmount, TransDesc, TransAccount, TransDate, ct.CusIC, CusAccountNo FROM Customer_Transaction ct JOIN Transaction t ON t.TransId = ct.TransId JOIN customer c ON ct.CusIC = c.CusIC WHERE TransDate BETWEEN ? AND ? ORDER BY TransDate");
			break;
		case 2:
			ps = conn.prepareStatement("SELECT t.TransID, TransType, CusTransID, TransAmount, TransDesc, TransAccount, TransDate, ct.CusIC, CusAccountNo FROM Customer_Transaction ct JOIN Transaction t ON t.TransId = ct.TransId JOIN customer c ON ct.CusIC = c.CusIC WHERE TransDate BETWEEN ? AND ? AND ct.TransID = 3 ORDER BY TransDate");
			break;
		case 3:
			ps = conn.prepareStatement("SELECT t.TransID, TransType, CusTransID, TransAmount, TransDesc, TransAccount, TransDate, ct.CusIC, CusAccountNo FROM Customer_Transaction ct JOIN Transaction t ON t.TransId = ct.TransId JOIN customer c ON ct.CusIC = c.CusIC WHERE TransDate BETWEEN ? AND ? AND ct.TransID = 1 ORDER BY TransDate");
			break;
		case 4:
			ps = conn.prepareStatement("SELECT t.TransID, TransType, CusTransID, TransAmount, TransDesc, TransAccount, TransDate, ct.CusIC, CusAccountNo FROM Customer_Transaction ct JOIN Transaction t ON t.TransId = ct.TransId JOIN customer c ON ct.CusIC = c.CusIC WHERE TransDate BETWEEN ? AND ? AND ct.TransID = 2 ORDER BY TransDate");
			break;
		}
		break;
	case 2:
		switch (choice)
		{
		case 1:
			ps = conn.prepareStatement("SELECT t.TransID, TransType, CusTransID, TransAmount, TransDesc, TransAccount, TransDate, ct.CusIC, CusAccountNo FROM Customer_Transaction ct JOIN Transaction t ON t.TransId = ct.TransId JOIN customer c ON ct.CusIC = c.CusIC WHERE TransDate BETWEEN ? AND ? ORDER BY TransDate DESC");
			break;
		case 2:
			ps = conn.prepareStatement("SELECT t.TransID, TransType, CusTransID, TransAmount, TransDesc, TransAccount, TransDate, ct.CusIC, CusAccountNo FROM Customer_Transaction ct JOIN Transaction t ON t.TransId = ct.TransId JOIN customer c ON ct.CusIC = c.CusIC WHERE TransDate BETWEEN ? AND ? AND ct.TransID = 3 ORDER BY TransDate DESC");
			break;
		case 3:
			ps = conn.prepareStatement("SELECT t.TransID, TransType, CusTransID, TransAmount, TransDesc, TransAccount, TransDate, ct.CusIC, CusAccountNo FROM Customer_Transaction ct JOIN Transaction t ON t.TransId = ct.TransId JOIN customer c ON ct.CusIC = c.CusIC WHERE TransDate BETWEEN ? AND ? AND ct.TransID = 1 ORDER BY TransDate DESC");
			break;
		case 4:
			ps = conn.prepareStatement("SELECT t.TransID, TransType, CusTransID, TransAmount, TransDesc, TransAccount, TransDate, ct.CusIC, CusAccountNo FROM Customer_Transaction ct JOIN Transaction t ON t.TransId = ct.TransId JOIN customer c ON ct.CusIC = c.CusIC WHERE TransDate BETWEEN ? AND ? AND ct.TransID = 2 ORDER BY TransDate DESC");
			break;
		}
		break;
	}
	ps->setString(1,start);
	ps->setString(2,end);
	sql::ResultSet* rs = ps->executeQuery();
	Transaction* trans = NULL;
	Transaction* head = NULL;

	while (rs->next())
	{
		if (trans == NULL)
		{
			trans = new Transaction();
			head = trans;
		}
		else
		{
			trans->next = new Transaction();
			trans = trans->next;
		}
		transform(rs, trans);
	}

	rs->close();
	ps->close();

	delete rs;
	delete ps;

	return head;
}

//generate monthly data
void ManageTransaction::generateSummary(std::string month[], int count[]) 
{
	DbConnect conn;
	sql::PreparedStatement* ps = conn.prepareStatement("SELECT MONTHNAME(TransDate), COUNT(TransDate) FROM customer_transaction GROUP BY MONTH(TransDate)");
	sql::ResultSet* rs = ps->executeQuery();
	int i = 0;
	while (rs->next())
	{
		month[i] = rs->getString("MONTHNAME(TransDate)");
		count[i] = rs->getInt("COUNT(TransDate)");
		i++;
	}

	rs->close();
	ps->close();

	delete rs;
	delete ps;
}



//delete restore
int ManageTransaction::deleteRestore(std::string id)
{
	DbConnect conn;
	sql::PreparedStatement* ps = conn.prepareStatement("DELETE FROM Restore_Trans WHERE CusTransID = ?");
	ps->setString(1, id);
	int rowAffected = ps->executeUpdate();
	if (rowAffected == 1) {
		std::cout << "\t\t\t<<Transaction details permenantly deleted.>>\n";
		system("pause");
	}
}

//restore transaction
int ManageTransaction::restoreTransaction(std::string id)
{
	DbConnect conn;
	sql::PreparedStatement* ps = conn.prepareStatement("SELECT r.CusTransID, r.TransDate, r.TransAmount, r.TransDesc, r.TransAccount, r.CusIC, r.TransID, r.DeleteDate, c.CusAccountNo, t.TransType FROM Restore_Trans r JOIN Customer c ON c.CusIC = r.CusIC JOIN Transaction t ON t.TransID = r.TransID  WHERE CusTransID = ?");
	ps->setString(1, id);
	sql::ResultSet* rs = ps->executeQuery();
	//cout << "dh run";
	Transaction* trans = new Transaction();
	if (rs->next())
	{
		//cout << "sebelum transform";
		transformRestore(rs, trans);
		//cout << "dh transform";
	}
	system("pause");
	ps = conn.prepareStatement("DELETE FROM Restore_Trans WHERE CusTransID = ?");
	ps->setString(1, id);
	int rowAffected = ps->executeUpdate();
	if (rowAffected == 1) {
		//cout << "dh delete";
		char curDate[40];
		conn.getCurrentTime(curDate);
		ps = conn.prepareStatement("INSERT INTO Customer_Transaction(CusTransID, TransDate, TransAmount, TransDesc, TransAccount, CusIC, TransID) VALUES(?,?,?,?,?,?,?)");
		ps->setString(1, trans->Id);
		ps->setString(2, trans->Date);
		ps->setDouble(3, trans->Amount);
		ps->setString(4, trans->Desc);
		ps->setString(5, trans->ReceiveAcc);
		ps->setString(6, trans->CusIC);
		ps->setInt(7, trans->TypeID);
		int rowInsert = ps->executeUpdate();
		if (rowInsert == 1) {
			std::cout << "\n\n<<Transaction details has been restored.>>\n";
			system("pause");
		}
		else {
			std::cout << "\n\n<<Restoration failed.>>\n";
			system("pause");
		}
	}
	else {
		std::cout << "\n\n<<Restoration failed.>>\n";
		system("pause");
	}
	rs->close();
	ps->close();
	delete rs;
	delete ps;
	return rowAffected;
}

//delete transaction
int ManageTransaction::deleteTransaction(std::string id)
{
	DbConnect conn;
	sql::PreparedStatement* ps = conn.prepareStatement("SELECT t.TransID, TransType, CusTransID, TransAmount, TransDesc, TransAccount, TransDate, ct.CusIC, CusAccountNo FROM Customer_Transaction ct JOIN Transaction t ON t.TransId = ct.TransId JOIN customer c ON ct.CusIC = c.CusIC WHERE CusTransID = ?");
	ps->setString(1, id);
	sql::ResultSet* rs = ps->executeQuery();
	Transaction* trans = new Transaction();
	if (rs->next())
	{
		transform(rs, trans);
	}
	ps = conn.prepareStatement("DELETE FROM Customer_Transaction WHERE CusTransID = ?");
	ps->setString(1,id);
	int rowAffected = ps->executeUpdate();
	if (rowAffected == 1) {
		char curDate[40];
		conn.getCurrentTime(curDate);
		ps = conn.prepareStatement("INSERT INTO Restore_Trans(CusTransID, TransDate, TransAmount, TransDesc, TransAccount, CusIC, TransID, DeleteDate) VALUES(?,?,?,?,?,?,?,?)");
		ps->setString(1,trans->Id);
		ps->setString(2,trans->Date);
		ps->setDouble(3,trans->Amount);
		ps->setString(4,trans->Desc);
		ps->setString(5,trans->ReceiveAcc);
		ps->setString(6,trans->CusIC);
		ps->setInt(7,trans->TypeID);
		ps->setString(8,curDate);
		int rowInsert = ps->executeUpdate();
		if (rowInsert == 1) {
			std::cout << "\t\t\t<<Transaction details does not permenantly deleted, it will be store in restore bin.>>\n";
			system("pause");
		}
		else {
			std::cout << "\n\n<<Deletion failed.>>\n";
			system("pause");
		}
	}
	else {
		std::cout << "\n\n<<Deletion failed.>>\n";
		system("pause");
	}
	rs->close();
	ps->close();
	delete rs;
	delete ps;	
	return rowAffected;
}

//get restore transaction list
Transaction* ManageTransaction::getRestore()
{
	DbConnect conn;
	sql::PreparedStatement* ps = conn.prepareStatement("SELECT r.CusTransID, r.TransDate, r.TransAmount, r.TransDesc, r.TransAccount, r.CusIC, r.TransID, r.DeleteDate, c.CusAccountNo, t.TransType FROM Restore_Trans r JOIN Customer c ON c.CusIC = r.CusIC JOIN Transaction t ON t.TransID = r.TransID");
	sql::ResultSet* rs = ps->executeQuery();
	Transaction* trans = NULL;
	Transaction* head = NULL;

	while (rs->next())
	{
		if (trans == NULL)
		{
			trans = new Transaction();
			head = trans;
		}
		else
		{
			trans->next = new Transaction();
			trans = trans->next;
		}
		transformRestore(rs, trans);
	}

	rs->close();
	ps->close();

	delete rs;
	delete ps;

	return head;
}
