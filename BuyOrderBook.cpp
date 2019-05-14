//
// Created by dotelsaramsha on 5/14/19.
//

#include "BuyOrderBook.h"

BuyOrderBook::BuyOrderBook(){

}

void BuyOrderBook::push(OrderPoint *order) {
    container.push(order);
}

OrderPoint* BuyOrderBook::pop() {
    OrderPoint* topOrder = container.top(); //returns a const type. Probably, will need std::move()
    container.pop();
    return topOrder;
}