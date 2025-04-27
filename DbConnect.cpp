#include <ctime>
#include <chrono>
#include "DbConnect.h"

DbConnect::DbConnect()
{
	sql::mysql::MySQL_Driver* driver = sql::mysql::get_mysql_driver_instance();
	connection = driver->connect("tcp://localhost", "root", "");

	connection->setSchema("bankdb");
}

DbConnect::~DbConnect()
{
	connection->close();

	delete connection;
}

sql::PreparedStatement* DbConnect::prepareStatement(std::string query)
{
	return connection->prepareStatement(query);
}

//void DbConnect::getCurrentTime(char (&curDate)[30])
//{
//	auto start = std::chrono::system_clock::now();
//	auto legacyStart = std::chrono::system_clock::to_time_t(start);
//	ctime_s(curDate, sizeof(curDate), &legacyStart);
//}
void DbConnect::getCurrentTime(char(&str)[40])
{
	tm newDate;
	auto start = std::chrono::system_clock::now();
	auto legacyStart = std::chrono::system_clock::to_time_t(start);
	localtime_s(&newDate, &legacyStart);
	strftime(str, sizeof(str), "%Y-%m-%d %T", &newDate);
}