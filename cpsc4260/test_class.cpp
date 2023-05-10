#include <iostream>
#include "customer.h"

using namespace std;

Customer::Customer(unsigned int n) {
    accountNum = n;
    balance = 0;
}

Customer::Customer(unsigned int n, double b) {
    accountNum = n;
    balance = b;
}

void Customer::addMoney(double m) {
    if (m < 0)
        throw invalid_argument("Cannot add negative money");
    balance += m;
}

void Customer::purchase(double p) {
    balance -= p;
}

double Customer::getBalance() const {
    return balance;
}