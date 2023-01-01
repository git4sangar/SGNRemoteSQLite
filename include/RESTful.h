//sgn
#pragma once

#include <iostream>
#include <memory>

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/mime.h>
#include <pistache/net.h>
#include <pistache/router.h>

#include "DBInterface.h"

class RESTful {
public:
    using PistacheReq   = Pistache::Rest::Request;
    using PistacheResp  = Pistache::Http::ResponseWriter;

private:
    DBInterface::Ptr mpDBInterface;
    Pistache::Rest::Router mRouter;
    std::shared_ptr<Pistache::Http::Endpoint> mEndPoint;

    void selectQuery(const PistacheReq &request, PistacheResp response);
    void updateQuery(const PistacheReq &request, PistacheResp response);
    void selectUpdate(const PistacheReq &request, PistacheResp& response, bool bFlag);
    void selectNotUpdate(const PistacheReq &request, PistacheResp response);
    void selectSucceedUpdate(const PistacheReq &request, PistacheResp response);

    void configureRoutes();
    json packResp(bool isOk, const std::string& pStrErr);

public:
    RESTful(uint32_t pPort, DBInterface::Ptr pDBInterface)
        : mpDBInterface(pDBInterface)
        , mEndPoint(std::make_shared<Pistache::Http::Endpoint>(Pistache::Address("68.183.84.172", pPort)))
        //, mEndPoint(std::make_shared<Pistache::Http::Endpoint>(Pistache::Address("UGC13R4HN83.chennaigtbc.ford.com", pPort)))
    {}

    virtual ~RESTful() {}
    void run();
};
