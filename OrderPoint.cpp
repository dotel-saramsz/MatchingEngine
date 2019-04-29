//
// Created by dotelsaramsha on 4/29/19.
//
#include <chrono>
#include <cstring>
#include "OrderPoint.h"

OrderPoint::OrderPoint(bool type, string companyID, float price, long shareQty, long clientID) {
    //assign values
    this->type = type;
    this->companyID = companyID;    //potential for performance tuning
    this->price = price;
    this->shareQty = shareQty;
    this->clientID = clientID;

    //set timestamp (in millis)
    timestamp = getOrderTime();

    //set orderID as companyID<orderedTimestamp>
    orderID = companyID + to_string(timestamp);
}

long OrderPoint::getOrderTime() {
    //retrieve the current system time as millis since epoch and return it
    chrono::milliseconds ms = chrono::duration_cast< chrono::milliseconds >(
            chrono::system_clock::now().time_since_epoch()
    );
    return ms.count();
}
