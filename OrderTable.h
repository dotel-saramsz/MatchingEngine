//
// Created by dotelsaramsha on 4/29/19.
//

#ifndef ORDERMATCHING_ORDERTABLE_H
#define ORDERMATCHING_ORDERTABLE_H

// This class is the abstract class that represents the order table for preorder session. It is implementation
// independent, i.e. for its implementation, it is inherited by the respective class.
#include "OrderPoint.h"
#include <vector>


class OrderTable {
protected:
    //Common members for any implementation of the order table
    float equilibriumPrice;
    vector<pair<float,long> > equilibriumOrders;    //stores those orders which have the maximal tradable qty in their tablerow
    long maxTradableQty;

public:
    virtual void insert(OrderPoint*) = 0;
    virtual void remove(OrderPoint*) = 0;
    virtual void lookup(OrderPoint*) = 0;
    virtual void calculateEQprice() = 0;

    OrderTable();


};


#endif //ORDERMATCHING_ORDERTABLE_H
