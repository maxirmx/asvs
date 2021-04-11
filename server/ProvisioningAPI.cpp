#include "server.h"

using namespace std;
using namespace folly;
using namespace proxygen;

#include "CustomerDb.h"
#include "AccountDb.h"
#include "ProvisioningAPI.h"


void ProvApiHandler::onRequest(std::unique_ptr<HTTPMessage> req) noexcept
{
    try
    {
        this->message = move(req);
        auto path = message->getPath();
        VLOG(1) << "[Provisioning API request] " << path << endl;

        waiting_post = true;

        /*
        if (path == "/api/remap") 
        {
         //   this->Remap();
        }
        else if (   path == "/api/customer_create"      ||
                    path == "/api/customer_update"      ||
                    path == "/api/customer_delete"      || 
                    path == "/api/customer_ip_create"   ||
                    path == "/api/customer_ip_update"   ||
                    path == "/api/customer_ip_delete"   ||
                    path == "/api/account_create"       ||
                    path == "/api/account_update"       ||
                    path == "/api/account_delete"       ||
                    path == "/api/gateway_ip_create"    ||
                    path == "/api/gateway_ip_update"    ||
                    path == "/api/gateway_ip_delete"    ||
                    path == "/api/cert_create"          ||
                    path == "/api/cert_update"          ||
                    path == "/api/cert_delete"          ||
                    path == "/api/tn_create"            ||
                    path == "/api/tn_update"            ||
                    path == "/api/tn_delete"            ) waiting_post = true;
        else
            logError(404, "Not Found", "API does not exist: " + path);
            */
    }
    catch (const std::exception& e)
    {
        logError(500, "Internal Server Error", e.what());
    }
}

void ProvApiHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept
{
    try
    {
        auto post_body = body->moveToFbString().toStdString();
        auto path = message->getPath();
        waiting_post = false;
        bool not_found = false;
        string r;

        VLOG(1) << "[Provisioning API request body] " << post_body << endl;

        if (path == "/api/customer_create")         r = customerCreate(post_body);
        else if (path == "/api/customer_update")    r = customerUpdate(post_body);
        else if (path == "/api/customer_delete")    r = customerDelete(post_body);
        else if (path == "/api/customer_ip_create") r = customerIpCreate(post_body);
        else if (path == "/api/customer_ip_update") r = customerIpUpdate(post_body);
        else if (path == "/api/customer_ip_delete") r = customerIpDelete(post_body);
        else if (path == "/api/account_create")     r = accountCreate(post_body);
        else if (path == "/api/account_update")     r = accountUpdate(post_body);
        else if (path == "/api/account_delete")     r = accountDelete(post_body);
        else if (path == "/api/gateway_ip_create")  r = gatewayIpCreate(post_body);
        else if (path == "/api/gateway_ip_update")  r = gatewayIpUpdate(post_body);
        else if (path == "/api/gateway_ip_delete")  r = gatewayIpDelete(post_body);
        else if (path == "/api/cert_create")        r = certCreate(post_body);
        else if (path == "/api/cert_update")        r = certUpdate(post_body);
        else if (path == "/api/cert_delete")        r = certDelete(post_body);
        else if (path == "/api/tn_create")          r = tnCreate(post_body);
        else if (path == "/api/tn_update")          r = tnUpdate(post_body);
        else if (path == "/api/tn_delete")          r = tnDelete(post_body);
        else not_found = true;

        if (not_found) logError(404, "Not Found", "API does not exist: " + path);
        else
        {
            ResponseBuilder(downstream_)
                .status(200, "OK")
                .body(r)
                .send();

        }
    }
    catch (const AsVsException& e)
    {
        logError(400, "Bad Request", e.what());
    }
    catch (const boost::property_tree::ptree_error& e)
    {
        logError(400, "Bad Request", e.what());
    }
    catch (const std::exception& e)
    {
        logError(500, "Internal Server Error", e.what());
    }

}


