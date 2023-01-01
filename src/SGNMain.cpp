//sgn

#include <iostream>
#include <memory>

#include "RESTful.h"
#include "DBInterface.h"

int main() {
    std::cout << "SGN" << std::endl;

    DBInterface::Ptr pDBInterface = std::make_shared<DBInterface>("/home/sgn/sgn/projs/SGNIso/db/iso_review_assit.db");
    pDBInterface->init();

    RESTful *pRestful = new RESTful(8080, pDBInterface);
    pRestful->run();
    return 0;
}
