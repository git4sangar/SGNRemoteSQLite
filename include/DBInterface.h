//sgn

#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <unordered_map>

#include "SQLiteCpp/SQLiteCpp.h"
#include "nlohmann_json.hpp"

using json = nlohmann::json;

class DBInterface {
    std::shared_ptr<SQLite::Database> mDB;
    std::unordered_map<std::string, int16_t> mNameToType;

    std::vector<std::string>    splitString(const std::string& pString);
public:
    typedef std::shared_ptr<DBInterface> Ptr;
    DBInterface(std::string pDBFileName) {
        mDB = std::make_shared<SQLite::Database>(pDBFileName, SQLite::OPEN_READWRITE);
    }

    void init();
    json selectQuery(const std::string& pSelect);
    bool transacStmt(const std::string& pTransc);
    bool transacStmt(const std::vector<std::string>& pQueries);
    bool selectAndUpdate(const std::string &pTransc, bool bFlag = false);
};
