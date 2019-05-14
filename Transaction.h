//
// Created by dotelsaramsha on 5/14/19.
//

#ifndef ORDERMATCHING_TRANSACTION_H
#define ORDERMATCHING_TRANSACTION_H

#include <string>
#include <chrono>

using namespace std;

class Transaction {
public:
    string transID;
    float price;
    long shareQty;
    string buyOrderID;
    string sellOrderID;
    long timestamp;
    string companyID;

    long getMatchTime();

    void display();

    Transaction(string buyOrderID, string sellOrderID, string companyID, float price, long shareQty);
};


#endif //ORDERMATCHING_TRANSACTION_H
