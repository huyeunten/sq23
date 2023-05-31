#include <iostream>
#include "singleton.h"

int main() {
    // Invalid, private constructor
    // Database d = Database();
    Database *db = Database::getInstance();
    db->databaseMethods();
}