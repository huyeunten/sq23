#include <iostream>
#include "singleton.h"

Database *Database::getInstance() {
    if (instance == nullptr) 
        instance = new Database();
    return instance;
}
void Database::databaseMethods() {
    std::cout << "Methods" << std::endl;
}
