#include "server.h"

using namespace std;
using namespace folly;
using namespace proxygen;


spCustomerInfo::spCustomerInfo(const string& body):
                                has_sp_customer_uuid (false),
                                has_active(false), 
                                has_started_on(false),
                                has_modified_on(false),
                                has_expired_on(false),
                                has_cps_limit(false), 
                                has_as_enabled(false),
                                has_vs_enabled(false), 
                                has_sp_customer_name(false)
{
    boost::property_tree::ptree pt;
    istringstream is(body);
    boost::property_tree::read_json(is, pt);

    auto _sp_customer_uuid  = pt.get_optional<string>   ("sp_customer_uuid");
    auto _active            = pt.get_optional<bool>     ("active");
    auto _started_on        = pt.get_optional<string>   ("started_on");
    auto _modified_on       = pt.get_optional<string>   ("modified_on");
    auto _expired_on        = pt.get_optional<string>   ("expired_on");
    auto _cps_limit         = pt.get_optional<int32_t>  ("cps_limit");
    auto _as_enabled        = pt.get_optional<bool>     ("as_enabled");
    auto _vs_enabled        = pt.get_optional<bool>     ("vs_enabled");
    auto _sp_customer_name  = pt.get_optional<string>   ("sp_customer_name");

    if (_sp_customer_uuid)  { has_sp_customer_uuid =    true;  sp_customer_uuid =  *_sp_customer_uuid;  }
    if (_active)            { has_active =              true;  active =            *_active;            }
    if (_started_on)        { has_started_on =          true;  started_on =    str2time(*_started_on);  }
    if (_modified_on)       { has_modified_on =         true;  modified_on =   str2time (*_modified_on);}
    if (_expired_on)        { has_expired_on =          true;  expired_on =    str2time(*_expired_on);  }
    if (_cps_limit)         { has_cps_limit =           true;  cps_limit =         *_cps_limit;         }
    if (_as_enabled)        { has_as_enabled =          true;  as_enabled =        *_as_enabled;        }
    if (_vs_enabled)        { has_vs_enabled =          true;  vs_enabled =        *_vs_enabled;        }
    if (_sp_customer_name)  { has_sp_customer_name =    true;  sp_customer_name =  *_sp_customer_name;  }
       
}

spCustomerInfo::spCustomerInfo(const pqxx::row& r) :
    has_sp_customer_uuid(false),
    has_active(false),
    has_started_on(false),
    has_modified_on(false),
    has_expired_on(false),
    has_cps_limit(false),
    has_as_enabled(false),
    has_vs_enabled(false),
    has_sp_customer_name(false)
{
    __rmpIUS(r);
}


string spCustomerInfo::__insert(void)
{
    has_modified_on = true; 
    modified_on = chrono::system_clock::now();

    string statement = "INSERT INTO sp_customers " + __crtIUS(" VALUES ", true) + " RETURNING *";

    pqxx::result r = DbConnection::d->exec(statement);
    __rmpIUS(r[0]);

    return __srsIUS();
}

string spCustomerInfo::__update(void)
{
    __chkUUID();

    has_modified_on = true;
    modified_on = chrono::system_clock::now();
    
    string statement = "UPDATE sp_customers SET " + __crtIUS(" = ", false) + "WHERE sp_customer_uuid = " + DbConnection::DbConnection::d->c->quote(sp_customer_uuid) + " RETURNING * ";

    pqxx::result r = DbConnection::d->exec(statement);
    if (r.size() == 0) throw (AsVsException("Record not found"));
    __rmpIUS(r[0]);

    return __srsIUS();
}

string spCustomerInfo::__delete(void)
{
    __chkUUID();

    string statement = "DELETE FROM sp_customers WHERE sp_customer_uuid = " + DbConnection::DbConnection::d->c->quote(sp_customer_uuid) + " RETURNING * ";

    pqxx::result r = DbConnection::d->exec(statement);
    if (r.size() == 0) throw (AsVsException("Record not found"));
    __rmpIUS(r[0]);

    return __srsIUS();
}


string spCustomerInfo::__crtIUS(const string& eqs, bool with_uuid)
{
    stringstream prefix; 
    stringstream suffix; 
    string delim = "(";

    if (with_uuid) 
    {
       if (has_sp_customer_uuid){ prefix << delim << "sp_customer_uuid";    suffix << delim << DbConnection::d->c->quote(sp_customer_uuid);        delim = ","; }
    }
    if (has_active)             { prefix << delim << "active";              suffix << delim << DbConnection::d->c->quote(active);                  delim = ","; }
    if (has_started_on)         { prefix << delim << "started_on";          suffix << delim << DbConnection::d->c->quote(time2str(started_on));    delim = ","; }
    if (has_expired_on)         { prefix << delim << "expired_on";          suffix << delim << DbConnection::d->c->quote(time2str(expired_on));    delim = ","; }
    if (has_modified_on)        { prefix << delim << "modified_on";         suffix << delim << DbConnection::d->c->quote(time2str(modified_on));   delim = ","; }
    if (has_cps_limit)          { prefix << delim << "cps_limit";           suffix << delim << DbConnection::d->c->quote(cps_limit);               delim = ","; }
    if (has_as_enabled)         { prefix << delim << "as_enabled";          suffix << delim << DbConnection::d->c->quote(as_enabled);              delim = ","; }
    if (has_vs_enabled)         { prefix << delim << "vs_enabled";          suffix << delim << DbConnection::d->c->quote(vs_enabled);              delim = ","; }
    if (has_sp_customer_name)   { prefix << delim << "sp_customer_name";    suffix << delim << DbConnection::d->c->quote(sp_customer_name);        delim = ","; }

    prefix << ")" << eqs << suffix.str() << ")";

    return prefix.str();

}

