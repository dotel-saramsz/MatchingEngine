//
// Created by dotelsaramsha on 5/14/19.
//

#ifndef ORDERMATCHING_SELLORDERBOOK_H
#define ORDERMATCHING_SELLORDERBOOK_H

#include "OrderContainer.h"
#include "OrderBook.h"

class SellOrderBook:public OrderBook {
private:
    minPriceHeap container;
public:
    SellOrderBook();
    void push(OrderPoint*) override;
    OrderPoint* pop() override;
};


#endif //ORDERMATCHING_SELLORDERBOOK_H
