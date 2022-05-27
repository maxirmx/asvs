#include "server.h"

using namespace std;

std::unique_ptr<DbInMemory> DbInMemory::d;

void DbInMemory::loadDb(void)
{
	pqxx::result r = DbConnection::d->exec("SELECT * FROM sp_customers");
	for (auto row = r.begin(); row != r.end(); row++) {
        auto n = make_shared<spCustomerInfo>(row);
        cuMap[n->getKey()] = n;
    }
	LOG(INFO) << "Loaded " << r.size() << " records from sp_customers.";

	r = DbConnection::d->exec("SELECT * FROM sp_customer_ip");
	for (auto row = r.begin(); row != r.end(); row++) {
        auto n = make_shared<spCustomerIpInfo>(row);
        cuIpMap[n->getKey()] = n;
    }
	LOG(INFO) << "Loaded " << r.size() << " records from sp_customer_ip.";

	r = DbConnection::d->exec("SELECT * FROM sp_accounts");
	for (auto row = r.begin(); row != r.end(); row++) {
        auto n = make_shared<spAccountInfo>(row);
        acMap[n->getKey()] = n;
    }
	LOG(INFO) << "Loaded " << r.size() << " records from sp_accounts.";

	r = DbConnection::d->exec("SELECT * FROM sp_gateway_ip");
	for (auto row = r.begin(); row != r.end(); row++) {
        auto n = make_shared<spGatewayIpInfo>(row);
        gwIpMap[n->getKey()] = n;
    }
	LOG(INFO) << "Loaded " << r.size() << " records from sp_gateway_ip.";

	r = DbConnection::d->exec("SELECT * FROM sp_tn");
	for (auto row = r.begin(); row != r.end(); row++) {
        auto n = make_shared<spTnInfo>(row);
        tnMap[n->getKey()] = n;
    }
	LOG(INFO) << "Loaded " << r.size() << " records from sp_tn.";
}

std::shared_ptr<spCustomerIpInfo> DbInMemory::findCustomerIpInfo(const std::string& ip) {
    for(auto iter = cuIpMap.begin(); iter != cuIpMap.end(); ++iter){
        if (iter->second->find("customer_ip")->getStringVal() == ip) return iter->second; 
    	LOG(INFO) << "Lookup: " << iter->second->find("customer_ip")->getStringVal() << " vs " << ip;
    }
	LOG(INFO) << "Returning nullptr.";
    return nullptr;
}
