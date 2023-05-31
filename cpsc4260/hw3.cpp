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

class Animal {
    protected:
        void walk() {
            std::cout << "Walk" << std::endl;
        }
        void run() {
            std::cout << "Run" << std::endl;
        }
        virtual void sound() = 0;
    public:
        Animal() {};
        void templateMethod() {
            walk();
            sound();
            run();
        }
};

class Cat : public Animal {
    protected:
        void sound() {
            std::cout << "Meow" << std::endl;
        }
    public:
        Cat() : Animal() {}
};

class Dog : public Animal {
    protected:
        void sound() {
            std::cout << "Woof" << std::endl;
        }
    public:
        Dog() : Animal() {}
};

Database* Database::instance = nullptr;

int main() {
    // Invalid command, private
    // Database d = new Database();
    std::cout << "Singleton object:" << std::endl;
    Database *db = Database::getInstance();
    db->setCommand("test");
    db->print();

    std::cout << std::endl;

    std::cout << "Template method:" << std::endl;
    Cat cat;
    Dog dog;
    cat.templateMethod();
    std::cout << std::endl;
    dog.templateMethod();
}

