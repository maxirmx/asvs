class DbConnection;

class spAccountInfo
{
public:
    spAccountInfo(const std::string& body);
    spAccountInfo(const pqxx::row& r);

    std::string __insert(void);
    std::string __update(void);
    std::string __delete(void);
private:
    std::string __crtIUS(const std::string& eqs, bool with_uuid);
    void   __rmpIUS(const pqxx::row& r);
    std::string __srsIUS(void);
    void   __chkUUID(void) { if (!has_sp_account_uuid) throw (AsVsException("Cannot identify record since 'sp_account_uuid' parameter is missing.")); }

    std::string                         sp_account_uuid;
    bool                                active;
    std::chrono::system_clock::time_point    started_on;
    std::chrono::system_clock::time_point    modified_on;
    std::chrono::system_clock::time_point    expired_on;
    int32_t                             cps_limit;
    bool                                as_enabled;
    bool                                vs_enabled;
    //    ATT RFC5213  https ://tools.ietf.org/html/rfc5213#page-74 
    //    Access Technology Type Option
    std::string                         default_att_level_with_sp_ani;              // CHARACTER(1)
    std::string                         default_att_level_wo_sp_ani;                // CHARACTER(1)
    std::string                         sp_name;
    std::string                         priv_phase;

    bool                               has_sp_account_uuid;
    bool                               has_active;
    bool                               has_started_on;
    bool                               has_modified_on;
    bool                               has_expired_on;
    bool                               has_cps_limit;
    bool                               has_as_enabled;
    bool                               has_vs_enabled;
    bool                               has_default_att_level_with_sp_ani;
    bool                               has_default_att_level_wo_sp_ani;
    bool                               has_sp_name;
    bool                               has_priv_phase;
};

class spGatewayIpInfo
{
public:
    spGatewayIpInfo(const std::string& body);
    spGatewayIpInfo(const pqxx::row& r);

    std::string __insert(void);
    std::string __update(void);
    std::string __delete(void);

private:
    std::string __crtIUS(const std::string& eqs, bool with_uuid);
    void   __rmpIUS(const pqxx::row& r);
    std::string __srsIUS(void);
    void   __chkUUID(void) { if (!has_sp_gateway_ip_uuid) throw (AsVsException("Cannot identify record since 'sp_gateway_ip_uuid' parameter is missing.")); }

    std::string                        sp_account_uuid;
    std::string                        sp_gateway_ip_uuid;
    std::string                        gateway_ip;

    bool                               has_sp_account_uuid;
    bool                               has_sp_gateway_ip_uuid;
    bool                               has_gateway_ip;

};

class spTnInfo
{
public:
    spTnInfo(const std::string& body);
    spTnInfo(const pqxx::row& r);

    std::string __insert(void);
    std::string __update(void);
    std::string __delete(void);

private:
    std::string __crtIUS(const std::string& eqs, bool with_uuid);
    void   __rmpIUS(const pqxx::row& r);
    std::string __srsIUS(void);
    void   __chkUUID(void)  { if (!has_sp_tn_uuid) throw (AsVsException("Cannot identify record since 'sp_tn_uuid' parameter is missing.")); }

    std::string                   sp_account_uuid;
    std::string                   sp_tn_uuid;
    std::string                   tn;

    bool                               has_sp_account_uuid;
    bool                               has_sp_tn_uuid;
    bool                               has_tn;

};

class spCertInfo
{
public:
    spCertInfo(const std::string& body);
    spCertInfo(const pqxx::row& r);

    std::string __insert(void);
    std::string __update(void);
    std::string __delete(void);

private:
    std::string __crtIUS(const std::string& eqs, bool with_uuid);
    void   __rmpIUS(const pqxx::row& r);
    std::string __srsIUS(void);
    void   __chkUUID(void) { if (!has_sp_cert_uuid) throw (AsVsException("Cannot identify record since 'sp_cert_uuid' parameter is missing.")); }

    std::string                      sp_account_uuid;
    std::string                      sp_cert_uuid;
    std::string                      priv_key;
    std::string                      pem_file_path;

    bool                               has_sp_account_uuid;
    bool                               has_sp_cert_uuid;
    bool                               has_priv_key;
    bool                               has_pem_file_path;
};

