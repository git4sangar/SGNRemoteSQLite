//sgn
#include <iostream>

#include "DBInterface.h"
#include "nlohmann_json.hpp"

using json = nlohmann::json;

/*
 *  Schema of data)type table
 *  CREATE TABLE IF NOT EXISTS "data_type" ("id" INTEGER, "name" TEXT, "type" INTEGER, PRIMARY KEY(id));
 *  Field name : name of the column
 *  Field type : 1 -> int32_t, 2 -> int64_t, 3 -> std::string
 *
 */
void DBInterface::init() {
    SQLite::Statement query(*mDB, "SELECT * FROM data_type");
    while(query.executeStep()) {
        std::string colName = query.getColumn("name").getString();
        int16_t     dataTyp = query.getColumn("type").getInt();
        mNameToType.insert({ colName, dataTyp });
    }
}

json DBInterface::selectQuery(const std::string& pSelect) {
    if(pSelect.empty()) return false;
    json jsRows;

    SQLite::Statement query(*mDB, pSelect);
    jsRows = json::array();

    while(query.executeStep()) {
        json jsResp;
        uint32_t iColCount  = query.getColumnCount();

        for(uint32_t iLoop = 0; iLoop < iColCount; iLoop++) {
            std::string colName = query.getColumnName(iLoop);
            if(mNameToType.count(colName) == 0) continue;

            switch(mNameToType.at(colName)) {
                case 1: jsResp[colName] = query.getColumn(colName.c_str()).getInt(); break;
                case 2: jsResp[colName] = query.getColumn(colName.c_str()).getInt64(); break;
                case 3: jsResp[colName] = query.getColumn(colName.c_str()).getString(); break;
            }
        }
        jsRows.push_back(jsResp);
    }
    return jsRows;
}

bool DBInterface::transacStmt(const std::string& pTransc) {
    if(pTransc.empty()) return false;

    SQLite::Transaction transaction(*mDB);
    std::vector<std::string> queries = splitString(pTransc);
    try {
        for(const auto& query : queries) {
            if(query.empty()) continue;
            mLogger << "Executing : " << query << std::endl;
            mDB->exec(query);
        }
        transaction.commit();
    } catch(SQLite::Exception& sqlEx) {
        mLogger << sqlEx.what() << std::endl;
        return false;
    }
    return true;
}

bool DBInterface::transacStmt(const std::vector<std::string>& pQueries) {
    if(pQueries.size() == 0) return false;

    SQLite::Transaction transaction(*mDB);
    try {
        for(const auto& query : pQueries) {
            if(query.empty()) continue;
            mLogger << "Executing : " << query << std::endl;
            mDB->exec(query);
        }
        transaction.commit();
    } catch(SQLite::Exception& sqlEx) {
        mLogger << sqlEx.what() << std::endl;
        return false;
    }
    return true;
}

bool DBInterface::selectAndUpdate(const std::string &pTransc, bool bFlag) {
    if(pTransc.empty()) return false;

    std::vector<std::string> queries = splitString(pTransc);
    if(queries.size() < 2) return false;

    SQLite::Statement query(*mDB, queries[0]);
    mLogger << "Executing : " << queries[0] << std::endl;
    if(query.executeStep() == bFlag) {
        queries.erase(queries.begin());
        return transacStmt(queries);
    }
    return false;
}

std::vector<std::string> DBInterface::splitString(const std::string& strLine) {
    std::vector<std::string> operands;
    std::string strToken;

    for (auto ch : strLine) {
        if (ch == '$') {
            if (strToken.empty()) continue;
            operands.push_back(strToken);
            strToken.clear();
        }
        else strToken += ch;
    }
    if (!strToken.empty()) operands.push_back(strToken);
    return operands;
}
