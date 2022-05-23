// spCustomerInfo
// spCustomerIpInfo
// spAccountInfo
// spGatewayInfo
// spTnInfo

class spCustomerInfo: public spBaseObject
{
public:
    spCustomerInfo(const std::string& body);
    spCustomerInfo(const pqxx::row& r);

private:
    void __proto(void);
};

class spCustomerIpInfo: public spBaseObject
{
public:
    spCustomerIpInfo(const std::string& body);
    spCustomerIpInfo(const pqxx::row& r);

private:
    void __proto(void);
};

class spAccountInfo: public spBaseObject
{
public:
    spAccountInfo(const std::string& body);
    spAccountInfo(const pqxx::row& r);

private:
    void __proto(void);
};

class spGatewayIpInfo: public spBaseObject
{
public:
    spGatewayIpInfo(const std::string& body);
    spGatewayIpInfo(const pqxx::row& r);

private:
    void __proto(void);
};

class spTnInfo:public spBaseObject
{
public:
    spTnInfo(const std::string& body);
    spTnInfo(const pqxx::row& r);

private:
    void __proto(void);
};