void   spCustomerInfo::__rmpIUS(const pqxx::row& r)
{
    auto f = r["sp_customer_uuid"];  has_sp_customer_uuid = !f.is_null();    if (has_sp_customer_uuid) sp_customer_uuid = f.as<string>();
    f = r["active"];                 has_active = !f.is_null();              if (has_active) active = f.as<bool>();
    f = r["started_on"];             has_started_on = !f.is_null();          if (has_started_on) started_on = str2time(f.as<string>());
    f = r["modified_on"];            has_modified_on = !f.is_null();         if (has_modified_on) modified_on = str2time(f.as<string>());
    f = r["expired_on"];             has_expired_on = !f.is_null();          if (has_expired_on) expired_on = str2time(f.as<string>());
    f = r["cps_limit"];              has_cps_limit = !f.is_null();           if (has_cps_limit) cps_limit = f.as<int>();
    f = r["as_enabled"];             has_as_enabled = !f.is_null();          if (has_as_enabled) as_enabled = f.as<bool>();
    f = r["vs_enabled"];             has_vs_enabled = !f.is_null();          if (has_vs_enabled) vs_enabled = f.as<bool>();
    f = r["sp_customer_name"];       has_sp_customer_name = !f.is_null();    if (has_sp_customer_name) sp_customer_name = f.as<string>();

}

string   spCustomerInfo::__srsIUS(void)
{
    boost::property_tree::ptree r;

    if (has_sp_customer_uuid)       r.put("sp_customer_uuid",   sp_customer_uuid);
    if (has_active)                 r.put("active",             active);
    if (has_started_on)             r.put("started_on",         time2str(started_on));
    if (has_expired_on)             r.put("expired_on",         time2str(expired_on));
    if (has_modified_on)            r.put("modified_on",        time2str(modified_on));
    if (has_cps_limit)              r.put("cps_limit",          cps_limit);
    if (has_as_enabled)             r.put("as_enabled",         as_enabled);
    if (has_vs_enabled)             r.put("vs_enabled",         vs_enabled);
    if (has_sp_customer_name)       r.put("sp_customer_name",   sp_customer_name);
    
    std::stringstream ss;
    boost::property_tree::json_parser::write_json(ss, r);

    return ss.str();
}

// --------------------------------------------------------------------------------------------------------------------------

spCustomerIpInfo::spCustomerIpInfo(const string& body) :
                               has_sp_customer_ip_uuid(false),
                               has_sp_customer_uuid(false),
                               has_customer_ip(false),
                               has_tech_prefix(false),
                               has_att_level_with_sp_ani(false),
                               has_att_level_wo_sp_ani(false)

{
    boost::property_tree::ptree pt;
    istringstream is(body);
    boost::property_tree::read_json(is, pt);

    auto _sp_customer_ip_uuid   = pt.get_optional<string>   ("sp_customer_ip_uuid");
    auto _sp_customer_uuid      = pt.get_optional<string>   ("sp_customer_uuid");
    auto _customer_ip           = pt.get_optional<string>   ("customer_ip");
    auto _tech_prefix           = pt.get_optional<string>   ("tech_prefix");
    auto _att_level_with_sp_ani = pt.get_optional<string>   ("att_level_with_sp_ani");
    auto _att_level_wo_sp_ani   = pt.get_optional<string>   ("att_level_wo_sp_ani");

    if (_sp_customer_ip_uuid)   { has_sp_customer_ip_uuid =     true;  sp_customer_ip_uuid =    *_sp_customer_ip_uuid;  }
    if (_sp_customer_uuid)      { has_sp_customer_uuid =        true;  sp_customer_uuid =       *_sp_customer_uuid;     }
    if (_customer_ip)           { has_customer_ip =             true;  customer_ip =            *_customer_ip;          }
    if (_tech_prefix)           { has_tech_prefix =             true;  tech_prefix =            *_tech_prefix;          }
    if (_att_level_with_sp_ani) { has_att_level_with_sp_ani =   true;  att_level_with_sp_ani =  *_att_level_with_sp_ani;}
    if (_att_level_wo_sp_ani)   { has_att_level_wo_sp_ani =     true;  att_level_wo_sp_ani =    *_att_level_wo_sp_ani;  }

}

