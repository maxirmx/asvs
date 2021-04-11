#include "server.h"

using namespace std;


DbConnection::DbConnection(string postgr) : postgres(postgr)
{
	c = make_shared<pqxx::connection>(postgres);
}

pqxx::result DbConnection::exec(string stmt)
{
	pqxx::work txn   { *c };
	pqxx::result res { txn.exec(stmt) };
	txn.commit();
	return res;
}

string DbConnection::describe(void)
{
	stringstream descr;
	descr << "Connection properties :" << std::endl <<
		     "  Host name     :  " << c->hostname()	<< std::endl <<
		     "  Port          :  " << c->port()		<< std::endl <<
		     "  User name     :  " << c->username()	<< std::endl <<
		     "  Database name :  " << c->dbname()	<< std::endl;
	
	pqxx::result version = exec("SELECT version_number FROM __db_schema_version ORDER BY version_number DESC LIMIT 1");

	descr << "  Schema version:" << version[0][0].as<int>();

	return descr.str();
}



