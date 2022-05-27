#include "server.h"

using namespace std;

std::unique_ptr<DbConnection> DbConnection::d;

DbConnection::DbConnection(string postgr) : postgres(postgr)
{
	c = make_unique<pqxx::connection>(postgres);
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

	if (version.size() == 0) throw(InternalError("Schema version unknown. Most likely this is the wrong host or schema."));
	else                 	 descr << "  Schema version:" << version[0][0].as<int>();

	return descr.str();
}
