//sgn

#include <iostream>
#include <memory>
#include <unistd.h>

#include "RESTful.h"
#include "DBInterface.h"

int main() {
    pid_t pid;
    pid = fork();
    
    //  Exit the parent process
    if(pid > 0) {
        std::cout << "Exiting pid " << getpid() << std::endl;
        exit(EXIT_SUCCESS);
    }

    //  -ve return means error
    if(pid < 0) {
        std::cout << "Error creating child process" << std::endl;
        return -1;
    }
    
    Logger::getInstance() << "SGN" << std::endl;

    DBInterface::Ptr pDBInterface = std::make_shared<DBInterface>("/home/tstone10/sgn/proj/SGNRemoteSQLite/db/iso_review_assit.db");
    pDBInterface->init();

    RESTful *pRestful = new RESTful(8080, pDBInterface);
    pRestful->run();
    return 0;
}
