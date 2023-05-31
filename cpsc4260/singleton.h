#include <iostream>

#ifndef DATABASE_H
#define DATABASE_H

class Database {
    private:
        static Database *instance;
        Database();
    public:
        Database(Database &other) = delete;
        void operator=(Database &other) = delete;
        static Database *getInstance();
        void databaseMethods();
};

#endif