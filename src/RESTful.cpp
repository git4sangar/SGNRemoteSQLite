//sgn

#include <iostream>
#include <exception>
#include <fstream>
#include <sstream>

#include <nlohmann_json.hpp>
#include "RESTful.h"

using json = nlohmann::json;

void RESTful::configureRoutes()
{
    Pistache::Rest::Routes::Put(mRouter,    "/selectquery", Pistache::Rest::Routes::bind(&RESTful::selectQuery, this));
    Pistache::Rest::Routes::Put(mRouter,    "/sqlupdate",   Pistache::Rest::Routes::bind(&RESTful::updateQuery, this));
    Pistache::Rest::Routes::Put(mRouter,    "/selectnotupdate",   Pistache::Rest::Routes::bind(&RESTful::selectNotUpdate, this));
    Pistache::Rest::Routes::Put(mRouter,    "/selectsucceedupdate",   Pistache::Rest::Routes::bind(&RESTful::selectSucceedUpdate, this));
    //Pistache::Rest::Routes::Delete(mRouter, "/sqlinsert",   Pistache::Rest::Routes::bind(&RESTful::delQuery, this));
    //Pistache::Rest::Routes::Post(mRouter, "/sqlquery",    Pistache::Rest::Routes::bind(&RESTful::getQueryResult, this));
}

void RESTful::run() {
    mEndPoint->init(Pistache::Http::Endpoint::options().threads(5).flags(Pistache::Tcp::Options::ReuseAddr).flags(Pistache::Tcp::Options::ReusePort));
    configureRoutes();
    mEndPoint->setHandler(mRouter.handler());
    mEndPoint->serve();
}

void RESTful::selectQuery(const PistacheReq &request, PistacheResp response) {
    json pRes;

    std::string strQuery	= request.body();
    if(strQuery.empty()) {
		response.send(Pistache::Http::Code::Not_Found, packResp(false, "Blank Query").dump(), MIME(Application, Json));
		return;
	}

    std::cout << "Got a select query : " << strQuery << std::endl;
    json pRoot      = mpDBInterface->selectQuery(strQuery);
    if(!pRoot.is_array()) {
        response.send(Pistache::Http::Code::Not_Found, packResp(false, "No Resp").dump(), MIME(Application, Json));
        return;
    }
    pRes["isOk"]    = true;
    pRes["count"]   = pRoot.size();
    pRes["rows"]    = pRoot;
    response.send(Pistache::Http::Code::Ok, pRes.dump(), MIME(Application, Json));
}

void RESTful::selectNotUpdate(const PistacheReq &request, PistacheResp response) { selectUpdate(request, response, false); }
void RESTful::selectSucceedUpdate(const PistacheReq &request, PistacheResp response) { selectUpdate(request, response, true); }
void RESTful::selectUpdate(const PistacheReq &request, PistacheResp& response, bool bFlag) {
    std::string strQuery    = request.body();
    if(strQuery.empty()) {
        response.send(Pistache::Http::Code::Not_Found, packResp(false, "Blank Query").dump(), MIME(Application, Json));
        return;
    }

    (bFlag) ?
    std::cout << "Got a select-succeed-update query : " << strQuery << std::endl :
    std::cout << "Got a select-not-update query : " << strQuery << std::endl;

    bool isCommitted        = mpDBInterface->selectAndUpdate(strQuery, bFlag);
    if(isCommitted)  response.send(Pistache::Http::Code::Ok, packResp(true, "Committed").dump(), MIME(Application, Json));
    else response.send(Pistache::Http::Code::Bad_Request, packResp(false, "Exists Already").dump(), MIME(Application, Json));
}

void RESTful::updateQuery(const PistacheReq &request, PistacheResp response) {
    std::string strQuery    = request.body();
    if(strQuery.empty()) {
        response.send(Pistache::Http::Code::Not_Found, packResp(false, "Blank Query").dump(), MIME(Application, Json));
        return;
    }
    std::cout << "Got a update query : " << strQuery << std::endl;
    bool isCommitted= mpDBInterface->transacStmt(strQuery);

    if(isCommitted)  response.send(Pistache::Http::Code::Ok, packResp(true, "Committed").dump(), MIME(Application, Json));
    else response.send(Pistache::Http::Code::Bad_Request, packResp(false, "Blank Query").dump(), MIME(Application, Json)); 
}

json RESTful::packResp(bool isOk, const std::string& pStatus) {
    json pRoot;
    pRoot["isOk"]   = isOk;
    pRoot["stat"]   = pStatus;
    return pRoot;
}
