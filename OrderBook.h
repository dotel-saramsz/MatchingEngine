//
// Created by dotelsaramsha on 3/6/19.
//

#ifndef ORDERMATCHING_ORDERBOOK_H
#define ORDERMATCHING_ORDERBOOK_H

#include "OrderPoint.h"

class OrderBook {
public:
    virtual void push(OrderPoint*) = 0;
    virtual OrderPoint* pop() = 0;

    OrderBook() = default;
};


#endif //ORDERMATCHING_ORDERBOOK_H
