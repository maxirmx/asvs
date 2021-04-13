class DbConnection;

class spCustomerInfo
{
// [TODO] cps_limit does not look solid enough 
// [TODO] check that uuid is valid on Provisioning API

public:
    spCustomerInfo(const std::string& body);
    spCustomerInfo(const pqxx::row& r);

    std::string __insert(void);
    std::string __update(void);
    std::string __delete(void);

private:
    std::string __crtIUS(const std::string& eqs, bool with_uuid);
    void   __rmpIUS(const pqxx::row& r);
    std::string __srsIUS(void);
    void   __chkUUID(void) { if (!has_sp_customer_uuid) throw (AsVsException("Cannot identify record since 'sp_customer_uuid' parameter is missing.")); }

    std::string                           sp_customer_uuid;
    bool                                  active;
    std::chrono::system_clock::time_point started_on;
    std::chrono::system_clock::time_point modified_on;
    std::chrono::system_clock::time_point expired_on;
    int32_t                               cps_limit;
    bool                                  as_enabled;
    bool                                  vs_enabled;
    std::string                           sp_customer_name;

    bool                                has_sp_customer_uuid;
    bool                                has_active;
    bool                                has_started_on;
    bool                                has_modified_on;
    bool                                has_expired_on;
    bool                                has_cps_limit;
    bool                                has_as_enabled;
    bool                                has_vs_enabled;
    bool                                has_sp_customer_name;
};

class spCustomerIpInfo
{
public:
    spCustomerIpInfo(const std::string& body);
    spCustomerIpInfo(const pqxx::row& r);

    std::string __insert(void);
    std::string __update(void);
    std::string __delete(void);

private:
    std::string __crtIUS(const std::string& eqs, bool with_uuid);
    void   __rmpIUS(const pqxx::row& r);
    std::string __srsIUS(void);
    void   __chkUUID(void) { if (!has_sp_customer_ip_uuid) throw (AsVsException("Cannot identify record since 'sp_customer_ip_uuid' parameter is missing.")); }

    std::string                              sp_customer_ip_uuid;
    std::string                              sp_customer_uuid;
    std::string                              customer_ip;
    std::string                              tech_prefix;

    //    ATT RFC5213  https ://tools.ietf.org/html/rfc5213#page-74 
    //    Access Technology Type Option ?
    std::string                              att_level_with_sp_ani;                   // CHARACTER(1)
    std::string                              att_level_wo_sp_ani;                     // CHARACTER(1)

    bool                               has_sp_customer_ip_uuid;
    bool                               has_sp_customer_uuid;
    bool                               has_customer_ip;
    bool                               has_tech_prefix;
    bool                               has_att_level_with_sp_ani;
    bool                               has_att_level_wo_sp_ani;
};

