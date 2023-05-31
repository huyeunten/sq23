#include <iostream>

class Database {
    private:
        std::string command;
        static Database *instance;
        Database() {}
    public:
        Database(Database &other) = delete;
        void operator=(Database &other) = delete;
        static Database *getInstance() {
            if (instance == nullptr) 
                instance = new Database();
            return instance;
        }
        void setCommand(std::string c) {
            command = c;
        }
        void print() {
            std::cout << "Entered " << command << std::endl;
        }
};

Database* Database::instance = nullptr;

int main() {
    // Invalid command, private
    // Database d = new Database();
    Database *db = Database::getInstance();
    db->setCommand("test 1");
    db->print();
}