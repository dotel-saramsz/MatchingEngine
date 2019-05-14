//
// Created by dotelsaramsha on 5/14/19.
//
#include "OrderContainer.h"


bool maxPriceComparator::operator() (const OrderPoint* a, const OrderPoint* b){
    return a->price < b->price;
}

bool minPriceComparator::operator() (const OrderPoint* a, const OrderPoint* b){
    return a->price > b->price;
}

bool maxQtyComparator::operator() (const OrderPoint* a, const OrderPoint* b){
    return a->shareQty < b->shareQty;
}

bool minQtyComparator::operator() (const OrderPoint* a, const OrderPoint* b){
    return a->shareQty < b->shareQty;
}

