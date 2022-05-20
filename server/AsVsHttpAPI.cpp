#include "server.h"

using namespace std;
using namespace folly;
using namespace proxygen;

#include "AsVsHttpAPI.h"


void AsVsApiHandler::onRequest(std::unique_ptr<HTTPMessage> req) noexcept
{
    try
    {
        this->message = move(req);

        auto path = message->getPath();
        auto headers = message->getHeaders();

/*  ATIS - 1000082
    5.2 Special Request Header Requirements
    5.3 Special Response Header Requirements

    The X-RequestID transaction ID should be included in order to make possible the transaction traceability  in case of troubleshooting
    and fault analysis. If received, it will not be validated explicitly by server.If not received, it will be automatically generated
    by STI - AS/VS service on request receipt. Received/Generated transaction ID will be returned back in the corresponding HTTP response
    in `X-RequestID` header.
*/
        XRequestID = headers.getSingleOrEmpty("X-RequestID");
        VLOG(1) << "[AS & VS HTTP API request] " << path << " XRequestID: '" << XRequestID << "'";

        if (XRequestID.empty())
        {
            XRequestID = boost::lexical_cast<std::string>(generator());
            VLOG(1) << "                    -----> XRequestID: '" << XRequestID << "'";
        }

        waiting_post = true;
    }
    catch (const std::exception& e)
    {
        yeildError(5000, e.what());
    }
}

void AsVsApiHandler::onBody(std::unique_ptr<folly::IOBuf> body) noexcept
{
    waiting_post = false;

    try
    {

        auto headers = message->getHeaders();

        string XInstanceID = headers.getSingleOrEmpty("X-InstanceID");
        string ContentType = headers.getSingleOrEmpty("Content-Type");
        string ContentLength = headers.getSingleOrEmpty("Content-Length");
        string Accept = headers.getSingleOrEmpty("Accept");
        AsVsOp op = AsVsUnknown;


        /*  ATIS - 1000082

            5.2 Special Request Header Requirements

            The following headers are expected to be sent in all HTTP requests :
            RequestID       -- optional
            InstanceID      -- optional
            Content-Type    -- mandatory Valid value is : �application/json�.
            Accept          -- optional  If specified, has to contain �application/json� content type, otherwise HTTP request will be rejected with �406 Not Acceptable� HTTP Status Code.
                                         If not specified, will be default handled as �application/json"
        */
        if (ContentType != "application/json")
        {
            yeildError(4004, "Only 'application/json' Content-Type is supported.", "application/json"   );
        }
        else if (Accept != "application/json" && Accept != "*/*")
        {
            yeildError(4002, "ATIS-1000082 requires Accept header field to be set to 'application/json', not '" + Accept + "'", Accept);
        }
        else if (ContentLength == "")
        {
            yeildError(4007, "Content-Length header is missing");
        }
        else
        {
            auto post_body = body->moveToFbString().toStdString();
            auto path = message->getPath();
            auto client_ip = message->getClientIP();


            VLOG(1) << "[AS & VS HTTP API request body] " << post_body << endl;

            if (path == "/stir/v1/signing")              op = AsVsSigning;
            else if (path == "/stir/v1/verification")    op = AsVsVerify;

            switch (op)
            {
            case AsVsSigning:
                yeildResponse(signing(post_body));
                break;
            case AsVsVerify:
                yeildResponse(verify(post_body));
                break;
            case AsVsUnknown:
            default:
                yeildError(4003, "API does not exist: " + path);
                break;
            }
        }
    }
    catch (const std::exception& e)
    {
        yeildError(5000, e.what());
    }
}

