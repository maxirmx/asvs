class SigningException: public AsVsException {
public:
    explicit SigningException(uint16_t _AsVsCode, const std::string& message, const std::string& _p1 = "", const std::string& _p2 = "") : 
                    AsVsException(message), AsVsCode(_AsVsCode), p1(_p1), p2(_p2) {  }
    uint16_t getCode(void) const noexcept { return AsVsCode; }
    const char* getp1(void) const noexcept { return p1.c_str(); }
    const char* getp2(void) const noexcept { return p2.c_str(); }
protected:
    uint16_t AsVsCode;
    std::string p1;
    std::string p2;
};

class AsVsApiHandler : public RequestHandler
{
public:
    explicit AsVsApiHandler(): XRequestID(""), waiting_post(false) {  }
    void onRequest(unique_ptr<proxygen::HTTPMessage> headers) noexcept override;
    void onBody(unique_ptr<folly::IOBuf> body) noexcept override;
    void onEOM() noexcept override;
    void onUpgrade(UpgradeProtocol proto) noexcept override;
    void requestComplete() noexcept override;
    void onError(ProxygenError err) noexcept override;

private:
    enum AsVsOp
    {
        AsVsSigning,
        AsVsVerify,
        AsVsUnknown
    };

    static uint16_t    AsVsCode2HttpCode(uint16_t AsVsCode);
    static const std::string AsVsCode2AsVsId(uint16_t AsVsCode);
    static const std::string AsVsCode2AsVsText(uint16_t AsVsCode);
    static const std::string AsVsCode2AsVsExceptionKey(uint16_t AsVsCode);

    void yeildResponse(const std::string& r);
    void yeildError(uint16_t AsVsCode, const std::string& msg, const std::string& p1 = "", const std::string& p2 = "");

    const std::string signing(const std::string& clientIp, const std::string& body);
    const std::string verify(const std::string& body);

    std::unique_ptr<HTTPMessage> message;
    std::string XRequestID;
    boost::uuids::random_generator generator;

    bool waiting_post;
};

class AsVsApiHandlerFactory : public RequestHandlerFactory
{
public:
    AsVsApiHandlerFactory(void)  {   }

    void onServerStart(folly::EventBase* /*evb*/) noexcept override {   }
    void onServerStop() noexcept override {   }

    RequestHandler* onRequest(RequestHandler*, HTTPMessage*) noexcept override { return new AsVsApiHandler(); }
};
