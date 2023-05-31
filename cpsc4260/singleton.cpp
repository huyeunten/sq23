#include <iostream>
#include "singleton.h"

Database::Database() {
    instance = new Database;
}
Database *Database::getInstance() {
    return instance;
}
void Database::databaseMethods() {
    std::cout << "Methods" << std::endl;
}
