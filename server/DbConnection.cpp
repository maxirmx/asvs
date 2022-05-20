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


class DbInMemory
{
public:
	void loadDb(void);


	std::unordered_map<std::string, std::shared_ptr<spCustomerInfo>> cuMap;
	std::unordered_map<std::string, std::shared_ptr<spCustomerIpInfo>> cuIpMap;
	std::unordered_map<std::string, std::shared_ptr<spAccountInfo>> acMap;
	std::unordered_map<std::string, std::shared_ptr<spGatewayIpInfo>> gwIpMap;
	std::unordered_map<std::string, std::shared_ptr<spTnInfo>> tnMap;
	std::unordered_map<std::string, std::shared_ptr<spCertInfo>> crMap;
};



void DbInMemory::loadDb(void)
{
	pqxx::result r = DbConnection::d->exec("SELECT * FROM sp_customers");
	for (auto row = r.begin(); row != r.end(); row++) { auto n = make_shared<spCustomerInfo>(row); }
	LOG(INFO) << "Loaded " << r.size() << " records from sp_customers.";

	r = DbConnection::d->exec("SELECT * FROM sp_customer_ip");
	for (auto row = r.begin(); row != r.end(); row++) { auto n = make_shared<spCustomerIpInfo>(row); }
	LOG(INFO) << "Loaded " << r.size() << " records from sp_customer_ip.";

	r = DbConnection::d->exec("SELECT * FROM sp_accounts");
	for (auto row = r.begin(); row != r.end(); row++) { auto n = make_shared<spAccountInfo>(row); }
	LOG(INFO) << "Loaded " << r.size() << " records from sp_accounts.";

	r = DbConnection::d->exec("SELECT * FROM sp_gateway_ip");
	for (auto row = r.begin(); row != r.end(); row++) { auto n = make_shared<spGatewayIpInfo>(row); }
	LOG(INFO) << "Loaded " << r.size() << " records from sp_gateway_ip.";

	r = DbConnection::d->exec("SELECT * FROM sp_tn");
	for (auto row = r.begin(); row != r.end(); row++) { auto n = make_shared<spTnInfo>(row); }
	LOG(INFO) << "Loaded " << r.size() << " records from sp_tn.";

	r = DbConnection::d->exec("SELECT * FROM sp_cert");
	for (auto row = r.begin(); row != r.end(); row++) { auto n = make_shared<spCertInfo>(row); }
	LOG(INFO) << "Loaded " << r.size() << " records from sp_cert.";
}


