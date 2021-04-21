#include "server.h"

using namespace std;
using namespace folly;
using namespace proxygen;

string spBaseObject::__insert(void)
{
    auto search = fields.find("modified_on");
    auto modified_on = chrono::system_clock::now();
    if (search != fields.end()) *fields["modified_on"] = modified_on;
    else                        fields.insert({ "modified_on", make_shared<spTimeColumn>(modified_on) });
  
    string statement = "INSERT INTO " + __getNME() + __crtIUS(" VALUES ", true) + " RETURNING *";

    pqxx::result r = DbConnection::d->exec(statement);
    __rmpIUS(r[0]);

    return __srsIUS();
}

string spBaseObject::__update(void)
{
    __chkUUID();

    auto search = fields.find("modified_on");
    auto modified_on = chrono::system_clock::now();
    if (search != fields.end()) *fields["modified_on"] = modified_on;
    else                        fields.insert({ "modified_on", make_shared <spTimeColumn>(modified_on) });


    auto qk = DbConnection::DbConnection::d->c->quote(fields[__getMKE()]->getStringVal());
    string statement = "UPDATE " + __getNME() + " SET " + __crtIUS(" = ", false) + " WHERE " + __getMKE() + " = " + qk + " RETURNING * ";

    pqxx::result r = DbConnection::d->exec(statement);
    if (r.size() == 0) throw (AsVsException("Record with " + __getMKE() + " = " + qk + " not found"));
    __rmpIUS(r[0]);

    return __srsIUS(); 
}

string spBaseObject::__delete(void)
{
    __chkUUID();

    auto qk = DbConnection::DbConnection::d->c->quote(fields[__getMKE()]->getStringVal());
    string statement = "DELETE FROM " + __getNME() + " WHERE " + __getMKE() + " = " + qk + " RETURNING * ";

    pqxx::result r = DbConnection::d->exec(statement);
    if (r.size() == 0) throw (AsVsException("Record with " + __getMKE() + " = " + qk + " not found"));
    __rmpIUS(r[0]);

    return __srsIUS();
}


void spBaseObject::__rmpINT(const std::string& body)
{
    boost::property_tree::ptree pt;
    istringstream is(body);
    boost::property_tree::read_json(is, pt);

    for (const std::pair<std::string, fptr_pt>& field : proto_fields_pt)
    {
        (this->*field.second)(field.first, pt);
    }
}

void   spBaseObject::__rmpIUS(const pqxx::row& r)
{
    for (const std::pair<std::string, fptr_pq>& field : proto_fields_pq)
    {
        (this->*field.second)(field.first, r);
    }
}

string spBaseObject::__crtIUS(const string& eqs, bool with_uuid)
{
    stringstream prefix;
    stringstream suffix;
    size_t count = 0;
    string delim = "";

    for (std::pair<std::string, shared_ptr<spBaseColumn>> field : fields)
    {
        if (field.first != master_key || with_uuid)
        {
            prefix << delim << field.first;
            suffix << delim << field.second->dump();
            delim = ",";
            count++;
        }
    }

    bool b1 = (count > 1);
    stringstream res;
    res << (b1 ? "(" : "") << prefix.str() << (b1 ? ")" : "") << eqs << (b1 ? "(" : "") << suffix.str() << (b1 ? ")" : "");

    string rw = res.str();
    return rw;
}

string   spBaseObject::__srsIUS(void)
{
    boost::property_tree::ptree r;

    for (std::pair<std::string, shared_ptr<spBaseColumn>> field : fields)
        field.second->put(field.first, r);

    std::stringstream ss;
    boost::property_tree::json_parser::write_json(ss, r);

    return ss.str();
}
