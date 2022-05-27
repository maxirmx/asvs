#include "server.h"

using namespace std;
using namespace folly;
using namespace proxygen;

#include "ProvisioningAPI.h"

void ProvApiHandler::onRequest(std::unique_ptr<HTTPMessage> req) noexcept
{
    try
    {
        this->message = move(req);
        auto path = message->getPath();
        VLOG(1) << "[Provisioning API request] " << path << endl;

        if (path == "/api/stop") 
        {
            string msg = "Stop receieved, exiting.";
            LOG(INFO) << msg << std::endl;
            sendMessage(200, msg);
            raise(SIGTERM);
        }
        else 
        {
            waiting_post = true;
        }
    }
    catch (const std::exception& e)
    {
        logError(500, e.what());
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
        else if (path == "/api/gateway_ip_create")  r = gwIpCreate(post_body);
        else if (path == "/api/gateway_ip_update")  r = gwIpUpdate(post_body);
        else if (path == "/api/gateway_ip_delete")  r = gwIpDelete(post_body);
        else if (path == "/api/tn_create")          r = tnCreate(post_body);
        else if (path == "/api/tn_update")          r = tnUpdate(post_body);
        else if (path == "/api/tn_delete")          r = tnDelete(post_body);
        else not_found = true;

        if (not_found) logError(404, "API does not exist: " + path);
        else
        {
            ResponseBuilder(downstream_)
                .status(200, HTTPMessage::getDefaultReason(200))
                .body(r)
                .send();
        }
    }
    catch (const AsVsException& e)
    {
        logError(400, e.what());
    }
    catch (const boost::property_tree::ptree_error& e)
    {
        logError(400, e.what());
    }
    catch (const pqxx::failure& e)
    {
        logError(400, e.what());
    }
    catch (const std::exception& e)
    {
        logError(500, e.what());
    }
}

void ProvApiHandler::onEOM() noexcept
{
    ResponseBuilder builder(downstream_);
    if (waiting_post)
        logError(400, "Provisioning API handler was waiting for request body but received EOM. Unexpected bad things may have happened and may happen in the future.");

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

void ProvApiHandler::sendMessage(uint16_t code, const string& msg)
{
    boost::property_tree::ptree resJson;
    resJson.put("Message", msg);
    stringstream ss;
    boost::property_tree::json_parser::write_json(ss, resJson);
    ResponseBuilder(downstream_)
        .status(code, HTTPMessage::getDefaultReason(code))
        .body(ss.str())
        .send();

}

void ProvApiHandler::logError(uint16_t code, const string& msg)
{
    LOG(ERROR) << msg << std::endl;
    sendMessage(code, msg);
}

string ProvApiHandler::customerCreate(const string& body)
{
    auto spCInfo = make_shared<spCustomerInfo>(body);
    string r = spCInfo->__insert();
    DbInMemory::d->cuMap[spCInfo->getKey()] = spCInfo;
    return r;
}

string ProvApiHandler::customerUpdate(const string& body)
{
    auto spCInfo = make_shared<spCustomerInfo>(body);
    string r = spCInfo->__update();
    DbInMemory::d->cuMap[spCInfo->getKey()] = spCInfo;
    return r;
}

string ProvApiHandler::customerDelete(const string& body)
{
    spCustomerInfo spCInfo(body);
    DbInMemory::d->cuMap.erase(spCInfo.getKey());
    return spCInfo.__delete();
}
string ProvApiHandler::customerIpCreate(const string& body)
{
    auto spIpInfo = make_shared<spCustomerIpInfo>(body);
    string r = spIpInfo->__insert();
    DbInMemory::d->cuIpMap[spIpInfo->getKey()] = spIpInfo;
    return r;
}

string ProvApiHandler::customerIpUpdate(const string& body)
{
    auto spIpInfo = make_shared<spCustomerIpInfo>(body);
    string r = spIpInfo->__update();
    DbInMemory::d->cuIpMap[spIpInfo->getKey()] = spIpInfo;
    return r;
}

string ProvApiHandler::customerIpDelete(const string& body)
{
    spCustomerIpInfo spIpInfo(body);
    DbInMemory::d->cuIpMap.erase(spIpInfo.getKey());
    return spIpInfo.__delete();
}

string ProvApiHandler::accountCreate(const string& body)
{
    auto spAccInfo = make_shared<spAccountInfo>(body);
    string r = spAccInfo->__insert();
    DbInMemory::d->acMap[spAccInfo->getKey()] = spAccInfo;
    return r;
}

string ProvApiHandler::accountUpdate(const string& body)
{
    auto spAccInfo = make_shared<spAccountInfo>(body);
    string r = spAccInfo->__update();
    DbInMemory::d->acMap[spAccInfo->getKey()] = spAccInfo;
    return r;
}

string ProvApiHandler::accountDelete(const string& body)
{
    spAccountInfo spAccInfo(body);
    DbInMemory::d->acMap.erase(spAccInfo.getKey());
    return spAccInfo.__delete();
}

string ProvApiHandler::gwIpCreate(const string& body)
{
    auto spIpInfo = make_shared<spGatewayIpInfo>(body);
    string r = spIpInfo->__insert();
    DbInMemory::d->gwIpMap[spIpInfo->getKey()] = spIpInfo;
    return r;
}

string ProvApiHandler::gwIpUpdate(const string& body)
{
    auto spIpInfo = make_shared<spGatewayIpInfo>(body);
    string r = spIpInfo->__update();
    DbInMemory::d->gwIpMap[spIpInfo->getKey()] = spIpInfo;
    return r;
}

string ProvApiHandler::gwIpDelete(const string& body)
{
    spGatewayIpInfo spIpInfo(body);
    DbInMemory::d->gwIpMap.erase(spIpInfo.getKey());
    return spIpInfo.__delete();
}

string ProvApiHandler::tnCreate(const string& body)
{
    auto spTInfo = make_shared<spTnInfo>(body);
    string r = spTInfo->__insert();
    DbInMemory::d->tnMap[spTInfo->getKey()] = spTInfo;
    return r;
}

string ProvApiHandler::tnUpdate(const string& body)
{
    auto spTInfo = make_shared<spTnInfo>(body);
    string r = spTInfo->__update();
    DbInMemory::d->tnMap[spTInfo->getKey()] = spTInfo;
    return r;
}

string ProvApiHandler::tnDelete(const string& body)
{
    spTnInfo spTInfo(body);
    DbInMemory::d->tnMap.erase(spTInfo.getKey());
    return spTInfo.__delete();
}