void AsVsApiHandler::onEOM() noexcept
{
    ResponseBuilder builder(downstream_);
    if (waiting_post)
        yeildError(4000, "AS & VS HTTP  API handler was waiting for request body but received EOM.");

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

void AsVsApiHandler::yeildResponse(const string& r)
{
    ResponseBuilder(downstream_)
        .status(200, HTTPMessage::getDefaultReason(200))
        .header("Content-Type", "application/json")
        .header("X-RequestID", XRequestID)
        .body(r)
        .send();
}

// ATIS-1000082
// 6.8 Datatype: exception
// 6.10 Datatype: requestError

void AsVsApiHandler::yeildError(uint16_t AsVsCode, const string& msg, const string& p1, const string& p2)
{
    uint16_t HttpCode = AsVsCode2HttpCode(AsVsCode);
    stringstream ss;

    LOG(ERROR) << "Error: " << AsVsCode2AsVsExceptionKey(AsVsCode) + " AsVsCode:" << AsVsCode << " Message: " << msg;

 //  Something like  { "requestError": { "serviceException": { "messageId": "POL4003", "text" : "Error: Requested resource was not found."  }  }  }

 ss <<
 "{ \n"
 " \"requestError\": \n"
 "  {\n"
 "    \"" << AsVsCode2AsVsExceptionKey(AsVsCode) << "\": \n"
 "    {\n"
 "        \"messageId\": \"" << AsVsCode2AsVsId(AsVsCode) << "\",\n"
 "        \"text\": \"" << AsVsCode2AsVsText(AsVsCode) << "\", \n"
 "        \"variables\": [ " << (p1 == "" ? "" : "\"" + p1 + "\"" + (p2 == "" ? "" : " , ")) + (p2 == "" ? "" : " , ") << " ]\n"
 "    }\n"
 "  }\n"
 "}\n";

 string x = ss.str();
    ResponseBuilder(downstream_)
        .status(HttpCode, HTTPMessage::getDefaultReason(HttpCode))
        .header("Content-Type", "application/json")
        .header("X-RequestID", XRequestID)
        .body(x)
        .send();
}

const string AsVsApiHandler::signing(const string& body)
{
    return "{ \"msg\": \"Thank you for the fish\"}";
}

const string AsVsApiHandler::verify(const string& body)
{
    return "{ \"msg\": \"Oh, My Godness !\"}";
}


uint16_t AsVsApiHandler::AsVsCode2HttpCode(uint16_t AsVsCode)
{
    uint16_t HttpCode;

    switch (AsVsCode)
    {
    case 4050:
        HttpCode = 405;
        break;
    default:
    case 5000:
        HttpCode = 500;
        break;
    case 4000:
        HttpCode = 400;
        break;
    case 4001:
        HttpCode = 400;
        break;
    case 4002:
        HttpCode = 406;
        break;
    case 4003:
        HttpCode = 404;
        break;
    case 4004:
        HttpCode = 415;
        break;
    case 4005:
        HttpCode = 400;
        break;
    case 4006:
        HttpCode = 400;
        break;
    case 4007:
        HttpCode = 411;
        break;
    }

    return HttpCode;
}

const string AsVsApiHandler::AsVsCode2AsVsId(uint16_t AsVsCode)
{
    string AsVsId;

    switch (AsVsCode)
    {
    case 4050:
        AsVsId = "POL4050";
        break;
    default:
    case 5000:
        AsVsId = "POL5000";
        break;
    case 4000:
        AsVsId = "POL4000";
        break;
    case 4001:
        AsVsId = "POL4001";
        break;
    case 4002:
        AsVsId = "POL4002";
        break;
    case 4003:
        AsVsId = "POL4003";
        break;
    case 4004:
        AsVsId = "POL4004";
        break;
    case 4005:
        AsVsId = "POL4005";
        break;
    case 4006:
        AsVsId = "POL4006";
        break;
    case 4007:
        AsVsId = "POL4007";
        break;
    }

    return AsVsId;
}

const string AsVsApiHandler::AsVsCode2AsVsText(uint16_t AsVsCode)
{
    string AsVsText = "Error: ";

    switch (AsVsCode)
    {
    case 4050:
        AsVsText += "Method not allowed";
        break;
    default:
    case 5000:
        AsVsText += "Internal Server Error. Please try again later.";
        break;
    case 4000:
        AsVsText += "Missing request body.";
        break;
    case 4001:
        AsVsText += "Missing mandatory parameter �%1�.";
        break;
    case 4002:
        AsVsText += "Requested response body type �%1� is not supported.";
        break;
    case 4003:
        AsVsText += "Requested resource was not found.";
        break;
    case 4004:
        AsVsText += "Unsupported request body type, expected �%1�.";
        break;
    case 4005:
        AsVsText += "Error: Invalid �%1� parameter value : %2.";
        break;
    case 4006:
        AsVsText += "Error: Failed to parse received message body : %1.";
        break;
    case 4007:
        AsVsText += "Missing mandatory Content-Length header";
        break;
    }

    return AsVsText;

}

const string AsVsApiHandler::AsVsCode2AsVsExceptionKey(uint16_t AsVsCode)
{
    string AsVsExceptionKey;

    switch (AsVsCode)
    {
    default:
    case 4050:
    case 5000:
        AsVsExceptionKey = "policyException";
        break;
    case 4000:
    case 4001:
    case 4002:
    case 4003:
    case 4004:
    case 4005:
    case 4006:
    case 4007:
        AsVsExceptionKey = "serviceException";
        break;
    }

    return AsVsExceptionKey;

}

