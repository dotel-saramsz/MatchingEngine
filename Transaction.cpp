//
// Created by dotelsaramsha on 5/14/19.
//

#include "Transaction.h"
#include<iostream>

using namespace std;

Transaction::Transaction(string buyOrderID, string sellOrderID, string companyID, float price, long shareQty) {
    this->buyOrderID = buyOrderID;
    this->sellOrderID = sellOrderID;
    this->companyID = companyID;
    this->price = price;
    this->shareQty = shareQty;

    this->timestamp = this->getMatchTime();

    this->transID = "TRANS_" + to_string(this->timestamp);
}

long Transaction::getMatchTime() {
    //retrieve the current system time as nanoseconds since epoch and return it
    chrono::nanoseconds ns = chrono::duration_cast<chrono::nanoseconds>(
            chrono::system_clock::now().time_since_epoch()
    );
    return ns.count();
}

void Transaction::display() {
    cout<<transID<<" | "<<shareQty<<" | "<<buyOrderID<<" | "<<sellOrderID<<endl;
}
