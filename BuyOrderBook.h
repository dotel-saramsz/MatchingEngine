//
// Created by dotelsaramsha on 5/14/19.
//

#ifndef ORDERMATCHING_BUYORDERBOOK_H
#define ORDERMATCHING_BUYORDERBOOK_H

#include "OrderBook.h"
#include "OrderContainer.h"

class BuyOrderBook:public OrderBook {
private:
    maxPriceHeap container;
public:
    BuyOrderBook();
    void push(OrderPoint*) override;
    OrderPoint* pop() override;
};


#endif //ORDERMATCHING_BUYORDERBOOK_H
