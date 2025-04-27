#include <string>
#include <iomanip>
#include <mysql/jdbc.h>

#ifndef DBCONNECT_H
#define DBCONNECT_H

class DbConnect
{
private:
	sql::Connection* connection;

public:
	DbConnect();
	~DbConnect();
	sql::PreparedStatement* prepareStatement(std::string query);
	//void getCurrentTime(char(&)[30]);
	void getCurrentTime(char(&)[40]);
};

#endif