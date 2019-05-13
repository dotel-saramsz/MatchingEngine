//
// Created by dotelsaramsha on 4/29/19.
//

#ifndef ORDERMATCHING_ORDERPOINT_H
#define ORDERMATCHING_ORDERPOINT_H

//This is the class for an orderpoint. It will represent any order that is entered by the user and needs to be matched

//CHECK: getter and setter will cause function call overhead and reduce efficiency? Is open class worth it?


#include <string>
#include <chrono>

using namespace std;

class OrderPoint {
public:
    string orderID;   //TODO: automatically generate order ID before inserting into the table row in preopen
    enum orderType {BUY,SELL};   //0: BUY, 1: SELL
    orderType type;
    string companyID;  //In assumption that company ID wont be more than 6 characters. Use strcpy to copy by value
    float price;
    long shareQty;
    long clientID;
    long timestamp;
    int testfile;
    // the timestamp is to be filled by ns.count() where ns is of type chrono::nanoseconds (nano seconds since epoch)
    OrderPoint(orderType type, string companyID, float price, long shareQty, long clientID);

    long getOrderTime();
};


#endif //ORDERMATCHING_ORDERPOINT_H
