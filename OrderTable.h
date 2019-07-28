//
// Created by dotelsaramsha on 4/29/19.
//

#ifndef ORDERMATCHING_ORDERTABLE_H
#define ORDERMATCHING_ORDERTABLE_H

// This class is the abstract class that represents the order table for preorder session. It is implementation
// independent, i.e. for its implementation, it is inherited by the respective class.
#include "OrderPoint.h"
#include "OrderContainer.h"
#include "BuyOrderBook.h"
#include "SellOrderBook.h"
#include <vector>
#include <list>


class OrderTable {
protected:
    //Common members for any implementation of the order table
    float equilibriumPrice;
    vector<pair<float,long> > equilibriumRows;    //stores those orders which have the maximal tradable qty in their tablerow
    long maxTradableQty;
    long unmatchedQty;
public:
    //The categorical containers where orders are placed into as they are parsed after eq price calculation
    list<OrderPoint*> eligibleBuy;
    list<OrderPoint*> eligibleSell;
    BuyOrderBook* pendingBuy;
    SellOrderBook* pendingSell;

    virtual void insert(OrderPoint*) = 0;
    virtual void remove(OrderPoint*) = 0;
    virtual void lookup(OrderPoint*) = 0;
    virtual void calculateEQprice() = 0;
    virtual void matchPreOpen(BuyOrderBook*, SellOrderBook*) = 0;

    OrderTable();


};


#endif //ORDERMATCHING_ORDERTABLE_H
