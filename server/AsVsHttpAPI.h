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

    static uint16_t    AsVsCode2HttpCode(uint16_t AsVsCode);
    static const std::string AsVsCode2AsVsId(uint16_t AsVsCode);
    static const std::string AsVsCode2AsVsText(uint16_t AsVsCode);
    static const std::string AsVsCode2AsVsExceptionKey(uint16_t AsVsCode);

    void yeildResponse(const std::string& r);
    void yeildError(uint16_t AsVsCode, const std::string& msg, const std::string& p1 = "", const std::string& p2 = "");

    const std::string signing(const std::string& body);
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
