#include "sqlCon.h"

sqlCon::sqlCon(const std::string &hostname, const std::string &username, const std::string& password, const std::string& database)
{
    con = new mysqlpp::Connection(false);

    if (!con->connect(database.c_str(), hostname.c_str(), username.c_str(), password.c_str()))
	connected = false;
    else
	connected = true;
}

sqlCon::~sqlCon()
{
    delete con;
}

bool sqlCon::execute(const std::string& command)
{
    mysqlpp::Query quer = con->query();
    quer << command;
    results = quer.store();

    return true;
}