spCustomerIpInfo::spCustomerIpInfo(const pqxx::row& r) :
    has_sp_customer_ip_uuid(false),
    has_sp_customer_uuid(false),
    has_customer_ip(false),
    has_tech_prefix(false),
    has_att_level_with_sp_ani(false),
    has_att_level_wo_sp_ani(false)
{
    __rmpIUS(r);
}
string spCustomerIpInfo::__insert(void)
{
    string statement = "INSERT INTO sp_customer_ip " + __crtIUS(" VALUES ", true) + " RETURNING *";

    pqxx::result r = DbConnection::d->exec(statement);
    if (r.size() == 0) throw (AsVsException("Record not found"));
    __rmpIUS(r[0]);

    return __srsIUS();
}

string spCustomerIpInfo::__update(void)
{
    __chkUUID();

    string statement = "UPDATE sp_customer_ip SET " + __crtIUS(" = ", false) + "WHERE sp_customer_ip_uuid = " + DbConnection::d->c->quote(sp_customer_ip_uuid) + " RETURNING * ";

    pqxx::result r = DbConnection::d->exec(statement);
    if (r.size() == 0) throw (AsVsException("Record not found"));
    __rmpIUS(r[0]);

    return __srsIUS();
}

string spCustomerIpInfo::__delete(void)
{
    if (!has_sp_customer_ip_uuid) throw (AsVsException("Nothing to delete since 'sp_customer_ip_uuid' parameter is missing."));

    string statement = "DELETE FROM sp_customer_ip WHERE sp_customer_ip_uuid = " + DbConnection::d->c->quote(sp_customer_ip_uuid) + " RETURNING * ";

    pqxx::result r = DbConnection::d->exec(statement);
    __rmpIUS(r[0]);

    return __srsIUS();
}

string spCustomerIpInfo::__crtIUS(const string& eqs, bool with_uuid)
{
    stringstream prefix;
    stringstream suffix;
    string delim = "(";

    if (with_uuid)
    {
       if (has_sp_customer_ip_uuid) { prefix << delim << "sp_customer_ip_uuid";         suffix << delim << DbConnection::d->c->quote(sp_customer_ip_uuid);    delim = ","; }
    }
    if (has_sp_customer_uuid)       { prefix << delim << "sp_customer_uuid";            suffix << delim << DbConnection::d->c->quote(sp_customer_uuid);       delim = ","; }
    if (has_customer_ip)            { prefix << delim << "customer_ip";                 suffix << delim << DbConnection::d->c->quote(customer_ip);            delim = ","; }
    if (has_tech_prefix)            { prefix << delim << "tech_prefix";                 suffix << delim << DbConnection::d->c->quote(tech_prefix);            delim = ","; }
    if (has_att_level_with_sp_ani)  { prefix << delim << "att_level_with_sp_ani";       suffix << delim << DbConnection::d->c->quote(att_level_with_sp_ani);  delim = ","; }
    if (has_att_level_wo_sp_ani)    { prefix << delim << "att_level_wo_sp_ani";         suffix << delim << DbConnection::d->c->quote(att_level_wo_sp_ani);    delim = ","; }

    prefix << ")" << eqs << suffix.str() << ")";

    return prefix.str();

}

void   spCustomerIpInfo::__rmpIUS(const pqxx::row& r)
{
    auto f = r["sp_customer_ip_uuid"];       has_sp_customer_ip_uuid =   !f.is_null(); if (has_sp_customer_ip_uuid) sp_customer_ip_uuid = f.as<string>();
    f =         r["sp_customer_uuid"];       has_sp_customer_uuid =      !f.is_null(); if (has_sp_customer_uuid) sp_customer_uuid =       f.as<string>();
    f =         r["customer_ip"];            has_customer_ip =           !f.is_null(); if (has_customer_ip) customer_ip =                 f.as<string>();
    f =         r["tech_prefix"];            has_tech_prefix =           !f.is_null(); if (has_tech_prefix) tech_prefix =                 f.as<string>();
    f =         r["att_level_with_sp_ani"];  has_att_level_with_sp_ani = !f.is_null(); if (has_att_level_with_sp_ani) att_level_with_sp_ani = f.as<string>();
    f =         r["att_level_wo_sp_ani"];    has_att_level_wo_sp_ani =   !f.is_null(); if (has_att_level_wo_sp_ani) att_level_wo_sp_ani = f.as<string>();
}

string   spCustomerIpInfo::__srsIUS(void)
{
    boost::property_tree::ptree r;

    if (has_sp_customer_ip_uuid)    r.put("sp_customer_ip_uuid",    sp_customer_ip_uuid);
    if (has_sp_customer_uuid)       r.put("sp_customer_uuid",       sp_customer_uuid);
    if (has_customer_ip)            r.put("customer_ip",            customer_ip);
    if (has_tech_prefix)            r.put("tech_prefix",            tech_prefix);
    if (has_att_level_with_sp_ani)  r.put("att_level_with_sp_ani",  att_level_with_sp_ani);
    if (has_att_level_wo_sp_ani)    r.put("att_level_wo_sp_ani",    att_level_wo_sp_ani);

    std::stringstream ss;
    boost::property_tree::json_parser::write_json(ss, r);

    return ss.str();
}