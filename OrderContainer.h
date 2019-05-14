//
// Created by dotelsaramsha on 5/14/19. Header file with definitions of the heap data-types.
//

#ifndef ORDERMATCHING_ORDERCONTAINER_H
#define ORDERMATCHING_ORDERCONTAINER_H

#include <queue>
#include <vector>
#include "OrderPoint.h"

using namespace std;

struct maxPriceComparator {
    bool operator() (const OrderPoint*, const OrderPoint*);
};

struct minPriceComparator {
    bool operator() (const OrderPoint*, const OrderPoint*);
};

struct maxQtyComparator {
    bool operator() (const OrderPoint*, const OrderPoint*);
};

struct minQtyComparator {
    bool operator() (const OrderPoint*, const OrderPoint*);
};


typedef priority_queue<OrderPoint*, vector<OrderPoint*>, maxPriceComparator> maxPriceHeap;
typedef priority_queue<OrderPoint*, vector<OrderPoint*>, minPriceComparator> minPriceHeap;
typedef priority_queue<OrderPoint*, vector<OrderPoint*>, maxQtyComparator> maxQtyHeap;
typedef priority_queue<OrderPoint*, vector<OrderPoint*>, minQtyComparator> minQtyHeap;

//class OrderContainer
//{
//public:
//    containerType type;
//    maxPriceHeap maxP;
//    minPriceHeap minP;
//    maxQtyHeap maxQ;
//    minQtyHeap minQ;
//    vector<OrderPoint*> list;
//
//    OrderContainer(containerType);
//
//};
#endif //ORDERMATCHING_ORDERCONTAINER_H