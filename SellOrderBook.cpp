//
// Created by dotelsaramsha on 5/14/19.
//

#include "SellOrderBook.h"


SellOrderBook::SellOrderBook(){

}

void SellOrderBook::push(OrderPoint *order) {
    container.push(order);
}

OrderPoint* SellOrderBook::pop() {
    OrderPoint* topOrder = container.top(); //returns a const type. Probably, will need std::move()
    container.pop();
    return topOrder;
}