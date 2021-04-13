class DbConnection
{
public:
    DbConnection(std::string postgr);
    pqxx::result exec(std::string stmt);
    std::string describe(void);

    std::unique_ptr<pqxx::connection> c;
    static std::unique_ptr<DbConnection> d;
private:
    std::string postgres;
};

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

