#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "vendor.h"
#include "entree.h"

using namespace std;

Vendor::Vendor(bool d) {
    isRefrigeratedDisplay = d;
}

Vendor::Vendor(const Vendor &v) {
    isRefrigeratedDisplay = v.getRefrigerationDisplay();
    int size = v.getSize();
    for (int i = 0; i < size; i++) {
        allStock.push_back(unique_ptr<Stock>(new Stock{
            v.getEntree(i),
            v.getPrice(i),
            v.getQuantity(i)}));
    }
} 

Vendor& Vendor::operator=(const Vendor &v) {
    if (this != &v) {
        allStock.clear();
        isRefrigeratedDisplay = v.getRefrigerationDisplay();
        int size = v.getSize();
        for (int i = 0; i < size; i++) {
            allStock.push_back(unique_ptr<Stock>(new Stock{
                v.getEntree(i),
                v.getPrice(i),
                v.getQuantity(i)}));
        }
    }
    return *this;
}

Vendor::Vendor(Vendor&& v) {
    isRefrigeratedDisplay = v.getRefrigerationDisplay();
    int size = v.getSize();
    for (int i = 0; i < size; i++) {
        allStock.emplace(allStock.begin() + i, move(v.allStock[i]));
    }
    v.allStock.clear();
}
        
Vendor& Vendor::operator=(Vendor &&v) {
    if (this != &v) {
        allStock.clear();
        isRefrigeratedDisplay = v.getRefrigerationDisplay();
        int size = v.getSize();
        for (int i = 0; i < size; i++) {
            v.allStock[i].swap(allStock[i]);
        }
    }
    return *this;
}

Vendor::~Vendor() {
    allStock.clear();
}

void Vendor::load(vector<Entree> e, double p, unsigned int q) {
    if (p <= 0)
        throw invalid_argument("Price must be positive");
    allStock.push_back(unique_ptr<Stock>(new Stock{e, p, q}));
}

void Vendor::sell(Customer &c, string entreeName) {
    int index = isStocked(entreeName);
    if (index == -1)
        throw invalid_argument("Entree not in vendor");
    if (c.getBalance() < allStock[index]->price)
        throw invalid_argument("Customer balance too low");
    if (allStock[index] == 0)
        throw invalid_argument("No entrees in stock");
    c.purchase(allStock[index]->price);
    allStock[index]->quantity--;
    if (allStock[index]->quantity == 0)
        allStock.erase(allStock.begin() + index);
    else
        allStock[index]->food.erase(allStock[index]->food.begin() + 
        allStock[index]->quantity);
}

void Vendor::powerOutage() {
    for (auto const &i : allStock) {
        int s = i->quantity;
        for (int j = 0; j < s; j++)
            i->food[j].powerOutage();
    }
}

void Vendor::cleanStock() {
    int count = 0;
    for (auto const &i : allStock) {
        int s = i->quantity;
        for (int j = 0; j < s; j++) {
            if (i->food[j].isSpoiled()) {
                i->food.erase(i->food.begin() + j);
                i->quantity--;
            }
        }
        if (i->quantity == 0)
            allStock.erase(allStock.begin() + count);
        count++;
    }
}

int Vendor::isStocked(string entreeName) {
    for (int i = 0; i < int(entreeName.length()); i++)
        entreeName[i] = tolower(entreeName[i]);
    int index = 0;
    for (auto const &i : allStock) {
        if (i->food[0].getName() == entreeName)
            return index;
        index++;
    }
    return -1;
} 

unsigned int Vendor::getQuantity(int i) const {
    if (i > int(allStock.size()) || i < 0 || allStock.size() == 0)
        throw out_of_range("index not in vector");
    return allStock[i]->quantity;
}

vector<Entree> Vendor::getEntree(int i) const {
    if (i > int(allStock.size()) || i < 0 || allStock.size() == 0)
        throw out_of_range("index not in vector");
    return allStock[i]->food;
}

double Vendor::getPrice(int i) const {
    if (i > int(allStock.size()) || i < 0 || allStock.size() == 0)
        throw out_of_range("index not in vector");
    return allStock[i]->price;
}

int Vendor::getSize() const {
    return allStock.size();
}

bool Vendor::getRefrigerationDisplay() const {
    return isRefrigeratedDisplay;
}