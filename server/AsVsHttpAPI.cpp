#include "server.h"

using namespace std;
using namespace folly;
using namespace proxygen;

#include "CustomerDb.h"
#include "AccountDb.h"
#include "AsVsHttpAPI.h"


void AsVsApiHandler::onRequest(std::unique_ptr<HTTPMessage> req) noexcept
{
    try
    {
        this->message = move(req);
        auto path = message->getPath();
        VLOG(1) << "[AS & VS HTTP API request] " << path << endl;

        waiting_post = true;
    }
    catch (const std::exception& e)
    {
        logError(500, "Internal Server Error", e.what());
    }
}

void AsVsApiHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept
{
    try
    {
        auto post_body = body->moveToFbString().toStdString();
        auto path = message->getPath();
        waiting_post = false;
        VLOG(1) << "[AS & VS HTTP API request body] " << post_body << endl;

        logError(404, "Not Found", "API does not exist: " + path);
    }
    catch (const std::exception& e)
    {
        logError(500, "Internal Server Error", e.what());
    }
}


void AsVsApiHandler::onEOM() noexcept
{
    ResponseBuilder builder(downstream_);
    if (waiting_post)
        logError(400, "Bad Request", "AS & VS HTTP  API handler was waiting for request body but received EOM. Unexpected bad things may have happened and may happen in the future.");

    ResponseBuilder(downstream_)
        .sendWithEOM();
}

void AsVsApiHandler::onUpgrade(UpgradeProtocol /*protocol*/) noexcept
{
    // handler doesn't support upgrades
}

void AsVsApiHandler::requestComplete() noexcept
{
    delete this;
}

void AsVsApiHandler::onError(ProxygenError /*err*/) noexcept
{
    delete this;
}

void AsVsApiHandler::logError(uint16_t code, const string& reason, const string& msg)
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
