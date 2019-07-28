//
// Created by dotelsaramsha on 4/29/19.
//

#ifndef ORDERMATCHING_STLMAPTABLE_H
#define ORDERMATCHING_STLMAPTABLE_H

#include "OrderTable.h"
#include "TableRow.h"
#include <map>

using namespace std;

class STLmapTable: public OrderTable {
private:
    //Here, the class that represents the tree node is to be defined
    //map class that maps order price in float to TableRow (which is a row of the order table with order(s)
    map<float, TableRow*> Node;

public:
    void insert(OrderPoint*) override;

    long forwardparse() override;

    long reverseparse() override;

    STLmapTable();

    void categorizeOrder() override;

};


#endif //ORDERMATCHING_AVLTABLE_H
