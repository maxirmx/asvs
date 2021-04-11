class DbConnection
{
public:
    DbConnection(std::string postgr);
    pqxx::result exec(std::string stmt);
    std::string describe(void);

    std::shared_ptr<pqxx::connection> c;
private:
    std::string postgres;
};

/*class DbMemory
{
    std::unordered_map<std::string, spCustomerInfo> cuMap;
    std::unordered_map<std::string, spCustomerIPInfo> cuIpMap;
    std::unordered_map<std::string, spAccountInfo> acMap;
    std::unordered_map<std::string, spGatewayIpInfo> gwIpMap;
    std::unordered_map<std::string, spTnInfo> tnMap;
    std::unordered_map<std::string, spCertInfo> crMap;
};

*/