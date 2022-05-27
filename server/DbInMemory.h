class DbInMemory
{
public:
	void loadDb(void);
    static std::unique_ptr<DbInMemory> d;

	std::unordered_map<std::string, std::shared_ptr<spCustomerInfo>> cuMap;
	std::unordered_map<std::string, std::shared_ptr<spCustomerIpInfo>> cuIpMap;
	std::unordered_map<std::string, std::shared_ptr<spAccountInfo>> acMap;
	std::unordered_map<std::string, std::shared_ptr<spGatewayIpInfo>> gwIpMap;
	std::unordered_map<std::string, std::shared_ptr<spTnInfo>> tnMap;

    std::shared_ptr<spCustomerIpInfo> findCustomerIpInfo(const std::string& ip);
};

