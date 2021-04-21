// spCustomerInfo 
// spCustomerIpInfo 
// spAccountInfo 
// spAsGatewayInfo
// spVsGatewayInfo
// spTnInfo
// spCertInfo

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

class spAsGatewayIpInfo: public spBaseObject
{
public:
    spAsGatewayIpInfo(const std::string& body);
    spAsGatewayIpInfo(const pqxx::row& r);

private:
    void __proto(void);
};

class spVsGatewayIpInfo : public spBaseObject
{
public:
    spVsGatewayIpInfo(const std::string& body);
    spVsGatewayIpInfo(const pqxx::row& r);

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

class spCertInfo: public spBaseObject
{
public:
    spCertInfo(const std::string& body);
    spCertInfo(const pqxx::row& r);

private:
    void __proto(void);
};


