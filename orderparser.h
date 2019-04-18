//
// Created by coder on 4/18/19.
//

#ifndef MATCHINGENGINE_ORDERPARSER_H
#define MATCHINGENGINE_ORDERPARSER_H

#include "header.h"

enum enOrderType
{
    ORDER_BUY,
    ORDER_SELL
};

class orderParser {
    string ORDER_TYPE;
    string CMP_NAME;     //company name
    int QTY;            //quantity
    float PRICE;       //price
    long long ORDER_ID;  //unique order ID for each order
public:
    void validateInput(vector<string>results);
    void generateOrderID();
    void parseInput(string input);
    enOrderType getOrderType();  //return an enum

};

#endif //MATCHINGENGINE_ORDERPARSER_H
