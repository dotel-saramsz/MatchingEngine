//
// Created by dotelsaramsha on 4/29/19.
//

#include "MapTable.h"
#include <iostream>

using namespace std;

void MapTable::insert(OrderPoint* orderPoint) {
    int value = insert(orderPoint->price);
    cout<<"In the MapTable overridden insert with value "<<value<<endl;
    cout<<"OrderID= "<<orderPoint->orderID<<endl;
}

void MapTable::remove(OrderPoint* orderPoint) {

}

void MapTable::lookup(OrderPoint* orderPoint) {

}

int MapTable::insert(float data) {
    cout<<"In the MapTable int insert with data "<<data<<endl;
    return 20;
}

MapTable::MapTable() {

}

void MapTable::calculateEQprice() {

}
