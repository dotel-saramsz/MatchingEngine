//
// Created by dotelsaramsha on 4/29/19.
//

#ifndef ORDERMATCHING_TABLEROW_H
#define ORDERMATCHING_TABLEROW_H

//This class is the data element that will be in each row of the order table, i.e
//The data element of each node of the AVL tree implementation
//The data element to be entered into the map of the Map implementation

#include <vector>
#include "OrderPoint.h"

using namespace std;

class TableRow {
public:
    float price;
    vector<OrderPoint*> orders;
    long buyQty;
    long sellQty;
    long demandQty;
    long supplyQty;
    long tradableQty;
    long unmatchedQty;

    TableRow(float price,OrderPoint* order,long buyQty,long sellQty);

    void addOrder(OrderPoint* order);

};


#endif //ORDERMATCHING_TABLEROW_H
