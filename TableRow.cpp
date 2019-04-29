//
// Created by dotelsaramsha on 4/29/19.
//

#include "TableRow.h"

TableRow::TableRow(float price, OrderPoint *order, long buyQty, long sellQty) {
    this->price = price;
    this->buyQty = buyQty;
    this->sellQty = sellQty;
    orders.push_back(order);    //adds the first order to the vector of orders
    //Note: demandQty and supplyQty are set on the forward and reverse pass after all orders have been entered.
}

void TableRow::addOrder(OrderPoint *order) {
    orders.push_back(order);
}
