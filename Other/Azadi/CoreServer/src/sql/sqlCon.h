#ifndef SQLCON_H
#define SQLCON_H

#include <mysql++.h>
#include <string>

class sqlCon
{
    public:
        sqlCon(const std::string&, const std::string&, const std::string&, const std::string&);
        ~sqlCon();

        bool execute(const std::string&);

	bool connected;

    private:
	mysqlpp::Connection *con;
	mysqlpp::Result	results;
};


#endif