void ProvApiHandler::onEOM() noexcept
{
    ResponseBuilder builder(downstream_);
    if (waiting_post)
        logError(400, "Bad Request", "Provisioning API handler was waiting for request body but received EOM. Unexpected bad things may have happened and may happen in the future.");

    ResponseBuilder(downstream_)
        .sendWithEOM();
}

void ProvApiHandler::onUpgrade(UpgradeProtocol /*protocol*/) noexcept 
{
    // handler doesn't support upgrades
}

void ProvApiHandler::requestComplete() noexcept 
{
    delete this;
}

void ProvApiHandler::onError(ProxygenError /*err*/) noexcept 
{
    delete this;
}

void ProvApiHandler::logError(uint16_t code, const string& reason, const string& msg)
{
    LOG(ERROR) << msg << std::endl;

    boost::property_tree::ptree resJson;
    resJson.put("Message", msg);
    stringstream ss;
    boost::property_tree::json_parser::write_json(ss, resJson);
    ResponseBuilder(downstream_)
        .status(code, reason)
        .body(ss.str())
        .send();
}


string ProvApiHandler::customerCreate(const string& body)
{
    spCustomerInfo spCInfo(body, db_connection);
    return spCInfo.__insert();
}

string ProvApiHandler::customerUpdate(const string& body)
{
    spCustomerInfo spCInfo(body, db_connection);
    return spCInfo.__update();
}

string ProvApiHandler::customerDelete(const string& body)
{
    spCustomerInfo spCInfo(body, db_connection);
    return spCInfo.__delete();   
}
string ProvApiHandler::customerIpCreate(const string& body)
{
    spCustomerIpInfo spIpInfo(body, db_connection);
    return spIpInfo.__insert();
}

string ProvApiHandler::customerIpUpdate(const string& body)
{
    spCustomerIpInfo spIpInfo(body, db_connection);
    return spIpInfo.__update();
}

string ProvApiHandler::customerIpDelete(const string& body)
{
    spCustomerIpInfo spIpInfo(body, db_connection);
    return spIpInfo.__delete();
}

string ProvApiHandler::accountCreate(const string& body)
{
    spAccountInfo spAccInfo(body, db_connection);
    return spAccInfo.__insert();
}

string ProvApiHandler::accountUpdate(const string& body)
{
    spAccountInfo spAccInfo(body, db_connection);
    return spAccInfo.__update();
}

string ProvApiHandler::accountDelete(const string& body)
{
    spAccountInfo spAccInfo(body, db_connection);
    return spAccInfo.__delete();
}

string ProvApiHandler::gatewayIpCreate(const string& body)
{
    spGatewayIpInfo spIpInfo(body, db_connection);
    return spIpInfo.__insert();
}

string ProvApiHandler::gatewayIpUpdate(const string& body)
{
    spGatewayIpInfo spIpInfo(body, db_connection);
    return spIpInfo.__update();
}

string ProvApiHandler::gatewayIpDelete(const string& body)
{
    spGatewayIpInfo spIpInfo(body, db_connection);
    return spIpInfo.__delete();
}

string ProvApiHandler::certCreate(const string& body)
{
    spCertInfo spCInfo(body, db_connection);
    return spCInfo.__insert();
}

string ProvApiHandler::certUpdate(const string& body)
{
    spCertInfo spCInfo(body, db_connection);
    return spCInfo.__update();
}

string ProvApiHandler::certDelete(const string& body)
{
    spCertInfo spCInfo(body, db_connection);
    return spCInfo.__delete();
}

string ProvApiHandler::tnCreate(const string& body)
{
    spTnInfo spTInfo(body, db_connection);
    return spTInfo.__insert();
}

string ProvApiHandler::tnUpdate(const string& body)
{
    spTnInfo spTInfo(body, db_connection);
    return spTInfo.__update();
}

string ProvApiHandler::tnDelete(const string& body)
{
    spTnInfo spTInfo(body, db_connection);
    return spTInfo.__delete();
}
